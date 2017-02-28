" VIM Syntax file for txr
" Kaz Kylheku <kaz@kylheku.com>

" INSTALL-HOWTO:
"
" 1. Create the directory .vim/syntax in your home directory and
"    put the files txr.vim and tl.vim into this directory.
" 2. In your .vimrc, add this command to associate *.txr and *.tl
"    files with the txr and tl filetypes:
"    :au BufRead,BufNewFile *.txr set filetype=txr | set lisp
"    :au BufRead,BufNewFile *.tl set filetype=tl | set lisp
"
" If you want syntax highlighting to be on automatically (for any language)
" you need to add ":syntax on" in your .vimrc also. But you knew that already!
"
" This file is generated by the genvim.txr script in the TXR source tree.

syn case match
syn spell toplevel

setlocal iskeyword=a-z,A-Z,48-57,!,$,&,*,+,-,:,<,=,>,?,\\,_,~,/

syn keyword tl_keyword contained %e% %pi% * *args*
syn keyword tl_keyword contained *args-full* *e* *flo-dig* *flo-epsilon*
syn keyword tl_keyword contained *flo-max* *flo-min* *full-args* *gensym-counter*
syn keyword tl_keyword contained *lib-version* *listener-hist-len* *listener-multi-line-p* *listener-sel-inclusive-p*
syn keyword tl_keyword contained *load-path* *package* *param-macro* *pi*
syn keyword tl_keyword contained *place-clobber-expander* *place-delete-expander* *place-macro* *place-update-expander*
syn keyword tl_keyword contained *print-base* *print-circle* *print-flo-digits* *print-flo-format*
syn keyword tl_keyword contained *print-flo-precision* *random-state* *random-warmup* *self-path*
syn keyword tl_keyword contained *stddebug* *stderr* *stdin* *stdlog*
syn keyword tl_keyword contained *stdnull* *stdout* *trace-output* *txr-version*
syn keyword tl_keyword contained *unhandled-hook* + - /
syn keyword tl_keyword contained /= : :abandoned :addr
syn keyword tl_keyword contained :apf :append :args :atime
syn keyword tl_keyword contained :auto :awk-file :awk-rec :begin
syn keyword tl_keyword contained :begin-file :blksize :blocks :bool
syn keyword tl_keyword contained :byte-oriented :cdigit :chars :cint
syn keyword tl_keyword contained :close :continue :counter :cspace
syn keyword tl_keyword contained :ctime :cword-char :dec :decline
syn keyword tl_keyword contained :dev :digit :downcase :end
syn keyword tl_keyword contained :end-file :env :equal-based :explicit-no
syn keyword tl_keyword contained :fallback :fd :filter :fini
syn keyword tl_keyword contained :finish :float :form :from-current
syn keyword tl_keyword contained :from-end :from-start :from_html :frombase64
syn keyword tl_keyword contained :fromhtml :frompercent :fromurl :fun
syn keyword tl_keyword contained :function :gap :gid :greedy
syn keyword tl_keyword contained :hex :hextoint :inf :init
syn keyword tl_keyword contained :ino :inp :inputs :instance
syn keyword tl_keyword contained :into :key :let :lfilt
syn keyword tl_keyword contained :lines :list :local :longest
syn keyword tl_keyword contained :mandatory :maxgap :maxtimes :method
syn keyword tl_keyword contained :mingap :mintimes :mode :mtime
syn keyword tl_keyword contained :name :named :next-spec :nlink
syn keyword tl_keyword contained :nothrow :oct :outf :outp
syn keyword tl_keyword contained :output :postinit :prio :rdev
syn keyword tl_keyword contained :real-time :reflect :repeat-spec :resolve
syn keyword tl_keyword contained :rfilt :set :set-file :shortest
syn keyword tl_keyword contained :size :space :static :str
syn keyword tl_keyword contained :string :symacro :times :tlist
syn keyword tl_keyword contained :to_html :tobase64 :tofloat :tohtml
syn keyword tl_keyword contained :tohtml* :toint :tonumber :topercent
syn keyword tl_keyword contained :tourl :uid :upcase :use
syn keyword tl_keyword contained :use-from :use-syms :userdata :var
syn keyword tl_keyword contained :vars :weak-keys :weak-vals :whole
syn keyword tl_keyword contained :word-char :wrap < <=
syn keyword tl_keyword contained = > >= abort
syn keyword tl_keyword contained abs abs-path-p acons acons-new
syn keyword tl_keyword contained aconsql-new acos ado af-inet
syn keyword tl_keyword contained af-inet6 af-unix af-unspec ai-addrconfig
syn keyword tl_keyword contained ai-all ai-canonname ai-numerichost ai-numericserv
syn keyword tl_keyword contained ai-passive ai-v4mapped alet alist-nremove
syn keyword tl_keyword contained alist-remove all and andf
syn keyword tl_keyword contained ap apf append append*
syn keyword tl_keyword contained append-each append-each* apply aret
syn keyword tl_keyword contained ash asin assoc assql
syn keyword tl_keyword contained at-exit-call at-exit-do-not-call atan atan2
syn keyword tl_keyword contained atom awk base64-decode base64-encode
syn keyword tl_keyword contained bignump bindable bit block
syn keyword tl_keyword contained block* boundp break-str brkint
syn keyword tl_keyword contained bs0 bs1 bsdly build
syn keyword tl_keyword contained build-list butlast butlastn caaaaar
syn keyword tl_keyword contained caaaadr caaaar caaadar caaaddr
syn keyword tl_keyword contained caaadr caaar caadaar caadadr
syn keyword tl_keyword contained caadar caaddar caadddr caaddr
syn keyword tl_keyword contained caadr caar cadaaar cadaadr
syn keyword tl_keyword contained cadaar cadadar cadaddr cadadr
syn keyword tl_keyword contained cadar caddaar caddadr caddar
syn keyword tl_keyword contained cadddar caddddr cadddr caddr
syn keyword tl_keyword contained cadr call call-clobber-expander call-delete-expander
syn keyword tl_keyword contained call-finalizers call-super-fun call-super-method call-update-expander
syn keyword tl_keyword contained callf car caseq caseq*
syn keyword tl_keyword contained caseql caseql* casequal casequal*
syn keyword tl_keyword contained cat-str cat-streams cat-vec catch
syn keyword tl_keyword contained catch* catenated-stream-p catenated-stream-push cbaud
syn keyword tl_keyword contained cbaudex cdaaaar cdaaadr cdaaar
syn keyword tl_keyword contained cdaadar cdaaddr cdaadr cdaar
syn keyword tl_keyword contained cdadaar cdadadr cdadar cdaddar
syn keyword tl_keyword contained cdadddr cdaddr cdadr cdar
syn keyword tl_keyword contained cddaaar cddaadr cddaar cddadar
syn keyword tl_keyword contained cddaddr cddadr cddar cdddaar
syn keyword tl_keyword contained cdddadr cdddar cddddar cdddddr
syn keyword tl_keyword contained cddddr cdddr cddr cdr
syn keyword tl_keyword contained ceil ceil-rem chain chand
syn keyword tl_keyword contained chdir chmod chr-digit chr-int
syn keyword tl_keyword contained chr-isalnum chr-isalpha chr-isascii chr-isblank
syn keyword tl_keyword contained chr-iscntrl chr-isdigit chr-isgraph chr-islower
syn keyword tl_keyword contained chr-isprint chr-ispunct chr-isspace chr-isunisp
syn keyword tl_keyword contained chr-isupper chr-isxdigit chr-num chr-str
syn keyword tl_keyword contained chr-str-set chr-tolower chr-toupper chr-xdigit
syn keyword tl_keyword contained chrp clamp clear-dirty clear-error
syn keyword tl_keyword contained clear-struct clearhash clocal close-stream
syn keyword tl_keyword contained closelog cmp-str cmspar collect-each
syn keyword tl_keyword contained collect-each* comb command-get command-get-lines
syn keyword tl_keyword contained command-get-string command-put command-put-lines command-put-string
syn keyword tl_keyword contained compare-swap compile-defr-warning compile-error compile-warning
syn keyword tl_keyword contained compl-span-str cond conda condlet
syn keyword tl_keyword contained cons conses conses* consp
syn keyword tl_keyword contained constantp copy copy-alist copy-cons
syn keyword tl_keyword contained copy-hash copy-list copy-str copy-struct
syn keyword tl_keyword contained copy-vec cos count-if countq
syn keyword tl_keyword contained countql countqual cr0 cr1
syn keyword tl_keyword contained cr2 cr3 crdly cread
syn keyword tl_keyword contained crtscts crypt cs5 cs6
syn keyword tl_keyword contained cs7 cs8 csize cstopb
syn keyword tl_keyword contained cum-norm-dist daemon dec defer-warning
syn keyword tl_keyword contained defex define-accessor define-modify-macro define-param-expander
syn keyword tl_keyword contained define-place-macro defmacro defmeth defpackage
syn keyword tl_keyword contained defparm defparml defplace defstruct
syn keyword tl_keyword contained defsymacro defun defvar defvarl
syn keyword tl_keyword contained del delay delete-package display-width
syn keyword tl_keyword contained do dohash dotimes downcase-str
syn keyword tl_keyword contained drop drop-until drop-while dump-deferred-warnings
syn keyword tl_keyword contained dup dupfd dwim each
syn keyword tl_keyword contained each* echo echoctl echoe
syn keyword tl_keyword contained echok echoke echonl echoprt
syn keyword tl_keyword contained eighth empty endgrent endp
syn keyword tl_keyword contained endpwent ensure-dir env env-fbind
syn keyword tl_keyword contained env-hash env-vbind eq eql
syn keyword tl_keyword contained equal equot errno error
syn keyword tl_keyword contained eval evenp exception-subtype-map exception-subtype-p
syn keyword tl_keyword contained exec exit exit* exp
syn keyword tl_keyword contained expand-left expand-right expt exptmod
syn keyword tl_keyword contained extproc f$ f^ f^$
syn keyword tl_keyword contained false fboundp ff0 ff1
syn keyword tl_keyword contained ffdly fifth file-append file-append-lines
syn keyword tl_keyword contained file-append-string file-get file-get-lines file-get-string
syn keyword tl_keyword contained file-put file-put-lines file-put-string fileno
syn keyword tl_keyword contained filter-equal filter-string-tree finalize find
syn keyword tl_keyword contained find-frame find-frames find-if find-max
syn keyword tl_keyword contained find-min find-package find-struct-type first
syn keyword tl_keyword contained fixnum-max fixnum-min fixnump flatcar
syn keyword tl_keyword contained flatcar* flatten flatten* flet
syn keyword tl_keyword contained flip flipargs flo-dig flo-epsilon
syn keyword tl_keyword contained flo-int flo-max flo-max-dig flo-min
syn keyword tl_keyword contained flo-str floatp floor floor-rem
syn keyword tl_keyword contained flush-stream flusho fmakunbound fmt
syn keyword tl_keyword contained fnm-casefold fnm-leading-dir fnm-noescape fnm-pathname
syn keyword tl_keyword contained fnm-period fnmatch for for*
syn keyword tl_keyword contained force fork format fourth
syn keyword tl_keyword contained fr$ fr^ fr^$ from
syn keyword tl_keyword contained frr fstat ftw ftw-actionretval
syn keyword tl_keyword contained ftw-chdir ftw-continue ftw-d ftw-depth
syn keyword tl_keyword contained ftw-dnr ftw-dp ftw-f ftw-mount
syn keyword tl_keyword contained ftw-ns ftw-phys ftw-skip-siblings ftw-skip-subtree
syn keyword tl_keyword contained ftw-sl ftw-sln ftw-stop fun
syn keyword tl_keyword contained func-get-env func-get-form func-get-name func-set-env
syn keyword tl_keyword contained functionp gcd gen generate
syn keyword tl_keyword contained gensym gequal get-byte get-char
syn keyword tl_keyword contained get-clobber-expander get-delete-expander get-error get-error-str
syn keyword tl_keyword contained get-frames get-hash-userdata get-indent get-indent-mode
syn keyword tl_keyword contained get-line get-lines get-list-from-stream get-sig-handler
syn keyword tl_keyword contained get-string get-string-from-stream get-update-expander getaddrinfo
syn keyword tl_keyword contained getegid getenv geteuid getgid
syn keyword tl_keyword contained getgrent getgrgid getgrnam getgroups
syn keyword tl_keyword contained gethash getitimer getopts getpid
syn keyword tl_keyword contained getppid getpwent getpwnam getpwuid
syn keyword tl_keyword contained getresgid getresuid getuid ginterate
syn keyword tl_keyword contained giterate glob glob-altdirfunc glob-brace
syn keyword tl_keyword contained glob-err glob-mark glob-nocheck glob-noescape
syn keyword tl_keyword contained glob-nomagic glob-nosort glob-onlydir glob-period
syn keyword tl_keyword contained glob-tilde glob-tilde-check go greater
syn keyword tl_keyword contained group-by group-reduce gun handle
syn keyword tl_keyword contained handle* handler-bind hash hash-alist
syn keyword tl_keyword contained hash-begin hash-construct hash-count hash-diff
syn keyword tl_keyword contained hash-eql hash-equal hash-from-pairs hash-isec
syn keyword tl_keyword contained hash-keys hash-list hash-next hash-pairs
syn keyword tl_keyword contained hash-proper-subset hash-revget hash-subset hash-uni
syn keyword tl_keyword contained hash-update hash-update-1 hash-userdata hash-values
syn keyword tl_keyword contained hashp have html-decode html-encode
syn keyword tl_keyword contained html-encode* hupcl iapply icanon
syn keyword tl_keyword contained icrnl identity ido iexten
syn keyword tl_keyword contained if ifa iff iffi
syn keyword tl_keyword contained iflet ignbrk igncr ignerr
syn keyword tl_keyword contained ignpar ignwarn imaxbel improper-plist-to-alist
syn keyword tl_keyword contained in in-package in6addr-any in6addr-loopback
syn keyword tl_keyword contained inaddr-any inaddr-loopback inc inc-indent
syn keyword tl_keyword contained indent-code indent-data indent-off inhash
syn keyword tl_keyword contained inlcr inpck int-chr int-flo
syn keyword tl_keyword contained int-str integerp intern interp-fun-p
syn keyword tl_keyword contained interpose invoke-catch ip ipf
syn keyword tl_keyword contained iread isig isqrt istrip
syn keyword tl_keyword contained itimer-prov itimer-real itimer-virtual iuclc
syn keyword tl_keyword contained iutf8 ixany ixoff ixon
syn keyword tl_keyword contained juxt keep-if keep-if* keepq
syn keyword tl_keyword contained keepql keepqual keyword-package keywordp
syn keyword tl_keyword contained kill labels lambda last
syn keyword tl_keyword contained lazy-str lazy-str-force lazy-str-force-upto lazy-str-get-trailing-list
syn keyword tl_keyword contained lazy-stream-cons lazy-stringp lcm lcons
syn keyword tl_keyword contained lcons-fun lconsp ldiff length
syn keyword tl_keyword contained length-list length-str length-str-< length-str-<=
syn keyword tl_keyword contained length-str-> length-str->= length-vec lequal
syn keyword tl_keyword contained less let let* lexical-fun-p
syn keyword tl_keyword contained lexical-lisp1-binding lexical-var-p lib-version link
syn keyword tl_keyword contained lisp-parse list list* list-str
syn keyword tl_keyword contained list-vec list-vector listp lnew
syn keyword tl_keyword contained load log log-alert log-auth
syn keyword tl_keyword contained log-authpriv log-cons log-crit log-daemon
syn keyword tl_keyword contained log-debug log-emerg log-err log-info
syn keyword tl_keyword contained log-ndelay log-notice log-nowait log-odelay
syn keyword tl_keyword contained log-perror log-pid log-user log-warning
syn keyword tl_keyword contained log10 log2 logand logior
syn keyword tl_keyword contained lognot logtest logtrunc logxor
syn keyword tl_keyword contained lset lstat m$ m^
syn keyword tl_keyword contained m^$ mac-param-bind macro-ancestor macro-form-p
syn keyword tl_keyword contained macro-time macroexpand macroexpand-1 macrolet
syn keyword tl_keyword contained major make-catenated-stream make-env make-hash
syn keyword tl_keyword contained make-lazy-cons make-lazy-struct make-like make-package
syn keyword tl_keyword contained make-random-state make-similar-hash make-string-byte-input-stream make-string-input-stream
syn keyword tl_keyword contained make-string-output-stream make-strlist-input-stream make-strlist-output-stream make-struct
syn keyword tl_keyword contained make-struct-type make-sym make-time make-time-utc
syn keyword tl_keyword contained make-trie makedev makunbound mapcar
syn keyword tl_keyword contained mapcar* mapdo mapf maphash
syn keyword tl_keyword contained mappend mappend* mask match-fun
syn keyword tl_keyword contained match-regex match-regex-right match-regst match-regst-right
syn keyword tl_keyword contained match-str match-str-tree max mboundp
syn keyword tl_keyword contained member member-if memp memq
syn keyword tl_keyword contained memql memqual merge meth
syn keyword tl_keyword contained method min minor minusp
syn keyword tl_keyword contained mismatch mkdir mknod mkstring
syn keyword tl_keyword contained mlet mmakunbound mod multi
syn keyword tl_keyword contained multi-sort n-choose-k n-perm-k nconc
syn keyword tl_keyword contained neq neql nequal new
syn keyword tl_keyword contained nexpand-left nil nilf ninth
syn keyword tl_keyword contained nl0 nl1 nldly noflsh
syn keyword tl_keyword contained none not notf nreconc
syn keyword tl_keyword contained nreverse nthcdr nthlast null
syn keyword tl_keyword contained nullify num-chr num-str numberp
syn keyword tl_keyword contained oand obtain obtain* obtain*-block
syn keyword tl_keyword contained obtain-block ocrnl oddp ofdel
syn keyword tl_keyword contained ofill olcuc onlcr onlret
syn keyword tl_keyword contained onocr op open-command open-directory
syn keyword tl_keyword contained open-file open-fileno open-files open-files*
syn keyword tl_keyword contained open-pipe open-process open-socket open-socket-pair
syn keyword tl_keyword contained open-tail openlog opip opost
syn keyword tl_keyword contained opt opthelp or orf
syn keyword tl_keyword contained package-alist package-fallback-list package-foreign-symbols package-local-symbols
syn keyword tl_keyword contained package-name package-symbols packagep pad
syn keyword tl_keyword contained parenb parmrk parodd partition
syn keyword tl_keyword contained partition* partition-by path-blkdev-p path-chrdev-p
syn keyword tl_keyword contained path-dir-p path-executable-to-me-p path-exists-p path-file-p
syn keyword tl_keyword contained path-mine-p path-my-group-p path-newer path-older
syn keyword tl_keyword contained path-pipe-p path-private-to-me-p path-read-writable-to-me-p path-readable-to-me-p
syn keyword tl_keyword contained path-same-object path-setgid-p path-setuid-p path-sock-p
syn keyword tl_keyword contained path-sticky-p path-strictly-private-to-me-p path-symlink-p path-writable-to-me-p
syn keyword tl_keyword contained pdec pendin perm pinc
syn keyword tl_keyword contained pipe place-form-p placelet placelet*
syn keyword tl_keyword contained plist-to-alist plusp poll poll-err
syn keyword tl_keyword contained poll-in poll-nval poll-out poll-pri
syn keyword tl_keyword contained poll-rdband poll-rdhup poll-wrband pop
syn keyword tl_keyword contained pos pos-if pos-max pos-min
syn keyword tl_keyword contained posq posql posqual pppred
syn keyword tl_keyword contained ppred pprinl pprint pprof
syn keyword tl_keyword contained pred prinl print prof
syn keyword tl_keyword contained prog prog* prog1 progn
syn keyword tl_keyword contained promisep prop proper-list-p proper-listp
syn keyword tl_keyword contained pset pure-rel-path-p purge-deferred-warning push
syn keyword tl_keyword contained pushhash pushnew put-byte put-char
syn keyword tl_keyword contained put-line put-lines put-string put-strings
syn keyword tl_keyword contained pwd qquote qref quote
syn keyword tl_keyword contained r$ r^ r^$ raise
syn keyword tl_keyword contained rand random random-fixnum random-state-get-vec
syn keyword tl_keyword contained random-state-p range range* range-regex
syn keyword tl_keyword contained rangep rassoc rassql rcomb
syn keyword tl_keyword contained rcons read read-until-match readlink
syn keyword tl_keyword contained real-time-stream-p record-adapter reduce-left reduce-right
syn keyword tl_keyword contained ref refset regex-compile regex-from-trie
syn keyword tl_keyword contained regex-parse regex-source regexp register-exception-subtypes
syn keyword tl_keyword contained register-tentative-def regsub rehome-sym release-deferred-warnings
syn keyword tl_keyword contained remhash remove-if remove-if* remove-path
syn keyword tl_keyword contained remq remq* remql remql*
syn keyword tl_keyword contained remqual remqual* rename-path repeat
syn keyword tl_keyword contained replace replace-list replace-str replace-struct
syn keyword tl_keyword contained replace-vec reset-struct rest ret
syn keyword tl_keyword contained retf return return* return-from
syn keyword tl_keyword contained revappend reverse rfind rfind-if
syn keyword tl_keyword contained rlcp rlcp-tree rlet rmember
syn keyword tl_keyword contained rmember-if rmemq rmemql rmemqual
syn keyword tl_keyword contained rotate round round-rem rperm
syn keyword tl_keyword contained rplaca rplacd rpos rpos-if
syn keyword tl_keyword contained rposq rposql rposqual rr
syn keyword tl_keyword contained rra rsearch rslot run
syn keyword tl_keyword contained s-ifblk s-ifchr s-ifdir s-ififo
syn keyword tl_keyword contained s-iflnk s-ifmt s-ifreg s-ifsock
syn keyword tl_keyword contained s-irgrp s-iroth s-irusr s-irwxg
syn keyword tl_keyword contained s-irwxo s-irwxu s-isgid s-isuid
syn keyword tl_keyword contained s-isvtx s-iwgrp s-iwoth s-iwusr
syn keyword tl_keyword contained s-ixgrp s-ixoth s-ixusr search
syn keyword tl_keyword contained search-regex search-regst search-str search-str-tree
syn keyword tl_keyword contained second seek-stream select self-load-path
syn keyword tl_keyword contained self-path seqp set set-diff
syn keyword tl_keyword contained set-hash-userdata set-indent set-indent-mode set-package-fallback-list
syn keyword tl_keyword contained set-sig-handler setegid setenv seteuid
syn keyword tl_keyword contained setgid setgrent setgroups sethash
syn keyword tl_keyword contained setitimer setlogmask setpwent setresgid
syn keyword tl_keyword contained setresuid setuid seventh sh
syn keyword tl_keyword contained shift shuffle shut-rd shut-rdwr
syn keyword tl_keyword contained shut-wr sig-abrt sig-alrm sig-bus
syn keyword tl_keyword contained sig-check sig-chld sig-cont sig-fpe
syn keyword tl_keyword contained sig-hup sig-ill sig-int sig-io
syn keyword tl_keyword contained sig-iot sig-kill sig-pipe sig-poll
syn keyword tl_keyword contained sig-prof sig-pwr sig-quit sig-segv
syn keyword tl_keyword contained sig-stkflt sig-stop sig-sys sig-term
syn keyword tl_keyword contained sig-trap sig-tstp sig-ttin sig-ttou
syn keyword tl_keyword contained sig-urg sig-usr1 sig-usr2 sig-vtalrm
syn keyword tl_keyword contained sig-winch sig-xcpu sig-xfsz sign-extend
syn keyword tl_keyword contained sin sixth size-vec slet
syn keyword tl_keyword contained slot slotp slots slotset
syn keyword tl_keyword contained sock-accept sock-bind sock-cloexec sock-connect
syn keyword tl_keyword contained sock-dgram sock-family sock-listen sock-nonblock
syn keyword tl_keyword contained sock-peer sock-recv-timeout sock-send-timeout sock-set-peer
syn keyword tl_keyword contained sock-shutdown sock-stream sock-type some
syn keyword tl_keyword contained sort sort-group source-loc source-loc-str
syn keyword tl_keyword contained span-str special-operator-p special-var-p splice
syn keyword tl_keyword contained split split* split-str split-str-set
syn keyword tl_keyword contained sqrt sssucc ssucc stat
syn keyword tl_keyword contained static-slot static-slot-ensure static-slot-p static-slot-set
syn keyword tl_keyword contained stdlib str-in6addr str-in6addr-net str-inaddr
syn keyword tl_keyword contained str-inaddr-net str< str<= str=
syn keyword tl_keyword contained str> str>= stream-get-prop stream-set-prop
syn keyword tl_keyword contained streamp string-extend string-lt stringp
syn keyword tl_keyword contained struct-type struct-type-p structp sub
syn keyword tl_keyword contained sub-list sub-str sub-vec subtypep
syn keyword tl_keyword contained succ super super-method suspend
syn keyword tl_keyword contained swap symacrolet symbol-function symbol-macro
syn keyword tl_keyword contained symbol-name symbol-package symbol-value symbolp
syn keyword tl_keyword contained symlink sys:*pl-env* sys:*trace-hash* sys:*trace-level*
syn keyword tl_keyword contained sys:abscond* sys:abscond-from sys:apply sys:awk-code-move-check
syn keyword tl_keyword contained sys:awk-expander sys:awk-fun-let sys:awk-fun-shadowing-env sys:awk-mac-let
syn keyword tl_keyword contained sys:awk-redir sys:awk-test sys:bad-slot-syntax sys:bits
syn keyword tl_keyword contained sys:build-key-list sys:capture-cont sys:catch sys:circref
syn keyword tl_keyword contained sys:compat sys:conv sys:conv-expand sys:conv-let
syn keyword tl_keyword contained sys:ctx-form sys:ctx-name sys:defmeth sys:do-conv
syn keyword tl_keyword contained sys:do-path-test sys:dvbind sys:dwim-del sys:dwim-set
syn keyword tl_keyword contained sys:each-op sys:eval-err sys:expand sys:expand-handle
syn keyword tl_keyword contained sys:expand-params sys:expand-with-free-refs sys:expr sys:extract-keys
syn keyword tl_keyword contained sys:extract-keys-p sys:fbind sys:for-op sys:gc
syn keyword tl_keyword contained sys:gc-set-delta sys:get-fun-getter-setter sys:get-mb sys:get-vb
syn keyword tl_keyword contained sys:handle-bad-syntax sys:if-to-cond sys:in6addr-condensed-text sys:l1-setq
syn keyword tl_keyword contained sys:l1-val sys:lbind sys:lisp1-setq sys:lisp1-value
syn keyword tl_keyword contained sys:list-builder-flets sys:loc sys:make-struct-lit sys:make-struct-type
syn keyword tl_keyword contained sys:mark-special sys:name-str sys:obtain-impl sys:opt-dash
syn keyword tl_keyword contained sys:opt-err sys:path-access sys:path-examine sys:path-test
syn keyword tl_keyword contained sys:path-test-mode sys:pl-expand sys:placelet-1 sys:propagate-ancestor
syn keyword tl_keyword contained sys:prune-missing-inits sys:qquote sys:quasi sys:quasilist
syn keyword tl_keyword contained sys:r-s-let-expander sys:reg-expand-nongreedy sys:reg-optimize sys:register-simple-accessor
syn keyword tl_keyword contained sys:rplaca sys:rplacd sys:rslotset sys:set-hash-rec-limit
syn keyword tl_keyword contained sys:set-hash-str-limit sys:set-macro-ancestor sys:setq sys:setqf
syn keyword tl_keyword contained sys:splice sys:str-inaddr-net-impl sys:struct-lit sys:switch
syn keyword tl_keyword contained sys:sym-clobber-expander sys:sym-delete-expander sys:sym-update-expander sys:top-fb
syn keyword tl_keyword contained sys:top-mb sys:top-vb sys:trace sys:trace-enter
syn keyword tl_keyword contained sys:trace-leave sys:unquote sys:untrace sys:var
syn keyword tl_keyword contained sys:wdwrap sys:with-dyn-rebinds syslog system-package
syn keyword tl_keyword contained t tab0 tab1 tab2
syn keyword tl_keyword contained tab3 tabdly tagbody take
syn keyword tl_keyword contained take-until take-while tan tb
syn keyword tl_keyword contained tc tcdrain tcflow tcflush
syn keyword tl_keyword contained tcgetattr tciflush tcioff tcioflush
syn keyword tl_keyword contained tcion tcoflush tcooff tcoon
syn keyword tl_keyword contained tcsadrain tcsaflush tcsanow tcsendbreak
syn keyword tl_keyword contained tcsetattr tentative-def-exists tenth test-clear
syn keyword tl_keyword contained test-clear-dirty test-dec test-dirty test-inc
syn keyword tl_keyword contained test-set test-set-indent-mode tf third
syn keyword tl_keyword contained throw throwf time time-fields-local
syn keyword tl_keyword contained time-fields-utc time-parse time-string-local time-string-utc
syn keyword tl_keyword contained time-struct-local time-struct-utc time-usec to
syn keyword tl_keyword contained tofloat tofloatz toint tointz
syn keyword tl_keyword contained tok-str tok-where tostop tostring
syn keyword tl_keyword contained tostringp tprint trace transpose
syn keyword tl_keyword contained tree-bind tree-case tree-find trie-add
syn keyword tl_keyword contained trie-compress trie-lookup-begin trie-lookup-feed-char trie-value-at
syn keyword tl_keyword contained trim-str true trunc trunc-rem
syn keyword tl_keyword contained truncate-stream tuples txr-case txr-case-impl
syn keyword tl_keyword contained txr-if txr-path txr-sym txr-version
syn keyword tl_keyword contained txr-when typecase typeof typep
syn keyword tl_keyword contained umask umeth umethod uname
syn keyword tl_keyword contained unget-byte unget-char unintern uniq
syn keyword tl_keyword contained unique unless unquote unsetenv
syn keyword tl_keyword contained until until* untrace unuse-package
syn keyword tl_keyword contained unuse-sym unwind-protect upcase-str upd
syn keyword tl_keyword contained update url-decode url-encode use
syn keyword tl_keyword contained use-package use-sym user-package usl
syn keyword tl_keyword contained usleep uslot vdiscard vec
syn keyword tl_keyword contained vec-list vec-push vec-set-length vecref
syn keyword tl_keyword contained vector vector-list vectorp veof
syn keyword tl_keyword contained veol veol2 verase vintr
syn keyword tl_keyword contained vkill vlnext vmin vquit
syn keyword tl_keyword contained vreprint vstart vstop vsusp
syn keyword tl_keyword contained vswtc vt0 vt1 vtdly
syn keyword tl_keyword contained vtime vwerase w-continued w-coredump
syn keyword tl_keyword contained w-exitstatus w-ifcontinued w-ifexited w-ifsignaled
syn keyword tl_keyword contained w-ifstopped w-nohang w-stopsig w-termsig
syn keyword tl_keyword contained w-untraced wait weave when
syn keyword tl_keyword contained whena whenlet where while
syn keyword tl_keyword contained while* whilet width width-check
syn keyword tl_keyword contained window-map window-mappend with-clobber-expander with-delete-expander
syn keyword tl_keyword contained with-gensyms with-hash-iter with-in-string-byte-stream with-in-string-stream
syn keyword tl_keyword contained with-objects with-out-string-stream with-out-strlist-stream with-resources
syn keyword tl_keyword contained with-slots with-stream with-update-expander wrap
syn keyword tl_keyword contained wrap* xcase yield yield-from
syn keyword tl_keyword contained zap zerop zip

