/* Copyright 2016-2024
 * Kaz Kylheku <kaz@kylheku.com>
 * Vancouver, Canada
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include "config.h"
#include "alloca.h"
#if HAVE_POLL
#include <poll.h>
#elif HAVE_SELECT
#include <sys/select.h>
#endif
#if HAVE_SYS_TIME
#include <sys/time.h>
#endif
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "lib.h"
#include "stream.h"
#include "signal.h"
#include "utf8.h"
#include "unwind.h"
#include "gc.h"
#include "eval.h"
#include "args.h"
#include "struct.h"
#include "arith.h"
#include "sysif.h"
#include "itypes.h"
#include "ffi.h"
#include "txr.h"
#include "autoload.h"
#include "socket.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))

struct dgram_stream {
  struct strm_base a;
  val stream;
  val family;
  val peer;
  val addr;
  val unget_c;
  utf8_decoder_t ud;
  struct sockaddr_storage peer_addr;
  socklen_t pa_len;
  int fd;
  int err;
  mem_t *rx_buf;
  mem_t *tx_buf;
  int rx_max, rx_size, rx_pos;
  int tx_pos;
  unsigned is_connected : 8;
  unsigned is_byte_oriented : 8;
};

val sockaddr_in_s, sockaddr_in6_s, sockaddr_un_s, addrinfo_s;
val flags_s, family_s, socktype_s, protocol_s, addr_s, canonname_s;
val port_s, flow_info_s, scope_id_s;

static val ipv4_addr_to_num(struct in_addr *src)
{
  return num_from_buffer(coerce(mem_t *, &src->s_addr), 4);
}

static void ipv4_addr_from_num(struct in_addr *dst, val addr)
{
  if (!num_to_buffer(addr, coerce(mem_t *, &dst->s_addr), 4))
    uw_throwf(socket_error_s, lit("~s out of range for IPv4 address"),
              addr, nao);
}

static val ipv6_addr_to_num(struct in6_addr *src)
{
  return num_from_buffer(src->s6_addr, 16);
}

static void ipv6_addr_from_num(struct in6_addr *dst, val addr)
{
  if (!num_to_buffer(addr, dst->s6_addr, 16))
    uw_throwf(socket_error_s, lit("~s out of range for IPv6 address"),
              addr, nao);
}

static void ipv6_flow_info_from_num(struct sockaddr_in6 *dst, val flow)
{
  if (!num_to_buffer(flow, coerce(mem_t *, &dst->sin6_flowinfo), 4))
    uw_throwf(socket_error_s, lit("~s out of range for IPv6 flow info"),
              flow, nao);
}

static void ipv6_scope_id_from_num(struct sockaddr_in6 *dst, val scope)
{
  if (!num_to_buffer(scope, coerce(mem_t *, &dst->sin6_scope_id), 4))
    uw_throwf(socket_error_s, lit("~s out of range for IPv6 scope ID"),
              scope, nao);
}

static val sockaddr_in_unpack(struct sockaddr_in *src)
{
  args_decl_constsize(args, ARGS_MIN);
  val out = make_struct(sockaddr_in_s, nil, args);
  slotset(out, addr_s, ipv4_addr_to_num(&src->sin_addr));
  slotset(out, port_s, num_fast(ntohs(src->sin_port)));
  return out;
}

static val sockaddr_in6_unpack(struct sockaddr_in6 *src)
{
  args_decl_constsize(args, ARGS_MIN);
  val out = make_struct(sockaddr_in6_s, nil, args);
  slotset(out, addr_s, ipv6_addr_to_num(&src->sin6_addr));
  slotset(out, port_s, num_fast(ntohs(src->sin6_port)));
  return out;
}

static val sockaddr_un_unpack(struct sockaddr_un *src)
{
  args_decl_constsize(args, ARGS_MIN);
  val out = make_struct(sockaddr_un_s, nil, args);
  slotset(out, path_s, string_utf8(src->sun_path));
  return out;
}

static val sockaddr_unpack(int family, struct sockaddr_storage *src)
{
  switch (family) {
  case AF_INET:
   return sockaddr_in_unpack(coerce(struct sockaddr_in *, src));
  case AF_INET6:
    return sockaddr_in6_unpack(coerce(struct sockaddr_in6 *, src));
  case AF_UNIX:
    return sockaddr_un_unpack(coerce(struct sockaddr_un *, src));
  default:
    return nil;
  }
}

#if HAVE_GETADDRINFO

static void addrinfo_in(struct addrinfo *dest, val src, val self)
{
  dest->ai_flags = c_num(default_arg(slot(src, flags_s), zero), self);
  dest->ai_family = c_num(default_arg(slot(src, family_s), zero), self);
  dest->ai_socktype = c_num(default_arg(slot(src, socktype_s), zero), self);
  dest->ai_protocol = c_num(default_arg(slot(src, protocol_s), zero), self);
}

static val getaddrinfo_wrap(val node_in, val service_in, val hints_in)
{
  val self = lit("getaddrinfo");
  val node = default_arg(node_in, nil);
  val service = default_arg(service_in, nil);
  val hints = default_arg(hints_in, nil);
  struct addrinfo hints_ai, *phints = hints ? &hints_ai : 0, *alist = 0, *aiter;
  char *node_u8 = stringp(node) ? utf8_dup_to(c_str(node, self)) : 0;
  char *service_u8 = stringp(service) ? utf8_dup_to(c_str(service, self)) : 0;
  val node_num_p = integerp(node);
  val svc_num_p = integerp(service);
  int res = 0;
  list_collect_decl (out, ptail);

  uw_simple_catch_begin;

  if (hints) {
    memset(&hints_ai, 0, sizeof hints_ai);
    addrinfo_in(&hints_ai, hints, self);
  }

  res = getaddrinfo(node_u8, service_u8, phints, &alist);

  uw_unwind {
    free(node_u8);
    free(service_u8);
  }

  uw_catch_end;

  if (res == 0) {
    val canonname = nil;
    for (aiter = alist; aiter; aiter = aiter->ai_next) {
      val addr = nil;
      switch (aiter->ai_family) {
      case AF_INET:
        {
          struct sockaddr_in *sa = coerce(struct sockaddr_in *, aiter->ai_addr);
          if (node_num_p)
            ipv4_addr_from_num(&sa->sin_addr, node);
          if (svc_num_p)
            sa->sin_port = htons(c_num(service, self));
          addr = sockaddr_in_unpack(sa);
        }
        break;
      case AF_INET6:
        {
          struct sockaddr_in6 *sa = coerce(struct sockaddr_in6 *, aiter->ai_addr);
          if (node_num_p)
            ipv6_addr_from_num(&sa->sin6_addr, node);
          if (svc_num_p)
            sa->sin6_port = ntohs(c_num(service, self));
          addr = sockaddr_in6_unpack(sa);
        }
        break;
      }
      if (addr) {
        if (aiter == alist && (hints_ai.ai_flags & AI_CANONNAME) != 0
            && aiter->ai_canonname)
          canonname = string_utf8(aiter->ai_canonname);
        if (canonname)
          slotset(addr, canonname_s, canonname);
        ptail = list_collect(ptail, addr);
      }
    }
  }

  /* Stupidly, POSIX doesn't require freeaddrinfo(0) to work. */
  if (alist)
    freeaddrinfo(alist);

  return out;
}

