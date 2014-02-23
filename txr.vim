" VIM Syntax file for txr
" Kaz Kylheku <kaz@kylheku.com>

" INSTALL-HOWTO:
"
" 1. Create the directory .vim/syntax in your home directory and
"    put this file there.
" 2. In your .vimrc, add this command to associate *.txr files
"    with the txr filetype.
"    :au BufRead,BufNewFile *.txr set filetype=txr | set lisp
"
" If you want syntax highlighting to be on automatically (for any language)
" you need to add ":syntax on" in your .vimrc also. But you knew that already!
"
" This file is generated by the genvim.txr script in the TXR source tree.

syn case match
syn spell toplevel

setlocal iskeyword=a-z,A-Z,48-57,!,$,&,*,+,-,<,=,>,?,\\,_,~,^

syn keyword txr_keyword contained accept all and bind
syn keyword txr_keyword contained block cases cat catch
syn keyword txr_keyword contained choose close coll collect
syn keyword txr_keyword contained defex deffilter define do
syn keyword txr_keyword contained end eof eol fail
syn keyword txr_keyword contained filter finally flatten forget
syn keyword txr_keyword contained freeform fuzz gather last
syn keyword txr_keyword contained load local maybe merge
syn keyword txr_keyword contained next none or output
syn keyword txr_keyword contained rebind rep repeat require
syn keyword txr_keyword contained set skip some text
syn keyword txr_keyword contained throw trailer try until
syn keyword txr_keyword contained var