syn keyword txr_keyword contained accept all and assert
syn keyword txr_keyword contained bind block call cases
syn keyword txr_keyword contained cat catch choose chr
syn keyword txr_keyword contained close coll collect data
syn keyword txr_keyword contained defex deffilter define do
syn keyword txr_keyword contained elif else empty end
syn keyword txr_keyword contained eof eol fail filter
syn keyword txr_keyword contained finally first flatten forget
syn keyword txr_keyword contained freeform fuzz gather if
syn keyword txr_keyword contained include last line load
syn keyword txr_keyword contained local maybe merge mod
syn keyword txr_keyword contained modlast name next none
syn keyword txr_keyword contained or output rebind rep
syn keyword txr_keyword contained repeat require set single
syn keyword txr_keyword contained skip some text throw
syn keyword txr_keyword contained trailer try until var
syn match txr_error "\(@[ \t]*\)[*]\?[\t ]*."
syn match txr_atat "\(@[ \t]*\)@"
syn match txr_comment "\(@[ \t]*\)[#;].*"
syn match txr_contin "\(@[ \t]*\)\\$"
syn match txr_char "\(@[ \t]*\)\\."
syn match txr_error "\(@[ \t]*\)\\[xo]"
syn match txr_char "\(@[ \t]*\)\\x[0-9A-Fa-f]\+;\?"
syn match txr_char "\(@[ \t]*\)\\[0-7]\+;\?"
syn match txr_regdir "\(@[ \t]*\)/\(\\/\|[^/]\|\\\n\)*/"
syn match txr_hashbang "^#!.*"
syn match txr_nested_error "[^\t ]\+" contained
syn match txr_variable "\(@[ \t]*\)[*]\?[ \t]*[A-Za-z_][A-Za-z_0-9]*"
syn match txr_splicevar "@[ \t,*@]*[A-Za-z_][A-Za-z_0-9]*" contained
syn match txr_metanum "@\+[0-9]\+" contained
syn match txr_badesc "\\." contained
syn match txr_escat "\\@" contained
syn match txr_stresc "\\[abtnvfre\\ \n"`']" contained
syn match txr_numesc "\\x[0-9A-Fa-f]\+;\?" contained
syn match txr_numesc "\\[0-7]\+;\?" contained
syn match txr_regesc "\\[abtnvfre\\ \n/sSdDwW()\|.*?+~&%\[\]\-]" contained

syn match txr_chr "#\\x[0-9A-Fa-f]\+" contained
syn match txr_chr "#\\o[0-7]\+" contained
syn match txr_chr "#\\[^ \t\nA-Za-z_0-9]" contained
syn match txr_chr "#\\[A-Za-z_0-9]\+" contained
syn match txr_ncomment ";.*" contained

syn match txr_dot "\." contained
syn match txr_num "#x[+\-]\?[0-9A-Fa-f]\+" contained
syn match txr_num "#o[+\-]\?[0-7]\+" contained
syn match txr_num "#b[+\-]\?[01]\+" contained
syn match txr_ident "[A-Za-z_0-9!$%&*+\-<=>?\\_~]*[A-Za-z_!$%&*+\-<=>?\\_~^][A-Za-z_0-9!$%&*+\-<=>?\\_~^]*" contained
syn match tl_ident "[:@][A-Za-z_0-9!$%&*+\-<=>?\\_~^/]\+" contained
syn match txr_braced_ident "[:][A-Za-z_0-9!$%&*+\-<=>?\\_~^/]\+" contained
syn match tl_ident "[A-Za-z_0-9!$%&*+\-<=>?\\_~/]*[A-Za-z_!$%&*+\-<=>?\\_~^/#][A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]*" contained
syn match txr_num "[+\-]\?[0-9]\+\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1 contained
syn match txr_badnum "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?[A-Za-z_!$%&*+\-<=>?\\_~^/#]\+" contained
syn match txr_num "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1 contained
syn match txr_num "[+\-]\?[0-9]\+\([eE][+\-]\?[0-9]\+\)\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1 contained
syn match tl_ident ":" contained
syn match tl_splice "[ \t,]\|,[*]" contained

syn match txr_unquote "," contained
syn match txr_splice ",\*" contained
syn match txr_quote "'" contained
syn match txr_quote "\^" contained
syn match txr_dotdot "\.\." contained
syn match txr_metaat "@" contained
syn match txr_circ "#[0-9]\+[#=]"

syn region txr_bracevar matchgroup=Delimiter start="@[ \t]*[*]\?{" matchgroup=Delimiter end="}" contains=txr_num,tl_ident,tl_splice,tl_metanum,txr_metaat,txr_circ,txr_braced_ident,txr_dot,txr_dotdot,txr_string,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_regex,txr_quasilit,txr_chr,txr_nested_error
syn region txr_directive matchgroup=Delimiter start="@[ \t]*(" matchgroup=Delimiter end=")" contains=txr_keyword,txr_string,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_num,txr_badnum,tl_ident,tl_regex,txr_string,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_circ,txr_ncomment,txr_nested_error
syn region txr_list contained matchgroup=Delimiter start="\(#[HSR]\?\)\?(" matchgroup=Delimiter end=")" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_ign_par,txr_ign_bkt,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_circ,txr_ncomment,txr_nested_error
syn region txr_bracket contained matchgroup=Delimiter start="\[" matchgroup=Delimiter end="\]" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_ign_par,txr_ign_bkt,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_circ,txr_ncomment,txr_nested_error
syn region txr_mlist contained matchgroup=Delimiter start="@[ \t^',]*(" matchgroup=Delimiter end=")" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_ign_par,txr_ign_bkt,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_circ,txr_ncomment,txr_nested_error
syn region txr_mbracket matchgroup=Delimiter start="@[ \t^',]*\[" matchgroup=Delimiter end="\]" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_ign_par,txr_ign_bkt,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_circ,txr_ncomment,txr_nested_error
syn region txr_string contained start=+#\?\*\?"+ end=+["\n]+ contains=txr_stresc,txr_numesc,txr_badesc
syn region txr_quasilit contained start=+#\?\*\?`+ end=+[`\n]+ contains=txr_splicevar,txr_metanum,txr_bracevar,txr_mlist,txr_mbracket,txr_escat,txr_stresc,txr_numesc,txr_badesc
syn region txr_regex contained start="/" end="[/\n]" contains=txr_regesc,txr_numesc,txr_badesc
syn region tl_regex contained start="#/" end="[/\n]" contains=txr_regesc,txr_numesc,txr_badesc
syn region txr_ign_par contained matchgroup=Comment start="#;[ \t',]*\(#[HSR]\?\)\?(" matchgroup=Comment end=")" contains=txr_ign_par_interior,txr_ign_bkt_interior
syn region txr_ign_bkt contained matchgroup=Comment start="#;[ \t',]*\(#[HSR]\?\)\?\[" matchgroup=Comment end="\]" contains=txr_ign_par_interior,txr_ign_bkt_interior
syn region txr_ign_par_interior contained matchgroup=Comment start="(" matchgroup=Comment end=")" contains=txr_ign_par_interior,txr_ign_bkt_interior
syn region txr_ign_bkt_interior contained matchgroup=Comment start="\[" matchgroup=Comment end="\]" contains=txr_ign_par_interior,txr_ign_bkt_interior

hi def link txr_at Special
hi def link txr_atstar Special
hi def link txr_atat Special
hi def link txr_comment Comment
hi def link txr_ncomment Comment
hi def link txr_hashbang Preproc
hi def link txr_contin Preproc
hi def link txr_char String
hi def link txr_keyword Keyword
hi def link tl_keyword Type
hi def link txr_string String
hi def link txr_chr String
hi def link txr_quasilit String
hi def link txr_regex String
hi def link tl_regex String
hi def link txr_regdir String
hi def link txr_variable Identifier
hi def link txr_splicevar Identifier
hi def link txr_metanum Identifier
hi def link txr_escat Special
hi def link txr_stresc Special
hi def link txr_numesc Special
hi def link txr_regesc Special
hi def link txr_badesc Error
hi def link txr_ident Identifier
hi def link tl_ident Identifier
hi def link txr_num Number
hi def link txr_badnum Error
hi def link txr_quote Special
hi def link txr_unquote Special
hi def link txr_splice Special
hi def link txr_dot Special
hi def link txr_dotdot Special
hi def link txr_metaat Special
hi def link txr_circ Special
hi def link txr_munqspl Special
hi def link tl_splice Special
hi def link txr_error Error
hi def link txr_nested_error Error
hi def link txr_ign_par Comment
hi def link txr_ign_bkt_interior Comment
hi def link txr_ign_par_interior Comment
hi def link txr_ign_bkt Comment

let b:current_syntax = "lisp"

set lispwords=ado,alet,ap,append-each,append-each*,aret,awk,block,block*,build,caseq,caseq*,caseql,caseql*,casequal,casequal*,catch,catch*,collect-each,collect-each*,compare-swap,cond,conda,condlet,dec,defex,define-accessor,define-modify-macro,define-param-expander,define-place-macro,defmacro,defmeth,defpackage,defparm,defparml,defplace,defstruct,defsymacro,defun,defvar,defvarl,del,delay,do,dohash,dotimes,each,each*,equot,flet,flip,for,for*,fun,gen,go,gun,handle,handle*,handler-bind,ido,if,ifa,iflet,ignerr,ignwarn,in-package,ip,labels,lambda,lcons,let,let*,lset,mac-param-bind,macro-time,macrolet,mlet,obtain,obtain*,obtain*-block,obtain-block,op,pdec,pinc,placelet,placelet*,pop,pprof,prof,prog,prog*,prog1,progn,push,pushnew,ret,return,return-from,rlet,rslot,slet,splice,suspend,symacrolet,sys:abscond-from,sys:awk-fun-let,sys:awk-mac-let,sys:awk-redir,sys:catch,sys:conv,sys:dvbind,sys:each-op,sys:expr,sys:fbind,sys:for-op,sys:l1-val,sys:lbind,sys:lisp1-value,sys:path-examine,sys:path-test,sys:placelet-1,sys:splice,sys:struct-lit,sys:switch,sys:unquote,sys:var,sys:with-dyn-rebinds,tagbody,tb,tc,test-clear,test-dec,test-inc,test-set,trace,tree-bind,tree-case,txr-case,txr-case-impl,txr-if,txr-when,typecase,unless,unquote,until,until*,untrace,unwind-protect,upd,when,whena,whenlet,while,while*,whilet,with-clobber-expander,with-delete-expander,with-gensyms,with-hash-iter,with-in-string-byte-stream,with-in-string-stream,with-objects,with-out-string-stream,with-out-strlist-stream,with-resources,with-slots,with-stream,with-update-expander,yield,yield-from,zap,:method,:function,:init,:postinit,:fini