#endif

static void addr_mismatch(val addr, val family)
{
  uw_throwf(socket_error_s, lit("address ~s doesn't match address family ~s"),
            addr, family, nao);
}

static void sockaddr_pack(val sockaddr, val family,
                          struct sockaddr_storage *buf, socklen_t *len,
                          val self)
{
  val addr_type = typeof(sockaddr);

  if (addr_type == sockaddr_in_s) {
    val addr = slot(sockaddr, addr_s);
    val port = slot(sockaddr, port_s);
    struct sockaddr_in *sa = coerce(struct sockaddr_in *, buf);
    if (family != num_fast(AF_INET))
      addr_mismatch(sockaddr, family);
    memset(sa, 0, sizeof *sa);
    sa->sin_family = AF_INET;
    ipv4_addr_from_num(&sa->sin_addr, addr);
    sa->sin_port = ntohs(c_num(port, self));
    *len = sizeof *sa;
  } else if (addr_type == sockaddr_in6_s) {
    val addr = slot(sockaddr, addr_s);
    val port = slot(sockaddr, port_s);
    val flow = slot(sockaddr, flow_info_s);
    val scope = slot(sockaddr, scope_id_s);
    struct sockaddr_in6 *sa = coerce(struct sockaddr_in6 *, buf);
    if (family != num_fast(AF_INET6))
      addr_mismatch(sockaddr, family);
    memset(sa, 0, sizeof *sa);
    sa->sin6_family = AF_INET6;
    ipv6_addr_from_num(&sa->sin6_addr, addr);
    ipv6_flow_info_from_num(sa, flow);
    ipv6_scope_id_from_num(sa, scope);
    sa->sin6_port = ntohs(c_num(port, self));
    *len = sizeof *sa;
  } else if (addr_type == sockaddr_un_s) {
    val path = slot(sockaddr, path_s);
    struct sockaddr_un *sa = coerce(struct sockaddr_un *, buf);
    size_t size;
    unsigned char *path_u8 = utf8_dup_to_buf(c_str(path, self), &size, 0);
    memset(sa, 0, sizeof *sa);
    sa->sun_family = AF_UNIX;
    memcpy(sa->sun_path, path_u8, MIN(size, sizeof sa->sun_path));
    free(path_u8);
    *len = sizeof *sa;
  } else {
    uw_throwf(socket_error_s, lit("object ~s isn't a socket address"),
              sockaddr, nao);
  }
}

static_forward(struct strm_ops dgram_strm_ops);

static val make_dgram_sock_stream(int fd, val family, val peer,
                                  mem_t *dgram, int dgram_size,
                                  struct sockaddr *peer_addr, socklen_t pa_len,
                                  struct stdio_mode m, struct dgram_stream *d_proto)
{
  struct dgram_stream *d = coerce(struct dgram_stream *,
                                  chk_malloc(sizeof *d));
  val stream;

  strm_base_init(&d->a);
  d->stream = nil;
  d->fd = fd;
  d->family = d->peer = d->addr = d->unget_c = nil;
  d->err = 0;
  d->rx_buf = dgram;
  d->rx_max = (m.buforder == -1)
               ? d_proto ? d_proto->rx_max : 65536
               : 1024 << m.buforder;
  d->rx_size = dgram_size;
  d->rx_pos = 0;
  d->tx_buf = 0;
  d->tx_pos = 0;
  utf8_decoder_init(&d->ud);
  if (peer_addr != 0)
    memcpy(&d->peer_addr, peer_addr, pa_len);
  d->pa_len = pa_len;
  stream = cobj(coerce(mem_t *, d), stream_cls, &dgram_strm_ops.cobj_ops);
  d->stream = stream;
  d->family = family;
  d->peer = peer;
  d->is_connected = 0;
  return stream;
}

static void dgram_print(val stream, val out, val pretty, struct strm_ctx *ctx)
{
  struct strm_ops *ops = coerce(struct strm_ops *, stream->co.ops);
  val name = static_str(ops->name);
  val descr = ops->get_prop(stream, name_k);

  (void) pretty;
  (void) ctx;

  format(out, lit("#<~a ~a ~p>"), name, descr, stream, nao);
}