syn keyword txl_keyword contained * *args* *full-args* *gensym-counter*
syn keyword txl_keyword contained *keyword-package* *random-state* *self-path* *stddebug*
syn keyword txl_keyword contained *stderr* *stdin* *stdlog* *stdnull*
syn keyword txl_keyword contained *stdout* *system-package* *user-package* +
syn keyword txl_keyword contained - / /= <
syn keyword txl_keyword contained <= = > >=
syn keyword txl_keyword contained abs acons acons-new aconsql-new
syn keyword txl_keyword contained acos alist-nremove alist-remove all
syn keyword txl_keyword contained and andf append append*
syn keyword txl_keyword contained append-each append-each* apply ash
syn keyword txl_keyword contained asin assoc assql atan
syn keyword txl_keyword contained atan2 atom bignump block
syn keyword txl_keyword contained boundp break-str call car
syn keyword txl_keyword contained cat-str cat-vec catch cdr
syn keyword txl_keyword contained ceil chain chr-isalnum chr-isalpha
syn keyword txl_keyword contained chr-isascii chr-iscntrl chr-isdigit chr-isgraph
syn keyword txl_keyword contained chr-islower chr-isprint chr-ispunct chr-isspace
syn keyword txl_keyword contained chr-isupper chr-isxdigit chr-num chr-str
syn keyword txl_keyword contained chr-str-set chr-tolower chr-toupper chrp
syn keyword txl_keyword contained close-stream closelog collect-each collect-each*
syn keyword txl_keyword contained comb compl-span-str cond cons
syn keyword txl_keyword contained conses conses* consp copy-alist
syn keyword txl_keyword contained copy-cons copy-hash copy-list copy-str
syn keyword txl_keyword contained copy-vec cos count-if countq
syn keyword txl_keyword contained countql countqual cum-norm-dist daemon
syn keyword txl_keyword contained dec defmacro defun defvar
syn keyword txl_keyword contained del delete-package do dohash
syn keyword txl_keyword contained downcase-str dwim each each*
syn keyword txl_keyword contained env env-hash eq eql
syn keyword txl_keyword contained equal errno error eval
syn keyword txl_keyword contained evenp exit exp expand
syn keyword txl_keyword contained expt exptmod fboundp fifth
syn keyword txl_keyword contained find find-if find-package first
syn keyword txl_keyword contained fixnump flatten flatten* flip
syn keyword txl_keyword contained flo-int flo-str floatp floor
syn keyword txl_keyword contained flush-stream for for* force
syn keyword txl_keyword contained format fourth fun func-get-env
syn keyword txl_keyword contained func-get-form func-set-env functionp gcd
syn keyword txl_keyword contained generate gensym get-byte get-char
syn keyword txl_keyword contained get-hash-userdata get-line get-list-from-stream get-sig-handler
syn keyword txl_keyword contained get-string-from-stream gethash group-by hash
syn keyword txl_keyword contained hash-alist hash-construct hash-count hash-diff
syn keyword txl_keyword contained hash-eql hash-equal hash-isec hash-keys
syn keyword txl_keyword contained hash-pairs hash-uni hash-update hash-update-1
syn keyword txl_keyword contained hash-values hashp identity if
syn keyword txl_keyword contained iff iffi inc inhash
syn keyword txl_keyword contained int-flo int-str integerp intern
syn keyword txl_keyword contained interp-fun-p isqrt keep-if keep-if*
syn keyword txl_keyword contained keywordp lambda lazy-str lazy-str-force
syn keyword txl_keyword contained lazy-str-force-upto lazy-str-get-trailing-list lazy-stream-cons lazy-stringp
syn keyword txl_keyword contained lcons-fun ldiff length length-list
syn keyword txl_keyword contained length-str length-str-< length-str-<= length-str->
syn keyword txl_keyword contained length-str->= length-vec let let*
syn keyword txl_keyword contained lisp-parse list list* list-str
syn keyword txl_keyword contained list-vector listp log log-alert
syn keyword txl_keyword contained log-auth log-authpriv log-cons log-crit
syn keyword txl_keyword contained log-daemon log-debug log-emerg log-err
syn keyword txl_keyword contained log-info log-ndelay log-notice log-nowait
syn keyword txl_keyword contained log-odelay log-perror log-pid log-user
syn keyword txl_keyword contained log-warning logand logior lognot
syn keyword txl_keyword contained logtest logtrunc logxor macro-form-p
syn keyword txl_keyword contained macro-time macroexpand macroexpand-1 macrolet
syn keyword txl_keyword contained make-catenated-stream make-hash make-lazy-cons make-package
syn keyword txl_keyword contained make-random-state make-similar-hash make-string-byte-input-stream make-string-input-stream
syn keyword txl_keyword contained make-string-output-stream make-strlist-output-stream make-sym make-time
syn keyword txl_keyword contained make-time-utc mapcar mapcar* maphash
syn keyword txl_keyword contained mappend mappend* mask match-fun
syn keyword txl_keyword contained match-regex match-regex-right match-str match-str-tree
syn keyword txl_keyword contained max memq memql memqual
syn keyword txl_keyword contained merge min mkstring mod
syn keyword txl_keyword contained multi-sort n-choose-k n-perm-k none
syn keyword txl_keyword contained not nreverse null num-chr
syn keyword txl_keyword contained num-str numberp oddp op
syn keyword txl_keyword contained open-command open-directory open-file open-pipe
syn keyword txl_keyword contained open-process open-tail openlog or
syn keyword txl_keyword contained orf packagep perm pop
syn keyword txl_keyword contained pos pos-if posq posql
syn keyword txl_keyword contained posqual pprinl pprint prinl
syn keyword txl_keyword contained print prog1 progn prop
syn keyword txl_keyword contained proper-listp push pushhash put-byte
syn keyword txl_keyword contained put-char put-line put-string qquote
syn keyword txl_keyword contained quasi quote rand random
syn keyword txl_keyword contained random-fixnum random-state-p range range*
syn keyword txl_keyword contained rcomb read real-time-stream-p reduce-left
syn keyword txl_keyword contained reduce-right ref refset regex-compile
syn keyword txl_keyword contained regex-parse regexp regsub rehome-sym
syn keyword txl_keyword contained remhash remove-if remove-if* remove-path
syn keyword txl_keyword contained remq remq* remql remql*
syn keyword txl_keyword contained remqual remqual* rename-path repeat
syn keyword txl_keyword contained replace replace-list replace-str replace-vec
syn keyword txl_keyword contained rest return return-from reverse
syn keyword txl_keyword contained rlcp rperm rplaca rplacd
syn keyword txl_keyword contained s-ifblk s-ifchr s-ifdir s-ififo
syn keyword txl_keyword contained s-iflnk s-ifmt s-ifreg s-irgrp
syn keyword txl_keyword contained s-iroth s-irusr s-irwxg s-irwxo
syn keyword txl_keyword contained s-irwxu s-isgid s-isuid s-isvtx
syn keyword txl_keyword contained s-iwgrp s-iwoth s-iwusr s-ixgrp
syn keyword txl_keyword contained s-ixoth s-ixusr search-regex search-str
syn keyword txl_keyword contained search-str-tree second seek-stream set
syn keyword txl_keyword contained set-diff set-hash-userdata set-sig-handler sethash
syn keyword txl_keyword contained setlogmask sig-abrt sig-alrm sig-bus
syn keyword txl_keyword contained sig-check sig-chld sig-cont sig-fpe
syn keyword txl_keyword contained sig-hup sig-ill sig-int sig-io
syn keyword txl_keyword contained sig-iot sig-kill sig-lost sig-pipe
syn keyword txl_keyword contained sig-poll sig-prof sig-pwr sig-quit
syn keyword txl_keyword contained sig-segv sig-stkflt sig-stop sig-sys
syn keyword txl_keyword contained sig-term sig-trap sig-tstp sig-ttin
syn keyword txl_keyword contained sig-ttou sig-urg sig-usr1 sig-usr2
syn keyword txl_keyword contained sig-vtalrm sig-winch sig-xcpu sig-xfsz
syn keyword txl_keyword contained sin sixth size-vec some
syn keyword txl_keyword contained sort source-loc source-loc-str span-str
syn keyword txl_keyword contained splice split-str split-str-set sqrt
syn keyword txl_keyword contained stat stream-get-prop stream-set-prop streamp
syn keyword txl_keyword contained string-cmp string-extend string-lt stringp
syn keyword txl_keyword contained sub sub-list sub-str sub-vec
syn keyword txl_keyword contained symbol-function symbol-name symbol-package symbol-value
syn keyword txl_keyword contained symbolp syslog tan third
syn keyword txl_keyword contained throw throwf time time-fields-local
syn keyword txl_keyword contained time-fields-utc time-string-local time-string-utc time-usec
syn keyword txl_keyword contained tok-str tostring tostringp tree-bind
syn keyword txl_keyword contained tree-case tree-find trim-str trunc
syn keyword txl_keyword contained typeof unget-byte unget-char unquote
syn keyword txl_keyword contained upcase-str update url-decode url-encode
syn keyword txl_keyword contained usleep uw-protect vec vec-push
syn keyword txl_keyword contained vec-set-length vecref vector vector-list
syn keyword txl_keyword contained vectorp with-saved-vars zerop

