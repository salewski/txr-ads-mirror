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
syn keyword tl_keyword contained *keyword-package* *lib-version* *listener-hist-len* *listener-multi-line-p*
syn keyword tl_keyword contained *listener-sel-inclusive-p* *load-path* *pi* *place-clobber-expander*
syn keyword tl_keyword contained *place-delete-expander* *place-macro* *place-update-expander* *print-base*
syn keyword tl_keyword contained *print-circle* *print-flo-digits* *print-flo-format* *print-flo-precision*
syn keyword tl_keyword contained *random-state* *random-warmup* *self-path* *stddebug*
syn keyword tl_keyword contained *stderr* *stdin* *stdlog* *stdnull*
syn keyword tl_keyword contained *stdout* *system-package* *trace-output* *txr-version*
syn keyword tl_keyword contained *unhandled-hook* *user-package* + -
syn keyword tl_keyword contained / /= : :abandoned
syn keyword tl_keyword contained :addr :apf :append :args
syn keyword tl_keyword contained :atime :auto :awk-file :awk-rec
syn keyword tl_keyword contained :begin :begin-file :blksize :blocks
syn keyword tl_keyword contained :bool :byte-oriented :cdigit :chars
syn keyword tl_keyword contained :cint :close :continue :counter
syn keyword tl_keyword contained :cspace :ctime :cword-char :dec
syn keyword tl_keyword contained :decline :dev :digit :downcase
syn keyword tl_keyword contained :end :end-file :env :equal-based
syn keyword tl_keyword contained :explicit-no :fd :filter :fini
syn keyword tl_keyword contained :finish :float :form :from-current
syn keyword tl_keyword contained :from-end :from-start :from_html :frombase64
syn keyword tl_keyword contained :fromhtml :frompercent :fromurl :fun
syn keyword tl_keyword contained :function :gap :gid :greedy
syn keyword tl_keyword contained :hex :hextoint :inf :init
syn keyword tl_keyword contained :ino :inp :inputs :instance
syn keyword tl_keyword contained :into :let :lfilt :lines
syn keyword tl_keyword contained :list :longest :mandatory :maxgap
syn keyword tl_keyword contained :maxtimes :method :mingap :mintimes
syn keyword tl_keyword contained :mode :mtime :name :named
syn keyword tl_keyword contained :next-spec :nlink :nothrow :oct
syn keyword tl_keyword contained :outf :outp :output :postinit
syn keyword tl_keyword contained :prio :rdev :real-time :reflect
syn keyword tl_keyword contained :repeat-spec :resolve :rfilt :set
syn keyword tl_keyword contained :set-file :shortest :size :space
syn keyword tl_keyword contained :static :str :string :symacro
syn keyword tl_keyword contained :times :tlist :to_html :tobase64
syn keyword tl_keyword contained :tofloat :tohtml :tohtml* :toint
syn keyword tl_keyword contained :tonumber :topercent :tourl :uid
syn keyword tl_keyword contained :upcase :userdata :var :vars
syn keyword tl_keyword contained :weak-keys :weak-vals :whole :word-char
syn keyword tl_keyword contained :wrap < <= =
syn keyword tl_keyword contained > >= abort abs
syn keyword tl_keyword contained abs-path-p acons acons-new aconsql-new
syn keyword tl_keyword contained acos ado af-inet af-inet6
syn keyword tl_keyword contained af-unix af-unspec ai-addrconfig ai-all
syn keyword tl_keyword contained ai-canonname ai-numerichost ai-numericserv ai-passive
syn keyword tl_keyword contained ai-v4mapped alet alist-nremove alist-remove
syn keyword tl_keyword contained all and andf ap
syn keyword tl_keyword contained apf append append* append-each
syn keyword tl_keyword contained append-each* apply aret ash
syn keyword tl_keyword contained asin assoc assql at-exit-call
syn keyword tl_keyword contained at-exit-do-not-call atan atan2 atom
syn keyword tl_keyword contained awk base64-decode base64-encode bignump
syn keyword tl_keyword contained bindable bit block block*
syn keyword tl_keyword contained boundp break-str brkint bs0
syn keyword tl_keyword contained bs1 bsdly build build-list
syn keyword tl_keyword contained butlast butlastn caaaaar caaaadr
syn keyword tl_keyword contained caaaar caaadar caaaddr caaadr
syn keyword tl_keyword contained caaar caadaar caadadr caadar
syn keyword tl_keyword contained caaddar caadddr caaddr caadr
syn keyword tl_keyword contained caar cadaaar cadaadr cadaar
syn keyword tl_keyword contained cadadar cadaddr cadadr cadar
syn keyword tl_keyword contained caddaar caddadr caddar cadddar
syn keyword tl_keyword contained caddddr cadddr caddr cadr
syn keyword tl_keyword contained call call-clobber-expander call-delete-expander call-finalizers
syn keyword tl_keyword contained call-super-fun call-super-method call-update-expander callf
syn keyword tl_keyword contained car caseq caseql casequal
syn keyword tl_keyword contained cat-str cat-streams cat-vec catch
syn keyword tl_keyword contained catenated-stream-p catenated-stream-push cbaud cbaudex
syn keyword tl_keyword contained cdaaaar cdaaadr cdaaar cdaadar
syn keyword tl_keyword contained cdaaddr cdaadr cdaar cdadaar
syn keyword tl_keyword contained cdadadr cdadar cdaddar cdadddr
syn keyword tl_keyword contained cdaddr cdadr cdar cddaaar
syn keyword tl_keyword contained cddaadr cddaar cddadar cddaddr
syn keyword tl_keyword contained cddadr cddar cdddaar cdddadr
syn keyword tl_keyword contained cdddar cddddar cdddddr cddddr
syn keyword tl_keyword contained cdddr cddr cdr ceil
syn keyword tl_keyword contained chain chand chdir chmod
syn keyword tl_keyword contained chr-digit chr-int chr-isalnum chr-isalpha
syn keyword tl_keyword contained chr-isascii chr-isblank chr-iscntrl chr-isdigit
syn keyword tl_keyword contained chr-isgraph chr-islower chr-isprint chr-ispunct
syn keyword tl_keyword contained chr-isspace chr-isunisp chr-isupper chr-isxdigit
syn keyword tl_keyword contained chr-num chr-str chr-str-set chr-tolower
syn keyword tl_keyword contained chr-toupper chr-xdigit chrp clamp
syn keyword tl_keyword contained clear-error clear-struct clocal close-stream
syn keyword tl_keyword contained closelog cmp-str cmspar collect-each
syn keyword tl_keyword contained collect-each* comb compare-swap compl-span-str
syn keyword tl_keyword contained cond conda condlet cons
syn keyword tl_keyword contained conses conses* consp constantp
syn keyword tl_keyword contained copy copy-alist copy-cons copy-hash
syn keyword tl_keyword contained copy-list copy-str copy-struct copy-vec
syn keyword tl_keyword contained cos count-if countq countql
syn keyword tl_keyword contained countqual cr0 cr1 cr2
syn keyword tl_keyword contained cr3 crdly cread crtscts
syn keyword tl_keyword contained crypt cs5 cs6 cs7
syn keyword tl_keyword contained cs8 csize cstopb cum-norm-dist
syn keyword tl_keyword contained daemon dec defex define-accessor
syn keyword tl_keyword contained define-modify-macro define-place-macro defmacro defmeth
syn keyword tl_keyword contained defparm defparml defplace defstruct
syn keyword tl_keyword contained defsymacro defun defvar defvarl
syn keyword tl_keyword contained del delay delete-package display-width
syn keyword tl_keyword contained do dohash dotimes downcase-str
syn keyword tl_keyword contained drop drop-until drop-while dup
syn keyword tl_keyword contained dupfd dwim each each*
syn keyword tl_keyword contained echo echoctl echoe echok
syn keyword tl_keyword contained echoke echonl echoprt eighth
syn keyword tl_keyword contained empty endgrent endpwent ensure-dir
syn keyword tl_keyword contained env env-fbind env-hash env-vbind
syn keyword tl_keyword contained eq eql equal errno
syn keyword tl_keyword contained error eval evenp exception-subtype-p
syn keyword tl_keyword contained exec exit exit* exp
syn keyword tl_keyword contained expand-left expand-right expt exptmod
syn keyword tl_keyword contained extproc f$ f^ f^$
syn keyword tl_keyword contained false fboundp ff0 ff1
syn keyword tl_keyword contained ffdly fifth fileno filter-equal
syn keyword tl_keyword contained filter-string-tree finalize find find-frame
syn keyword tl_keyword contained find-if find-max find-min find-package
syn keyword tl_keyword contained find-struct-type first fixnum-max fixnum-min
syn keyword tl_keyword contained fixnump flatcar flatcar* flatten
syn keyword tl_keyword contained flatten* flet flip flipargs
syn keyword tl_keyword contained flo-dig flo-epsilon flo-int flo-max
syn keyword tl_keyword contained flo-max-dig flo-min flo-str floatp
syn keyword tl_keyword contained floor flush-stream flusho fmakunbound
syn keyword tl_keyword contained fmt fnm-casefold fnm-leading-dir fnm-noescape
syn keyword tl_keyword contained fnm-pathname fnm-period fnmatch for
syn keyword tl_keyword contained for* force fork format
syn keyword tl_keyword contained fourth from fstat ftw
syn keyword tl_keyword contained ftw-actionretval ftw-chdir ftw-continue ftw-d
syn keyword tl_keyword contained ftw-depth ftw-dnr ftw-dp ftw-f
syn keyword tl_keyword contained ftw-mount ftw-ns ftw-phys ftw-skip-siblings
syn keyword tl_keyword contained ftw-skip-subtree ftw-sl ftw-sln ftw-stop
syn keyword tl_keyword contained fun func-get-env func-get-form func-get-name
syn keyword tl_keyword contained func-set-env functionp gcd gen
syn keyword tl_keyword contained generate gensym gequal get-byte
syn keyword tl_keyword contained get-char get-clobber-expander get-delete-expander get-error
syn keyword tl_keyword contained get-error-str get-frames get-hash-userdata get-indent
syn keyword tl_keyword contained get-indent-mode get-line get-lines get-list-from-stream
syn keyword tl_keyword contained get-sig-handler get-string get-string-from-stream get-update-expander
syn keyword tl_keyword contained getaddrinfo getegid getenv geteuid
syn keyword tl_keyword contained getgid getgrent getgrgid getgrnam
syn keyword tl_keyword contained getgroups gethash getitimer getopts
syn keyword tl_keyword contained getpid getppid getpwent getpwnam
syn keyword tl_keyword contained getpwuid getresgid getresuid getuid
syn keyword tl_keyword contained ginterate giterate glob glob-altdirfunc
syn keyword tl_keyword contained glob-brace glob-err glob-mark glob-nocheck
syn keyword tl_keyword contained glob-noescape glob-nomagic glob-nosort glob-onlydir
syn keyword tl_keyword contained glob-period glob-tilde glob-tilde-check greater
syn keyword tl_keyword contained group-by group-reduce gun handle
syn keyword tl_keyword contained handler-bind hash hash-alist hash-begin
syn keyword tl_keyword contained hash-construct hash-count hash-diff hash-eql
syn keyword tl_keyword contained hash-equal hash-from-pairs hash-isec hash-keys
syn keyword tl_keyword contained hash-list hash-next hash-pairs hash-proper-subset
syn keyword tl_keyword contained hash-revget hash-subset hash-uni hash-update
syn keyword tl_keyword contained hash-update-1 hash-userdata hash-values hashp
syn keyword tl_keyword contained have html-decode html-encode html-encode*
syn keyword tl_keyword contained hupcl iapply icanon icrnl
syn keyword tl_keyword contained identity ido iexten if
syn keyword tl_keyword contained ifa iff iffi iflet
syn keyword tl_keyword contained ignbrk igncr ignerr ignpar
syn keyword tl_keyword contained imaxbel in in6addr-any in6addr-loopback
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
syn keyword tl_keyword contained member member-if memq memql
syn keyword tl_keyword contained memqual merge meth method
syn keyword tl_keyword contained min minor minusp mkdir
syn keyword tl_keyword contained mknod mkstring mlet mmakunbound
syn keyword tl_keyword contained mod multi multi-sort n-choose-k
syn keyword tl_keyword contained n-perm-k nconc neq neql
syn keyword tl_keyword contained nequal new nexpand-left nil
syn keyword tl_keyword contained nilf ninth nl0 nl1
syn keyword tl_keyword contained nldly noflsh none not
syn keyword tl_keyword contained notf nreconc nreverse nthcdr
syn keyword tl_keyword contained nthlast null nullify num-chr
syn keyword tl_keyword contained num-str numberp oand obtain
syn keyword tl_keyword contained obtain* obtain*-block obtain-block ocrnl
syn keyword tl_keyword contained oddp ofdel ofill olcuc
syn keyword tl_keyword contained onlcr onlret onocr op
syn keyword tl_keyword contained open-command open-directory open-file open-fileno
syn keyword tl_keyword contained open-files open-files* open-pipe open-process
syn keyword tl_keyword contained open-socket open-socket-pair open-tail openlog
syn keyword tl_keyword contained opip opost opt opthelp
syn keyword tl_keyword contained or orf package-alist package-name
syn keyword tl_keyword contained package-symbols packagep pad parenb
syn keyword tl_keyword contained parmrk parodd partition partition*
syn keyword tl_keyword contained partition-by path-blkdev-p path-chrdev-p path-dir-p
syn keyword tl_keyword contained path-executable-to-me-p path-exists-p path-file-p path-mine-p
syn keyword tl_keyword contained path-my-group-p path-newer path-older path-pipe-p
syn keyword tl_keyword contained path-private-to-me-p path-read-writable-to-me-p path-readable-to-me-p path-same-object
syn keyword tl_keyword contained path-setgid-p path-setuid-p path-sock-p path-sticky-p
syn keyword tl_keyword contained path-strictly-private-to-me-p path-symlink-p path-writable-to-me-p pdec
syn keyword tl_keyword contained pendin perm pinc pipe
syn keyword tl_keyword contained place-form-p placelet placelet* plusp
syn keyword tl_keyword contained poll poll-err poll-in poll-nval
syn keyword tl_keyword contained poll-out poll-pri poll-rdband poll-rdhup
syn keyword tl_keyword contained poll-wrband pop pos pos-if
syn keyword tl_keyword contained pos-max pos-min posq posql
syn keyword tl_keyword contained posqual pppred ppred pprinl
syn keyword tl_keyword contained pprint pprof pred prinl
syn keyword tl_keyword contained print prof prog1 progn
syn keyword tl_keyword contained promisep prop proper-list-p proper-listp
syn keyword tl_keyword contained pset pure-rel-path-p push pushhash
syn keyword tl_keyword contained pushnew put-byte put-char put-line
syn keyword tl_keyword contained put-lines put-string put-strings pwd
syn keyword tl_keyword contained qquote qref quote r$
syn keyword tl_keyword contained r^ r^$ raise rand
syn keyword tl_keyword contained random random-fixnum random-state-get-vec random-state-p
syn keyword tl_keyword contained range range* range-regex rangep
syn keyword tl_keyword contained rcomb rcons read read-until-match
syn keyword tl_keyword contained readlink real-time-stream-p record-adapter reduce-left
syn keyword tl_keyword contained reduce-right ref refset regex-compile
syn keyword tl_keyword contained regex-from-trie regex-parse regex-source regexp
syn keyword tl_keyword contained register-exception-subtypes regsub rehome-sym remhash
syn keyword tl_keyword contained remove-if remove-if* remove-path remq
syn keyword tl_keyword contained remq* remql remql* remqual
syn keyword tl_keyword contained remqual* rename-path repeat replace
syn keyword tl_keyword contained replace-list replace-str replace-struct replace-vec
syn keyword tl_keyword contained reset-struct rest ret retf
syn keyword tl_keyword contained return return* return-from revappend
syn keyword tl_keyword contained reverse rfind rfind-if rlcp
syn keyword tl_keyword contained rlet rmember rmember-if rmemq
syn keyword tl_keyword contained rmemql rmemqual rotate rperm
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
syn keyword tl_keyword contained set-hash-userdata set-indent set-indent-mode set-sig-handler
syn keyword tl_keyword contained setegid setenv seteuid setgid
syn keyword tl_keyword contained setgrent setgroups sethash setitimer
syn keyword tl_keyword contained setlogmask setpwent setresgid setresuid
syn keyword tl_keyword contained setuid seventh sh shift
syn keyword tl_keyword contained shuffle shut-rd shut-rdwr shut-wr
syn keyword tl_keyword contained sig-abrt sig-alrm sig-bus sig-check
syn keyword tl_keyword contained sig-chld sig-cont sig-fpe sig-hup
syn keyword tl_keyword contained sig-ill sig-int sig-io sig-iot
syn keyword tl_keyword contained sig-kill sig-pipe sig-poll sig-prof
syn keyword tl_keyword contained sig-pwr sig-quit sig-segv sig-stkflt
syn keyword tl_keyword contained sig-stop sig-sys sig-term sig-trap
syn keyword tl_keyword contained sig-tstp sig-ttin sig-ttou sig-urg
syn keyword tl_keyword contained sig-usr1 sig-usr2 sig-vtalrm sig-winch
syn keyword tl_keyword contained sig-xcpu sig-xfsz sign-extend sin
syn keyword tl_keyword contained sixth size-vec slet slot
syn keyword tl_keyword contained slotp slots slotset sock-accept
syn keyword tl_keyword contained sock-bind sock-cloexec sock-connect sock-dgram
syn keyword tl_keyword contained sock-family sock-listen sock-nonblock sock-peer
syn keyword tl_keyword contained sock-recv-timeout sock-send-timeout sock-set-peer sock-shutdown
syn keyword tl_keyword contained sock-stream sock-type some sort
syn keyword tl_keyword contained sort-group source-loc source-loc-str span-str
syn keyword tl_keyword contained special-operator-p special-var-p splice split
syn keyword tl_keyword contained split* split-str split-str-set sqrt
syn keyword tl_keyword contained sssucc ssucc stat static-slot
syn keyword tl_keyword contained static-slot-ensure static-slot-p static-slot-set stdlib
syn keyword tl_keyword contained str-in6addr str-in6addr-net str-inaddr str-inaddr-net
syn keyword tl_keyword contained str< str<= str= str>
syn keyword tl_keyword contained str>= stream-get-prop stream-set-prop streamp
syn keyword tl_keyword contained string-extend string-lt stringp struct-type
syn keyword tl_keyword contained struct-type-p structp sub sub-list
syn keyword tl_keyword contained sub-str sub-vec subtypep succ
syn keyword tl_keyword contained super super-method suspend swap
syn keyword tl_keyword contained symacrolet symbol-function symbol-macro symbol-name
syn keyword tl_keyword contained symbol-package symbol-value symbolp symlink
syn keyword tl_keyword contained sys:*lisp1* sys:*pl-env* sys:*trace-hash* sys:*trace-level*
syn keyword tl_keyword contained sys:abscond* sys:abscond-from sys:apply sys:awk-expander
syn keyword tl_keyword contained sys:awk-let sys:awk-redir sys:awk-test sys:bad-slot-syntax
syn keyword tl_keyword contained sys:capture-cont sys:circref sys:conv sys:conv-expand
syn keyword tl_keyword contained sys:conv-let sys:cp-origin sys:defmeth sys:do-conv
syn keyword tl_keyword contained sys:do-path-test sys:dwim-del sys:dwim-set sys:eval-err
syn keyword tl_keyword contained sys:expand sys:expr sys:fbind sys:gc
syn keyword tl_keyword contained sys:gc-set-delta sys:get-fun-getter-setter sys:get-mb sys:get-vb
syn keyword tl_keyword contained sys:handle-bad-syntax sys:if-to-cond sys:in6addr-condensed-text sys:l1-setq
syn keyword tl_keyword contained sys:l1-val sys:lbind sys:lisp1-setq sys:lisp1-value
syn keyword tl_keyword contained sys:list-builder-macrolets sys:make-struct-lit sys:make-struct-type sys:mark-special
syn keyword tl_keyword contained sys:obtain-impl sys:opt-dash sys:opt-err sys:path-access
syn keyword tl_keyword contained sys:path-examine sys:path-test sys:path-test-mode sys:pl-expand
syn keyword tl_keyword contained sys:placelet-1 sys:prune-missing-inits sys:qquote sys:quasi
syn keyword tl_keyword contained sys:quasilist sys:r-s-let-expander sys:reg-expand-nongreedy sys:reg-optimize
syn keyword tl_keyword contained sys:register-simple-accessor sys:rplaca sys:rplacd sys:rslotset
syn keyword tl_keyword contained sys:set-hash-rec-limit sys:set-hash-str-limit sys:set-macro-ancestor sys:setq
syn keyword tl_keyword contained sys:setqf sys:splice sys:str-inaddr-net-impl sys:struct-lit
syn keyword tl_keyword contained sys:sym-clobber-expander sys:sym-delete-expander sys:sym-update-expander sys:top-fb
syn keyword tl_keyword contained sys:top-mb sys:top-vb sys:trace sys:trace-enter
syn keyword tl_keyword contained sys:trace-leave sys:unquote sys:untrace sys:var
syn keyword tl_keyword contained sys:wdwrap sys:with-saved-vars syslog system-package
syn keyword tl_keyword contained t tab0 tab1 tab2
syn keyword tl_keyword contained tab3 tabdly take take-until
syn keyword tl_keyword contained take-while tan tb tc
syn keyword tl_keyword contained tcdrain tcflow tcflush tcgetattr
syn keyword tl_keyword contained tciflush tcioff tcioflush tcion
syn keyword tl_keyword contained tcoflush tcooff tcoon tcsadrain
syn keyword tl_keyword contained tcsaflush tcsanow tcsendbreak tcsetattr
syn keyword tl_keyword contained tenth test-clear test-dec test-inc
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
syn keyword tl_keyword contained unget-byte unget-char uniq unique
syn keyword tl_keyword contained unless unquote unsetenv until
syn keyword tl_keyword contained until* untrace unwind-protect upcase-str
syn keyword tl_keyword contained upd update url-decode url-encode
syn keyword tl_keyword contained use user-package usl usleep
syn keyword tl_keyword contained uslot vdiscard vec vec-list
syn keyword tl_keyword contained vec-push vec-set-length vecref vector
syn keyword tl_keyword contained vector-list vectorp veof veol
syn keyword tl_keyword contained veol2 verase vintr vkill
syn keyword tl_keyword contained vlnext vmin vquit vreprint
syn keyword tl_keyword contained vstart vstop vsusp vswtc
syn keyword tl_keyword contained vt0 vt1 vtdly vtime
syn keyword tl_keyword contained vwerase w-continued w-coredump w-exitstatus
syn keyword tl_keyword contained w-ifcontinued w-ifexited w-ifsignaled w-ifstopped
syn keyword tl_keyword contained w-nohang w-stopsig w-termsig w-untraced
syn keyword tl_keyword contained wait weave when whenlet
syn keyword tl_keyword contained where while while* whilet
syn keyword tl_keyword contained width width-check window-map window-mappend
syn keyword tl_keyword contained with-clobber-expander with-delete-expander with-gensyms with-hash-iter
syn keyword tl_keyword contained with-in-string-byte-stream with-in-string-stream with-objects with-out-string-stream
syn keyword tl_keyword contained with-out-strlist-stream with-resources with-slots with-stream
syn keyword tl_keyword contained with-update-expander wrap wrap* xcase
syn keyword tl_keyword contained yield yield-from zap zerop
syn keyword tl_keyword contained zip
syn match txr_nested_error "[^\t ]\+" contained
syn match txr_variable "\(@[ \t]*\)[*]\?[ \t]*[A-Za-z_][A-Za-z_0-9]*"
syn match txr_splicevar "@[ \t,*@]*[A-Za-z_][A-Za-z_0-9]*" contained
syn match txr_metanum "@\+[0-9]\+"
syn match txr_badesc "\\." contained
syn match txr_escat "\\@" contained
syn match txr_stresc "\\[abtnvfre\\ \n"`']" contained
syn match txr_numesc "\\x[0-9A-Fa-f]\+;\?" contained
syn match txr_numesc "\\[0-7]\+;\?" contained
syn match txr_regesc "\\[abtnvfre\\ \n/sSdDwW()\|.*?+~&%\[\]\-]" contained

syn match txr_chr "#\\x[0-9A-Fa-f]\+"
syn match txr_chr "#\\o[0-7]\+"
syn match txr_chr "#\\[^ \t\nA-Za-z_0-9]"
syn match txr_chr "#\\[A-Za-z_0-9]\+"
syn match txr_ncomment ";.*"

syn match txr_dot "\." contained
syn match txr_num "#x[+\-]\?[0-9A-Fa-f]\+"
syn match txr_num "#o[+\-]\?[0-7]\+"
syn match txr_num "#b[+\-]\?[01]\+"
syn match txr_ident "[A-Za-z_0-9!$%&*+\-<=>?\\_~]*[A-Za-z_!$%&*+\-<=>?\\_~^][A-Za-z_0-9!$%&*+\-<=>?\\_~^]*" contained
syn match tl_ident "[:@][A-Za-z_0-9!$%&*+\-<=>?\\_~^/]\+"
syn match txr_braced_ident "[:][A-Za-z_0-9!$%&*+\-<=>?\\_~^/]\+" contained
syn match tl_ident "[A-Za-z_0-9!$%&*+\-<=>?\\_~/]*[A-Za-z_!$%&*+\-<=>?\\_~^/#][A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]*"
syn match txr_num "[+\-]\?[0-9]\+\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1
syn match txr_badnum "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?[A-Za-z_!$%&*+\-<=>?\\_~^/#]\+"
syn match txr_num "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1
syn match txr_num "[+\-]\?[0-9]\+\([eE][+\-]\?[0-9]\+\)\([^A-Za-z_0-9!$%&*+\-<=>?\\_~^/#]\|\n\)"me=e-1
syn match tl_ident ":"
syn match tl_splice "[ \t,]\|,[*]"

syn match txr_unquote "," contained
syn match txr_splice ",\*" contained
syn match txr_quote "'" contained
syn match txr_quote "\^" contained
syn match txr_dotdot "\.\." contained
syn match txr_metaat "@" contained

syn region txr_bracevar matchgroup=Delimiter start="@[ \t]*[*]\?{" matchgroup=Delimiter end="}" contains=txr_num,tl_ident,tl_splice,tl_metanum,txr_metaat,txr_braced_ident,txr_dot,txr_dotdot,txr_string,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_regex,txr_quasilit,txr_chr,tl_splice,txr_nested_error
syn region txr_list matchgroup=Delimiter start="\(#[HSR]\?\)\?(" matchgroup=Delimiter end=")" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_bracket matchgroup=Delimiter start="\[" matchgroup=Delimiter end="\]" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_mlist matchgroup=Delimiter start="@[ \t^',]*(" matchgroup=Delimiter end=")" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_mbracket matchgroup=Delimiter start="@[ \t^',]*\[" matchgroup=Delimiter end="\]" contains=tl_keyword,txr_string,tl_regex,txr_num,txr_badnum,tl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_string start=+#\?\*\?"+ end=+["\n]+ contains=txr_stresc,txr_numesc,txr_badesc
syn region txr_quasilit start=+#\?\*\?`+ end=+[`\n]+ contains=txr_splicevar,txr_metanum,txr_bracevar,txr_mlist,txr_mbracket,txr_escat,txr_stresc,txr_numesc,txr_badesc
syn region txr_regex start="/" end="[/\n]" contains=txr_regesc,txr_numesc,txr_badesc
syn region tl_regex start="#/" end="[/\n]" contains=txr_regesc,txr_numesc,txr_badesc

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
hi def link txr_munqspl Special
hi def link tl_splice Special
hi def link txr_error Error
hi def link txr_nested_error Error

let b:current_syntax = "lisp"

set lispwords=ado,alet,ap,append-each,append-each*,aret,awk,block,block*,build,caseq,caseql,casequal,catch,collect-each,collect-each*,compare-swap,cond,conda,condlet,dec,defex,define-accessor,define-modify-macro,define-place-macro,defmacro,defmeth,defparm,defparml,defplace,defstruct,defsymacro,defun,defvar,defvarl,del,delay,do,dohash,dotimes,each,each*,flet,flip,for,for*,fun,gen,gun,handle,handler-bind,ido,if,ifa,iflet,ignerr,ip,labels,lambda,lcons,let,let*,lset,mac-param-bind,macro-time,macrolet,mlet,obtain,obtain*,obtain*-block,obtain-block,op,pdec,pinc,placelet,placelet*,pop,pprof,prof,prog1,progn,push,pushnew,ret,return,return-from,rlet,rslot,slet,splice,suspend,symacrolet,sys:abscond-from,sys:awk-let,sys:awk-redir,sys:conv,sys:expr,sys:fbind,sys:l1-val,sys:lbind,sys:lisp1-value,sys:path-examine,sys:path-test,sys:placelet-1,sys:splice,sys:struct-lit,sys:unquote,sys:var,sys:with-saved-vars,tb,tc,test-clear,test-dec,test-inc,test-set,trace,tree-bind,tree-case,txr-case,txr-case-impl,txr-if,txr-when,typecase,unless,unquote,until,until*,untrace,unwind-protect,upd,when,whenlet,while,while*,whilet,with-clobber-expander,with-delete-expander,with-gensyms,with-hash-iter,with-in-string-byte-stream,with-in-string-stream,with-objects,with-out-string-stream,with-out-strlist-stream,with-resources,with-slots,with-stream,with-update-expander,yield,yield-from,zap,:method,:function,:init,:postinit,:fini