static void dgram_mark(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  strm_base_mark(&d->a);
  /* h->stream == stream and so no need to mark h->stream */
  gc_mark(d->family);
  gc_mark(d->peer);
  gc_mark(d->addr);
  gc_mark(d->unget_c);
}

static void dgram_destroy(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  free(d->rx_buf);
  free(d->tx_buf);
  d->rx_buf = d->tx_buf = 0;
  free(d);
}

static void dgram_overflow(val stream)
{
  /*
   * Not called under present logic, because dgram_put_byte_callback keeps
   * increasing the datagram size.
   */
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  errno = d->err = ENOBUFS;
  uw_ethrowf(socket_error_s, lit("dgram write overflow on ~s"), stream, nao);
}

static int dgram_put_byte_callback(int b, mem_t *ctx)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, ctx);
  d->tx_buf = chk_manage_vec(d->tx_buf, d->tx_pos, d->tx_pos + 1, 1, 0);
  d->tx_buf[d->tx_pos++] = b;
  return 1;
}

static val dgram_put_string(val stream, val str)
{
  val self = lit("put-string");
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  const wchar_t *s = c_str(str, self);

  while (*s) {
    if (!utf8_encode(*s++, dgram_put_byte_callback, coerce(mem_t *, d)))
      dgram_overflow(stream);
  }

  return t;
}

static val dgram_put_char(val stream, val ch)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  if (!utf8_encode(c_chr(ch), dgram_put_byte_callback, coerce(mem_t *, d)))
    dgram_overflow(stream);
  return t;
}

static val dgram_put_byte(val stream, int b)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  if (!dgram_put_byte_callback(b, coerce(mem_t *, d)))
    dgram_overflow(stream);
  return t;
}

static int dgram_get_byte_callback(mem_t *ctx)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, ctx);
  if (d->rx_buf) {
    return (d->rx_pos < d->rx_size) ? d->rx_buf[d->rx_pos++] : EOF;
  } else {
    const int dgram_size = d->rx_max;
    mem_t *dgram = chk_malloc(dgram_size);
    volatile ssize_t nbytes = -1;

    uw_simple_catch_begin;

    sig_save_enable;

    nbytes = recv(d->fd, dgram, dgram_size, 0);

    sig_restore_enable;

    if (nbytes == -1) {
      d->err = errno;
      uw_ethrowf(socket_error_s,
                 lit("get-byte: recv on ~s failed: ~d/~s"),
                 d->stream, num(errno), errno_to_str(errno), nao);
    }

    uw_unwind {
      if (nbytes == -1)
        free(dgram);
    }

    uw_catch_end;

    d->rx_buf = chk_realloc(dgram, nbytes);

    if (!d->rx_buf)
      d->rx_buf = dgram;

    d->rx_size = nbytes;

    return dgram_get_byte_callback(ctx);
  }
}

static val dgram_get_char(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  if (d->unget_c) {
    return rcyc_pop(&d->unget_c);
  } else {
    wint_t ch;

    if (d->is_byte_oriented) {
      ch = dgram_get_byte_callback(coerce(mem_t *, d));
      if (ch == 0)
        ch = 0xDC00;
    } else {
      ch = utf8_decode(&d->ud, dgram_get_byte_callback,
                       coerce(mem_t *, d));
    }

    return (ch != WEOF) ? chr(ch) : nil;
  }
}

static val dgram_get_byte(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  int b = dgram_get_byte_callback(coerce(mem_t *, d));
  return b == EOF ? nil : num_fast(b);
}

static val dgram_unget_char(val stream, val ch)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  mpush(ch, mkloc(d->unget_c, stream));
  return ch;
}

static val dgram_unget_byte(val stream, int byte)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  if (d->rx_pos <= 0) {
    d->err = EOVERFLOW;
    uw_throwf(file_error_s,
              lit("unget-byte: cannot push back past start of stream ~s"),
              stream, nao);
  }

  d->rx_buf[--d->rx_pos] = byte;
  return num_fast(byte);
}

static val dgram_flush(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  if (d->fd != -1 && d->tx_buf) {
    if (d->peer) {
      int nwrit = d->is_connected
                  ? send(d->fd, d->tx_buf, d->tx_pos, 0)
                  : sendto(d->fd, d->tx_buf, d->tx_pos, 0,
                           coerce(struct sockaddr *, &d->peer_addr),
                           d->pa_len);

      if (nwrit != d->tx_pos) {
        d->err = (nwrit < 0) ? errno : ENOBUFS;
        uw_ethrowf(socket_error_s,
                   lit("flush-stream: sendto on ~s ~a: ~d/~s"),
                   stream,
                   (nwrit < 0) ? lit("failed") : lit("truncated"),
                   num(errno), errno_to_str(errno), nao);
      }

      free(d->tx_buf);
      d->tx_buf = 0;
      d->tx_pos = 0;
    } else {
      errno = d->err = ENOTCONN;
      uw_ethrowf(socket_error_s,
                 lit("flush-stream: cannot transmit on ~s: peer not set"),
                 stream, nao);
    }
  }
  return t;
}

static val dgram_close(val stream, val throw_on_error)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  (void) throw_on_error;

  if (d->fd != -1) {
    dgram_flush(stream);
    close(d->fd);
    d->fd = -1;
    d->err = 0;
    return t;
  }

  return nil;
}

static val dgram_get_prop(val stream, val ind)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  if (ind == name_k) {
    if (d->fd == -1)
      return lit("closed");

    if (d->addr)
      return format(nil, lit("passive ~s"), d->addr, nao);

    if (d->peer)
      return format(nil, lit("connected ~s"), d->peer, nao);

    return lit("disconnected");
  } else if (ind == byte_oriented_k) {
    return d->is_byte_oriented ? t : nil;
  }

  return nil;
}