syn match txr_error "@[\t ]*[*]\?[\t ]*."
syn match txr_nested_error "[^\t `]\+" contained
syn match txr_hashbang "^#!.*"
syn match txr_atat "@[ \t]*@"
syn match txr_comment "@[ \t]*[#;].*"
syn match txr_contin "@[ \t]*\\$"
syn match txr_char "@[ \t]*\\."
syn match txr_char "@[ \t]*\\x[0-9A-Fa-f]\+"
syn match txr_char "@[ \t]*\\[0-9]\+"
syn match txr_variable "@[ \t]*[*]\?[ \t]*[A-Za-z_][A-Za-z0-9_]*"
syn match txr_metanum "@[0-9]\+"
syn match txr_regdir "@[ \t]*/\(\\/\|[^/]\|\\\n\)*/"

syn match txr_chr "#\\x[A-Fa-f0-9]\+" contained
syn match txr_chr "#\\o[0-9]\+" contained
syn match txr_chr "#\\[^ \t\nA-Za-z0-9_]" contained
syn match txr_chr "#\\[A-Za-z0-9_]\+" contained
syn match txr_regex "/\(\\/\|[^/]\|\\\n\)*/" contained
syn match txl_regex "#/\(\\/\|[^/]\|\\\n\)*/" contained
syn match txr_ncomment ";.*" contained