static val dgram_set_prop(val stream, val ind, val prop)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  if (ind == addr_k) {
    set(mkloc(d->addr, stream), prop);
    return t;
  } else if (ind == byte_oriented_k) {
    d->is_byte_oriented = prop ? 1 : 0;
    return t;
  }

  return nil;
}

static val dgram_get_error(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  if (d->err)
    return num(d->err);
  if (d->rx_buf && d->rx_pos == d->rx_size)
    return t;
  return nil;
}

static val dgram_get_error_str(val stream)
{
  return errno_to_string(dgram_get_error(stream));
}

static val dgram_clear_error(val stream)
{
  val ret = dgram_get_error(stream);
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);

  d->err = 0;

  if (d->rx_pos == d->rx_size) {
    d->rx_pos = d->rx_size = 0;
    free(d->rx_buf);
    d->rx_buf = 0;
  }

  return ret;
}

static val dgram_get_fd(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  return num(d->fd);
}

static val dgram_get_sock_family(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  return d->family;
}

static val dgram_get_sock_type(val stream)
{
  (void) stream;
  return num_fast(SOCK_DGRAM);
}

static val dgram_get_sock_peer(val stream)
{
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  return d->peer;
}

static val dgram_set_sock_peer(val stream, val peer)
{
  val self = lit("sock-set-peer");
  struct dgram_stream *d = coerce(struct dgram_stream *, stream->co.handle);
  sockaddr_pack(peer, d->family, &d->peer_addr, &d->pa_len, self);
  return set(mkloc(d->peer, stream), peer);
}

static_def(struct strm_ops dgram_strm_ops =
  strm_ops_init(cobj_ops_init(eq,
                              dgram_print,
                              dgram_destroy,
                              dgram_mark,
                              cobj_eq_hash_op,
                              0),
                wli("dgram-sock"),
                dgram_put_string,
                dgram_put_char,
                dgram_put_byte,
                generic_get_line,
                dgram_get_char,
                dgram_get_byte,
                dgram_unget_char,
                dgram_unget_byte,
                0,
                0,
                dgram_close,
                dgram_flush,
                0,
                0,
                dgram_get_prop,
                dgram_set_prop,
                dgram_get_error,
                dgram_get_error_str,
                dgram_clear_error,
                dgram_get_fd));

static val sock_bind(val sock, val sockaddr)
{
  val self = lit("sock-bind");
  val sfd = stream_fd(sock);

  if (sfd) {
    int fd = c_num(sfd, self);
    val family = sock_family(sock);
    struct sockaddr_storage sa;
    socklen_t salen;
    int reuse = 1;

    (void) setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_pack(sockaddr, family, &sa, &salen, self);

    if (bind(fd, coerce(struct sockaddr *, &sa), salen) != 0)
      uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
                 self, num(errno), errno_to_str(errno), nao);

    stream_set_prop(sock, addr_k, sockaddr);
    return t;
  }

  uw_throwf(socket_error_s, lit("~a: cannot bind ~s"), self, sock, nao);
}

#if HAVE_POLL

static int fd_timeout(int fd, val timeout, int write, val self)
{
    cnum ms = c_num(timeout, self) / 1000;
    int pollms = (ms > INT_MAX) ? INT_MAX : ms;
    struct pollfd pfd;
    int res;

    if (write)
      pfd.events = POLLOUT;
    else
      pfd.events = POLLIN;

    pfd.fd = fd;

    sig_save_enable;

    res = poll(&pfd, 1, pollms);

    sig_restore_enable;

    return res;
}

#elif HAVE_SELECT

static int fd_timeout(int fd, val timeout, int write, val self)
{
    cnum us = c_num(timeout, self);
    struct timeval tv;
    fd_set fds;

    tv.tv_sec = us / 1000000;
    tv.tv_usec = us % 1000000;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    sig_save_enable;

    if (write)
      res = select(fd + 1, 0 ,&rfds, 0, &tv);
    else
      res = select(fd + 1, &rfds, 0, 0, &tv);


    sig_restore_enable;

    return res;
}

#endif

static int to_connect(int fd, struct sockaddr *addr, socklen_t len,
                      val sock, val sockaddr, val timeout, val self)
{
  int res;

#if HAVE_POLL || HAVE_SELECT
  if (timeout) {
    int flags = fcntl(fd, F_GETFL);

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
      return -1;

    res = connect(fd, addr, len);

    fcntl(fd, F_SETFL, flags);

    switch (res) {
    case 0:
      return 0;
    case -1:
      if (errno != EINPROGRESS)
        return -1;
      break;
    }

    res = fd_timeout(fd, timeout, 1, self);

    switch (res) {
    case -1:
      return -1;
    case 0:
      uw_ethrowf(timeout_error_s, lit("~a: ~s: timeout on ~s"),
                 self, sock, sockaddr, nao);
    default:
      return 0;
    }
  }
#else
  (void) timeout;
#endif

  sig_save_enable;

  res = connect(fd, addr, len);

  sig_restore_enable;

  return res;
}

static val open_sockfd(val fd, val family, val type, val mode_str, val self)
{
  struct stdio_mode m, m_rpb = stdio_mode_init_rpb;

  if (type == num_fast(SOCK_DGRAM)) {
    return make_dgram_sock_stream(c_num(fd, self), family, nil, 0, 0, 0, 0,
                                  parse_mode(mode_str, m_rpb, self), 0);
  } else {
    FILE *f = (errno = 0, w_fdopen(c_num(fd, self),
                                   c_str(normalize_mode(&m, mode_str,
                                                        m_rpb, self),
                                         self)));

    if (!f) {
      int eno = errno;
      close(c_num(fd, self));
      uw_ethrowf(errno_to_file_error(eno), lit("error creating stream for socket ~a: ~d/~s"),
                 fd, num(eno), errno_to_str(eno), nao);
    }

    return set_mode_props(m, make_sock_stream(f, family, type));
  }
}

static val sock_connect(val sock, val sockaddr, val timeout)
{
  val self = lit("sock-connect");
  val sfd = stream_fd(sock);

  if (sfd) {
    val family = sock_family(sock);
    struct sockaddr_storage sa;
    socklen_t salen;

    sockaddr_pack(sockaddr, family, &sa, &salen, self);

    if (to_connect(c_num(sfd, self), coerce(struct sockaddr *, &sa), salen,
                   sock, sockaddr, default_null_arg(timeout), self) != 0)
      uw_ethrowf(socket_error_s, lit("~a: ~s to addr ~s: ~d/~s"),
                 self, sock, sockaddr, num(errno), errno_to_str(errno), nao);

    sock_set_peer(sock, sockaddr);

    if (sock_type(sock) == num_fast(SOCK_DGRAM)) {
      struct dgram_stream *d = coerce(struct dgram_stream *, sock->co.handle);
      d->is_connected = 1;
    }

    return sock;
  }

  uw_throwf(socket_error_s, lit("~a: cannot connect ~s"), self, sock, nao);
}

static val sock_mark_connected(val sock, val self)
{
  val sfd = stream_fd(sock);

  if (sfd) {
    val family = sock_family(sock);
    struct sockaddr_storage sa = all_zero_init;
    socklen_t salen = sizeof sa;

    (void) getpeername(c_num(sfd, self), coerce(struct sockaddr *, &sa), &salen);

    sock_set_peer(sock, sockaddr_unpack(c_num(family, self), &sa));

    if (sock_type(sock) == num_fast(SOCK_DGRAM)) {
      struct dgram_stream *d = coerce(struct dgram_stream *, sock->co.handle);
      d->is_connected = 1;
    }

    return sock;
  }

  return nil;
}

static val sock_listen(val sock, val backlog)
{
  val self = lit("sock-listen");
  val sfd = stream_fd(sock);

  if (!sfd)
    uw_throwf(socket_error_s, lit("~a: cannot listen on ~s"),
              self, sock, nao);

  if (sock_type(sock) == num_fast(SOCK_DGRAM)) {
    if (sock_peer(sock)) {
      errno = EISCONN;
      goto failed;
    }
  } else {
    if (listen(c_num(sfd, self), c_num(default_arg(backlog, num_fast(16)), self)))
      goto failed;
  }

  return t;
failed:
    uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
               self, num(errno), errno_to_str(errno), nao);
}

static val sock_accept(val sock, val mode_str, val timeout_in)
{
  val self = lit("sock-accept");
  val sfd = stream_fd(sock);
  int fd = sfd ? c_num(sfd, self) : -1;
  val family = sock_family(sock);
  val type = sock_type(sock);
  struct sockaddr_storage sa;
  socklen_t salen = sizeof sa;
  val peer = nil;
  val timeout = default_null_arg(timeout_in);

  if (!sfd)
    goto badfd;

#if HAVE_POLL || HAVE_SELECT
  if (timeout) {
    int res = fd_timeout(fd, timeout, 0, self);

    switch (res) {
    case -1:
      goto badfd;
    case 0:
      uw_ethrowf(timeout_error_s, lit("~a: ~s: timeout"), self, sock, nao);
    default:
      break;
    }
  }
#else
  (void) timeout;
#endif

  if (type == num_fast(SOCK_DGRAM)) {
    struct dgram_stream *d = coerce(struct dgram_stream *, sock->co.handle);
    volatile ssize_t nbytes = -1;
    const int dgram_size = d->rx_max;
    mem_t *dgram = chk_malloc(dgram_size);

    if (sock_peer(sock)) {
      free(dgram);
      errno = EISCONN;
      goto failed;
    }

    uw_simple_catch_begin;

    sig_save_enable;

    nbytes = recvfrom(fd, dgram, dgram_size, 0,
                      coerce(struct sockaddr *, &sa), &salen);

    sig_restore_enable;

    uw_unwind {
      if (nbytes == -1)
        free(dgram);
    }

    uw_catch_end;

    if (nbytes == -1)
      goto failed;

    if (nilp(peer = sockaddr_unpack(c_num(family, self), &sa))) {
      free(dgram);
      uw_throwf(socket_error_s, lit("~a: ~s isn't a supported socket family"),
                self, family, nao);
    }

    {
      int afd = dup(fd);
      struct stdio_mode mode_rpb = stdio_mode_init_rpb;
      mem_t *shrink = chk_realloc(dgram, nbytes);

      if (shrink)
        dgram = shrink;

      if (afd == -1) {
        free(dgram);
        dgram = 0;
        goto failed;
      }
      return make_dgram_sock_stream(afd, family, peer, dgram, nbytes,
                                    coerce(struct sockaddr *, &sa), salen,
                                    parse_mode(mode_str, mode_rpb, self), d);
    }
  } else {
    int afd = -1;
    sig_save_enable;

    afd = accept(fd, coerce(struct sockaddr *, &sa), &salen);

    sig_restore_enable;

    if (afd < 0)
      goto failed;

    if (nilp(peer = sockaddr_unpack(c_num(family, self), &sa)))
      uw_throwf(socket_error_s, lit("~a: ~s isn't a supported socket family"),
                self, family, nao);

    {
      val stream = open_sockfd(num(afd), family, num_fast(SOCK_STREAM),
                               mode_str, self);
      sock_set_peer(stream, peer);
      return stream;
    }
  }
failed:
  uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
            self, num(errno), errno_to_str(errno), nao);