syn match txr_dot "\." contained
syn match txr_num "#x[+\-]\?[0-9A-Fa-f]\+" contained
syn match txr_num "[+\-]\?[0-9]\+" contained
syn match txr_ident "[:@]\?[A-Za-z0-9!$%&*+\-<=>?\\^_~]*[A-Za-z!$%&*+\-<=>?\\^_~][A-Za-z0-9!$%&*+\-<=>?\\^_~]*" contained
syn match txl_ident "[:@]\?[A-Za-z0-9!$%&*+\-<=>?\\^_~/]*[A-Za-z!$%&*+\-<=>?\\^_~/][A-Za-z0-9!$%&*+\-<=>?\\^_~/]*" contained
syn match txr_num "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?" contained
syn match txr_num "[+\-]\?[0-9]\+\([eE][+\-]\?[0-9]\+\)" contained
syn match txl_ident ":" contained

syn match txr_unquote "," contained
syn match txr_splice ",\*" contained
syn match txr_quote "'" contained
syn match txr_dotdot "\.\." contained
syn match txr_metaat "@" contained

syn region txr_bracevar matchgroup=Delimiter start="@[ \t]*[*]\?{" matchgroup=Delimiter end="}" contains=txr_num,txr_ident,txr_string,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_regex,txr_quasilit,txr_chr,txr_nested_error

syn region txr_directive matchgroup=Delimiter start="@[ \t]*(" matchgroup=Delimiter end=")" contains=txr_keyword,txr_string,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_num,txl_ident,txl_regex,txr_string,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error

syn region txr_list contained matchgroup=Delimiter start="#\?H\?(" matchgroup=Delimiter end=")" contains=txl_keyword,txr_string,txl_regex,txr_num,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error

syn region txr_bracket contained matchgroup=Delimiter start="\[" matchgroup=Delimiter end="\]" contains=txl_keyword,txr_string,txl_regex,txr_num,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error

syn region txr_mlist contained matchgroup=Delimiter start="@[ \t]*(" matchgroup=Delimiter end=")" contains=txl_keyword,txr_string,txl_regex,txr_num,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error

syn region txr_mbracket matchgroup=Delimiter start="@[ \t]*\[" matchgroup=Delimiter end="\]" contains=txl_keyword,txr_string,txl_regex,txr_num,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error

syn region txr_string contained oneline start=+"+ skip=+\\\\\|\\"+ end=+"+
syn region txr_quasilit contained oneline start=+`+ skip=+\\\\\|\\`+ end=+`+ contains=txr_variable,txr_metanum,txr_bracevar,txr_mlist,txr_mbracket

hi def link txr_at Special
hi def link txr_atstar Special
hi def link txr_atat Special
hi def link txr_comment Comment
hi def link txr_ncomment Comment
hi def link txr_hashbang Preproc
hi def link txr_contin Preproc
hi def link txr_char String
hi def link txr_keyword Keyword
hi def link txl_keyword Type
hi def link txr_string String
hi def link txr_chr String
hi def link txr_quasilit String
hi def link txr_regex String
hi def link txl_regex String
hi def link txr_regdir String
hi def link txr_variable Identifier
hi def link txr_metanum Identifier
hi def link txr_ident Identifier
hi def link txl_ident Identifier
hi def link txr_num Number
hi def link txr_quote Special
hi def link txr_unquote Special
hi def link txr_splice Special
hi def link txr_dot Special
hi def link txr_dotdot Special
hi def link txr_metaat Special
hi def link txr_error Error
hi def link txr_nested_error Error

let b:current_syntax = "lisp"