badfd:
  uw_ethrowf(socket_error_s, lit("~a: cannot accept on ~s"),
             self, sock, nao);
}

static val sock_shutdown(val sock, val how)
{
  val self = lit("sock-shutdown");
  val sfd = stream_fd(sock);

  flush_stream(sock);

  if (shutdown(c_num(sfd, self), c_num(default_arg(how, num_fast(SHUT_WR)), self)))
    uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
               self, num(errno), errno_to_str(errno), nao);

  return t;
}

#if HAVE_SYS_TIME && defined SO_SNDTIMEO && defined SO_RCVTIMEO
static val sock_timeout(val sock, val usec, val name, int which, val self)
{
  cnum fd = c_num(stream_fd(sock), self);
  cnum u = c_num(usec, self);
  struct timeval tv;

  tv.tv_sec = u / 1000000;
  tv.tv_usec = u % 1000000;

  if (setsockopt(fd, SOL_SOCKET, which, &tv, sizeof tv) != 0)
    uw_ethrowf(socket_error_s, lit("~a failed on ~s: ~d/~s"),
               name, sock, num(errno),
               errno_to_str(errno), nao);

  return sock;
}

static val sock_send_timeout(val sock, val usec)
{
  val self = lit("sock-send-timeout");
  return sock_timeout(sock, usec, self, SO_SNDTIMEO, self);
}

static val sock_recv_timeout(val sock, val usec)
{
  val self = lit("sock-recv-timeout");
  return sock_timeout(sock, usec, self, SO_RCVTIMEO, self);
}
#endif

#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK 0
#endif

#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC 0
#endif

static val open_socket(val family, val type, val mode_str)
{
  val self = lit("open-socket");
  int fd = socket(c_num(family, self), c_num(type, self), 0);

  if (fd < 0)
    uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
               self, num(errno), errno_to_str(errno), nao);

  if (SOCK_NONBLOCK | SOCK_CLOEXEC)
    type = num_fast(c_num(type, self) & ~(SOCK_NONBLOCK | SOCK_CLOEXEC));

  return open_sockfd(num(fd), family, type, mode_str, self);
}

static val socketpair_wrap(val family, val type, val mode_str)
{
  val self = lit("open-socket-pair");
  int sv[2] = { -1, -1 };
  int res = socketpair(c_num(family, self), c_num(type, self), 0, sv);
  val out = nil;

  uw_simple_catch_begin;

  if (res < 0)
    uw_ethrowf(socket_error_s, lit("~a failed: ~d/~s"),
               self, num(errno), errno_to_str(errno), nao);

  if (SOCK_NONBLOCK | SOCK_CLOEXEC)
    type = num_fast(c_num(type, self) & ~(SOCK_NONBLOCK | SOCK_CLOEXEC));

  {
    val s0 = open_sockfd(num(sv[0]), family, type, mode_str, self);
    val s1 = open_sockfd(num(sv[1]), family, type, mode_str, self);

    sock_mark_connected(s0, self);
    sock_mark_connected(s1, self);

    out = list(s0, s1, nao);
  }

  uw_unwind {
    if (!out) {
      if (sv[0] != -1)
        close(sv[0]);
      if (sv[1] != -1)
        close(sv[1]);
    }
  }

  uw_catch_end;

  return out;
}

static val sock_opt(val sock, val level, val option, val type_opt)
{
  val self = lit("sock-opt");
  val sfd = stream_fd(sock);
  int lvl = c_int(level, self);
  int opt = c_int(option, self);
  val type = default_arg(type_opt, ffi_type_lookup(int_s));
  struct txr_ffi_type *tft = ffi_type_struct_checked(self, type);

  if (!sfd) {
    uw_throwf(socket_error_s, lit("~a: cannot get option on ~s"),
              self, sock, nao);
  } else {
    socklen_t typesize = convert(socklen_t, ffi_type_size(tft));
    socklen_t size = typesize;
    mem_t *data = coerce(mem_t *, zalloca(size));
    if (getsockopt(c_num(sfd, self), lvl, opt, data, &size) != 0)
      uw_ethrowf(socket_error_s, lit("~a failed on ~s: ~d/~s"),
                 self, sock, num(errno), errno_to_str(errno), nao);
    /* TODO: Add a separate function to handle options with
     * variable-size values, for example the platform-specific
     * SO_BINDTODEVICE.
     * (Or perhaps add an optional argument following type_opt
     * specifying the requested length of the value, presumably of type
     * carray.) */
    if (size != typesize)
      uw_throwf(socket_error_s, lit("~a: variable-size option on ~s"),
                self, sock, nao);
    return ffi_type_get(tft, data, self);
  }
}

static val sock_set_opt(val sock, val level, val option, val value,
                        val type_opt)
{
  val self = lit("sock-set-opt");
  val sfd = stream_fd(sock);
  int lvl = c_int(level, self);
  int opt = c_int(option, self);
  val type = default_arg(type_opt, ffi_type_lookup(int_s));
  struct txr_ffi_type *tft = ffi_type_struct_checked(self, type);

  if (!sfd) {
    uw_throwf(socket_error_s, lit("~a: cannot set option on ~s"),
              self, sock, nao);
  } else {
    socklen_t size = convert(socklen_t, ffi_type_size(tft));
    mem_t *data = coerce(mem_t *, zalloca(size));
    ffi_type_put(tft, value, data, self);
    if (setsockopt(c_num(sfd, self), lvl, opt, data, size) != 0)
      uw_ethrowf(socket_error_s, lit("~a failed on ~s: ~d/~s"),
                 self, sock, num(errno), errno_to_str(errno), nao);
    return value;
  }
}

static val sock_set_entries(val fun)
{
  val sname[] = {
    lit("sockaddr"), lit("sockaddr-in"), lit("sockaddr-in6"),
    lit("sockaddr-un"), lit("addrinfo"),
    nil
  };
  val vname[] = {
    lit("af-unspec"), lit("af-unix"), lit("af-inet"), lit("af-inet6"),
    lit("sock-stream"), lit("sock-dgram"),
    lit("inaddr-any"), lit("inaddr-loopback"),
    lit("in6addr-any"), lit("in6addr-loopback"),
    lit("sock-nonblock"), lit("sock-cloexec"),
    lit("ai-passive"), lit("ai-canonname"), lit("ai-numerichost"),
    lit("ai-v4mapped"), lit("ai-all"), lit("ai-addrconfig"),
    lit("ai-numericserv"), lit("sol-socket"), lit("ipproto-ip"),
    lit("ipproto-ipv6"), lit("ipproto-tcp"), lit("ipproto-udp"),
    lit("so-acceptconn"), lit("so-broadcast"), lit("so-debug"),
    lit("so-dontroute"), lit("so-error"), lit("so-keepalive"),
    lit("so-linger"), lit("so-oobinline"), lit("so-rcvbuf"),
    lit("so-rcvlowat"), lit("so-rcvtimeo"), lit("so-reuseaddr"),
    lit("so-sndbuf"), lit("so-sndlowat"), lit("so-sndtimeo"),
    lit("so-type"), lit("ipv6-join-group"), lit("ipv6-leave-group"),
    lit("ipv6-multicast-hops"), lit("ipv6-multicast-if"),
    lit("ipv6-multicast-loop"), lit("ipv6-unicast-hops"),
    lit("ipv6-v6only"), lit("tcp-nodelay"),
    nil
  };
  val name[] = {
    lit("getaddrinfo"),
    lit("str-inaddr"), lit("str-in6addr"),
    lit("str-inaddr-net"), lit("str-in6addr-net"),
    lit("inaddr-str"), lit("in6addr-str"),
    lit("shut-rd"), lit("shut-wr"), lit("shut-rdwr"),
    lit("open-socket"), lit("open-socket-pair"),
    lit("sock-bind"), lit("sock-connect"), lit("sock-listen"),
    lit("sock-accept"), lit("sock-shutdown"), lit("open-socket"),
    lit("open-socket-pair"), lit("sock-send-timeout"), lit("sock-recv-timeout"),
    lit("sock-opt"), lit("sock-set-opt"),
    lit("sockaddr-str"),
    nil
  };
  val name_noload[] = {
    lit("family"), lit("addr"), lit("port"), lit("flow-info"),
    lit("scope-id"), lit("prefix"), lit("path"), lit("flags"), lit("socktype"),
    lit("protocol"), lit("canonname"), lit("str-addr"), nil
  };
  autoload_set(al_struct, sname, fun);
  autoload_set(al_var, vname, fun);
  autoload_set(al_fun, name, fun);
  autoload_intern(name_noload);
  return nil;
}

static val sock_instantiate(void)
{
  sockaddr_in_s = intern(lit("sockaddr-in"), user_package);
  sockaddr_in6_s = intern(lit("sockaddr-in6"), user_package);
  sockaddr_un_s = intern(lit("sockaddr-un"), user_package);
  addrinfo_s = intern(lit("addrinfo"), user_package);
  flags_s = intern(lit("flags"), user_package);
  family_s = intern(lit("family"), user_package);
  socktype_s = intern(lit("socktype"), user_package);
  protocol_s = intern(lit("protocol"), user_package);
  addr_s = intern(lit("addr"), user_package);
  canonname_s = intern(lit("canonname"), user_package);
  port_s = intern(lit("port"), user_package);
  flow_info_s = intern(lit("flow-info"), user_package);
  scope_id_s = intern(lit("scope-id"), user_package);

#if HAVE_GETADDRINFO
  reg_fun(intern(lit("getaddrinfo"), user_package), func_n3o(getaddrinfo_wrap, 1));
#endif

  reg_varl(intern(lit("af-unspec"), user_package), num_fast(AF_UNSPEC));
  reg_varl(intern(lit("af-unix"), user_package), num_fast(AF_UNIX));
  reg_varl(intern(lit("af-inet"), user_package), num_fast(AF_INET));
  reg_varl(intern(lit("af-inet6"), user_package), num_fast(AF_INET6));
  reg_varl(intern(lit("sock-stream"), user_package), num_fast(SOCK_STREAM));
  reg_varl(intern(lit("sock-dgram"), user_package), num_fast(SOCK_DGRAM));
  reg_varl(intern(lit("inaddr-any"), user_package), zero);
  reg_varl(intern(lit("inaddr-loopback"), user_package), num(0x7F000001));
  reg_varl(intern(lit("in6addr-any"), user_package), zero);
  reg_varl(intern(lit("in6addr-loopback"), user_package), one);
#ifdef SOCK_NONBLOCK
  reg_varl(intern(lit("sock-nonblock"), user_package), num_fast(SOCK_NONBLOCK));
#endif
#ifdef SOCK_CLOEXEC
  reg_varl(intern(lit("sock-cloexec"), user_package), num_fast(SOCK_CLOEXEC));
#endif
#if HAVE_GETADDRINFO
  reg_varl(intern(lit("ai-passive"), user_package), num_fast(AI_PASSIVE));
  reg_varl(intern(lit("ai-canonname"), user_package), num_fast(AI_CANONNAME));
  reg_varl(intern(lit("ai-numerichost"), user_package), num_fast(AI_NUMERICHOST));
#ifdef AI_V4MAPPED
  reg_varl(intern(lit("ai-v4mapped"), user_package), num_fast(AI_V4MAPPED));
#endif
#ifdef AI_ALL
  reg_varl(intern(lit("ai-all"), user_package), num_fast(AI_ALL));
#endif
  reg_varl(intern(lit("ai-addrconfig"), user_package), num_fast(AI_ADDRCONFIG));
  reg_varl(intern(lit("ai-numericserv"), user_package), num_fast(AI_NUMERICSERV));
#endif
  reg_varl(intern(lit("shut-rd"), user_package), num_fast(SHUT_RD));
  reg_varl(intern(lit("shut-wr"), user_package), num_fast(SHUT_WR));
  reg_varl(intern(lit("shut-rdwr"), user_package), num_fast(SHUT_RDWR));

  reg_fun(intern(lit("sock-bind"), user_package), func_n2(sock_bind));
  reg_fun(intern(lit("sock-connect"), user_package), func_n3o(sock_connect, 2));
  reg_fun(intern(lit("sock-listen"), user_package), func_n2o(sock_listen, 1));
  reg_fun(intern(lit("sock-accept"), user_package), func_n3o(sock_accept, 1));
  reg_fun(intern(lit("sock-shutdown"), user_package), func_n2o(sock_shutdown, 1));
  reg_fun(intern(lit("open-socket"), user_package), func_n3o(open_socket, 2));
  reg_fun(intern(lit("open-socket-pair"), user_package), func_n3o(socketpair_wrap, 2));
#if HAVE_SYS_TIME && defined SO_SNDTIMEO && defined SO_RCVTIMEO
  reg_fun(intern(lit("sock-send-timeout"), user_package), func_n2(sock_send_timeout));
  reg_fun(intern(lit("sock-recv-timeout"), user_package), func_n2(sock_recv_timeout));
#endif
  reg_fun(intern(lit("sock-opt"), user_package), func_n4o(sock_opt, 3));
  reg_fun(intern(lit("sock-set-opt"), user_package), func_n5o(sock_set_opt, 4));
  reg_varl(intern(lit("sol-socket"), user_package), num_fast(SOL_SOCKET));
  reg_varl(intern(lit("ipproto-ip"), user_package), num_fast(IPPROTO_IP));
  reg_varl(intern(lit("ipproto-ipv6"), user_package), num_fast(IPPROTO_IPV6));
  reg_varl(intern(lit("ipproto-tcp"), user_package), num_fast(IPPROTO_TCP));
  reg_varl(intern(lit("ipproto-udp"), user_package), num_fast(IPPROTO_UDP));
  reg_varl(intern(lit("so-acceptconn"), user_package), num_fast(SO_ACCEPTCONN));
  reg_varl(intern(lit("so-broadcast"), user_package), num_fast(SO_BROADCAST));
  reg_varl(intern(lit("so-debug"), user_package), num_fast(SO_DEBUG));
  reg_varl(intern(lit("so-dontroute"), user_package), num_fast(SO_DONTROUTE));
  reg_varl(intern(lit("so-error"), user_package), num_fast(SO_ERROR));
  reg_varl(intern(lit("so-keepalive"), user_package), num_fast(SO_KEEPALIVE));
  reg_varl(intern(lit("so-linger"), user_package), num_fast(SO_LINGER));
  reg_varl(intern(lit("so-oobinline"), user_package), num_fast(SO_OOBINLINE));
  reg_varl(intern(lit("so-rcvbuf"), user_package), num_fast(SO_RCVBUF));
  reg_varl(intern(lit("so-rcvlowat"), user_package), num_fast(SO_RCVLOWAT));
  reg_varl(intern(lit("so-rcvtimeo"), user_package), num_fast(SO_RCVTIMEO));
  reg_varl(intern(lit("so-reuseaddr"), user_package), num_fast(SO_REUSEADDR));
  reg_varl(intern(lit("so-sndbuf"), user_package), num_fast(SO_SNDBUF));
  reg_varl(intern(lit("so-sndlowat"), user_package), num_fast(SO_SNDLOWAT));
  reg_varl(intern(lit("so-sndtimeo"), user_package), num_fast(SO_SNDTIMEO));
  reg_varl(intern(lit("so-type"), user_package), num_fast(SO_TYPE));
  reg_varl(intern(lit("ipv6-join-group"), user_package), num_fast(IPV6_JOIN_GROUP));
  reg_varl(intern(lit("ipv6-leave-group"), user_package), num_fast(IPV6_LEAVE_GROUP));
  reg_varl(intern(lit("ipv6-multicast-hops"), user_package), num_fast(IPV6_MULTICAST_HOPS));
  reg_varl(intern(lit("ipv6-multicast-if"), user_package), num_fast(IPV6_MULTICAST_IF));
  reg_varl(intern(lit("ipv6-multicast-loop"), user_package), num_fast(IPV6_MULTICAST_LOOP));
  reg_varl(intern(lit("ipv6-unicast-hops"), user_package), num_fast(IPV6_UNICAST_HOPS));
  reg_varl(intern(lit("ipv6-v6only"), user_package), num_fast(IPV6_V6ONLY));
  reg_varl(intern(lit("tcp-nodelay"), user_package), num_fast(TCP_NODELAY));

  fill_stream_ops(&dgram_strm_ops);

  dgram_strm_ops.get_sock_family = dgram_get_sock_family;
  dgram_strm_ops.get_sock_type = dgram_get_sock_type;
  dgram_strm_ops.get_sock_peer = dgram_get_sock_peer;
  dgram_strm_ops.set_sock_peer = dgram_set_sock_peer;

  load(scat2(stdlib_path, lit("socket")));
  return nil;
}

void sock_init(void)
{
  ffi_typedef(intern(lit("socklen-t"), user_package),
              ffi_type_by_size(convert(socklen_t, -1) > 0, sizeof (socklen_t)));
  autoload_reg(sock_instantiate, sock_set_entries);
}
