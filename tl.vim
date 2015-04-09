" VIM Syntax file for txr
" Kaz Kylheku <kaz@kylheku.com>

" INSTALL-HOWTO:
"
" 1. Create the directory .vim/syntax in your home directory and
"    put the files txr.vim and txl.vim into this directory.
" 2. In your .vimrc, add this command to associate *.txr and *.tl
"    files with the txr and txl filetypes:
"    :au BufRead,BufNewFile *.txr set filetype=txr | set lisp
"    :au BufRead,BufNewFile *.tl set filetype=txl | set lisp
"
" If you want syntax highlighting to be on automatically (for any language)
" you need to add ":syntax on" in your .vimrc also. But you knew that already!
"
" This file is generated by the genvim.txr script in the TXR source tree.

syn case match
syn spell toplevel

setlocal iskeyword=a-z,A-Z,48-57,!,$,&,*,+,-,<,=,>,?,\\,_,~,/

syn keyword txl_keyword contained * *args* *e* *flo-dig*
syn keyword txl_keyword contained *flo-epsilon* *flo-max* *flo-min* *full-args*
syn keyword txl_keyword contained *gensym-counter* *keyword-package* *pi* *random-state*
syn keyword txl_keyword contained *self-path* *stddebug* *stderr* *stdin*
syn keyword txl_keyword contained *stdlog* *stdnull* *stdout* *txr-version*
syn keyword txl_keyword contained *unhandled-hook* *user-package* + -
syn keyword txl_keyword contained / /= < <=
syn keyword txl_keyword contained = > >= abort
syn keyword txl_keyword contained abs abs-path-p acons acons-new
syn keyword txl_keyword contained aconsql-new acos ado alist-nremove
syn keyword txl_keyword contained alist-remove all and andf
syn keyword txl_keyword contained ap apf append append*
syn keyword txl_keyword contained append-each append-each* apply aret
syn keyword txl_keyword contained ash asin assoc assql
syn keyword txl_keyword contained atan atan2 atom bignump
syn keyword txl_keyword contained bit block boundp break-str
syn keyword txl_keyword contained call callf car caseq
syn keyword txl_keyword contained caseql casequal cat-str cat-streams
syn keyword txl_keyword contained cat-vec catch cdr ceil
syn keyword txl_keyword contained chain chand chdir chr-isalnum
syn keyword txl_keyword contained chr-isalpha chr-isascii chr-isblank chr-iscntrl
syn keyword txl_keyword contained chr-isdigit chr-isgraph chr-islower chr-isprint
syn keyword txl_keyword contained chr-ispunct chr-isspace chr-isunisp chr-isupper
syn keyword txl_keyword contained chr-isxdigit chr-num chr-str chr-str-set
syn keyword txl_keyword contained chr-tolower chr-toupper chrp clear-error
syn keyword txl_keyword contained close-stream closelog cmp-str collect-each
syn keyword txl_keyword contained collect-each* comb compl-span-str cond
syn keyword txl_keyword contained cons conses conses* consp
syn keyword txl_keyword contained constantp copy copy-alist copy-cons
syn keyword txl_keyword contained copy-hash copy-list copy-str copy-vec
syn keyword txl_keyword contained cos count-if countq countql
syn keyword txl_keyword contained countqual cum-norm-dist daemon dec
syn keyword txl_keyword contained defmacro defsymacro defun defvar
syn keyword txl_keyword contained del delay delete-package do
syn keyword txl_keyword contained dohash downcase-str dup dwim
syn keyword txl_keyword contained each each* empty ensure-dir
syn keyword txl_keyword contained env env-fbind env-hash env-vbind
syn keyword txl_keyword contained eq eql equal errno
syn keyword txl_keyword contained error eval evenp exit
syn keyword txl_keyword contained exp expt exptmod false
syn keyword txl_keyword contained fbind fboundp fifth filter-equal
syn keyword txl_keyword contained filter-string-tree finalize find find-if
syn keyword txl_keyword contained find-max find-min find-package first
syn keyword txl_keyword contained fixnump flatten flatten* flet
syn keyword txl_keyword contained flip flo-int flo-str floatp
syn keyword txl_keyword contained floor flush-stream for for*
syn keyword txl_keyword contained force fork format fourth
syn keyword txl_keyword contained fun func-get-env func-get-form func-set-env
syn keyword txl_keyword contained functionp gcd gen generate
syn keyword txl_keyword contained gensym gequal get-byte get-char
syn keyword txl_keyword contained get-error get-error-str get-hash-userdata get-line
syn keyword txl_keyword contained get-lines get-list-from-stream get-sig-handler get-string
syn keyword txl_keyword contained get-string-from-stream gethash getitimer getpid
syn keyword txl_keyword contained getppid giterate glob glob-altdirfunc
syn keyword txl_keyword contained glob-brace glob-err glob-mark glob-nocheck
syn keyword txl_keyword contained glob-noescape glob-nomagic glob-nosort glob-onlydir
syn keyword txl_keyword contained glob-period glob-tilde glob-tilde-check greater
syn keyword txl_keyword contained group-by gun hash hash-alist
syn keyword txl_keyword contained hash-construct hash-count hash-diff hash-eql
syn keyword txl_keyword contained hash-equal hash-isec hash-keys hash-pairs
syn keyword txl_keyword contained hash-uni hash-update hash-update-1 hash-values
syn keyword txl_keyword contained hashp html-decode html-encode iapply
syn keyword txl_keyword contained identity ido if iff
syn keyword txl_keyword contained iffi iflet ignerr in
syn keyword txl_keyword contained inc inhash int-flo int-str
syn keyword txl_keyword contained integerp intern interp-fun-p interpose
syn keyword txl_keyword contained ip ipf isqrt itimer-prov
syn keyword txl_keyword contained itimer-real itimer-virtual juxt keep-if
syn keyword txl_keyword contained keep-if* keywordp kill labels
syn keyword txl_keyword contained lambda last lazy-str lazy-str-force
syn keyword txl_keyword contained lazy-str-force-upto lazy-str-get-trailing-list lazy-stream-cons lazy-stringp
syn keyword txl_keyword contained lbind lcm lcons-fun lconsp
syn keyword txl_keyword contained ldiff length length-list length-str
syn keyword txl_keyword contained length-str-< length-str-<= length-str-> length-str->=
syn keyword txl_keyword contained length-vec lequal less let
syn keyword txl_keyword contained let* lexical-fun-p lexical-lisp1-binding lexical-var-p
syn keyword txl_keyword contained link lisp-parse list list*
syn keyword txl_keyword contained list-str list-vector listp log
syn keyword txl_keyword contained log-alert log-auth log-authpriv log-cons
syn keyword txl_keyword contained log-crit log-daemon log-debug log-emerg
syn keyword txl_keyword contained log-err log-info log-ndelay log-notice
syn keyword txl_keyword contained log-nowait log-odelay log-perror log-pid
syn keyword txl_keyword contained log-user log-warning log10 log2
syn keyword txl_keyword contained logand logior lognot logtest
syn keyword txl_keyword contained logtrunc logxor macro-form-p macro-time
syn keyword txl_keyword contained macroexpand macroexpand-1 macrolet major
syn keyword txl_keyword contained make-catenated-stream make-env make-hash make-lazy-cons
syn keyword txl_keyword contained make-like make-package make-random-state make-similar-hash
syn keyword txl_keyword contained make-string-byte-input-stream make-string-input-stream make-string-output-stream make-strlist-output-stream
syn keyword txl_keyword contained make-sym make-time make-time-utc make-trie
syn keyword txl_keyword contained makedev mapcar mapcar* mapdo
syn keyword txl_keyword contained mapf maphash mappend mappend*
syn keyword txl_keyword contained mask match-fun match-regex match-regex-right
syn keyword txl_keyword contained match-regst match-regst-right match-str match-str-tree
syn keyword txl_keyword contained max member member-if memq
syn keyword txl_keyword contained memql memqual merge min
syn keyword txl_keyword contained minor minusp mkdir mknod
syn keyword txl_keyword contained mkstring mod multi multi-sort
syn keyword txl_keyword contained n-choose-k n-perm-k nconc nilf
syn keyword txl_keyword contained none not notf nreverse
syn keyword txl_keyword contained null nullify num-chr num-str
syn keyword txl_keyword contained numberp oand oddp op
syn keyword txl_keyword contained open-command open-directory open-file open-files
syn keyword txl_keyword contained open-files* open-pipe open-process open-tail
syn keyword txl_keyword contained openlog opip or orf
syn keyword txl_keyword contained packagep pad partition partition*
syn keyword txl_keyword contained partition-by perm plusp pop
syn keyword txl_keyword contained pos pos-if pos-max pos-min
syn keyword txl_keyword contained posq posql posqual pppred
syn keyword txl_keyword contained ppred pprinl pprint pprof
syn keyword txl_keyword contained pred prinl print prof
syn keyword txl_keyword contained prog1 progn prop proper-listp
syn keyword txl_keyword contained push pushhash put-byte put-char
syn keyword txl_keyword contained put-line put-lines put-string put-strings
syn keyword txl_keyword contained pwd qquote quasi quasilist
syn keyword txl_keyword contained quote rand random random-fixnum
syn keyword txl_keyword contained random-state-p range range* range-regex
syn keyword txl_keyword contained rcomb read readlink real-time-stream-p
syn keyword txl_keyword contained reduce-left reduce-right ref refset
syn keyword txl_keyword contained regex-compile regex-parse regexp regsub
syn keyword txl_keyword contained rehome-sym remhash remove-if remove-if*
syn keyword txl_keyword contained remove-path remq remq* remql
syn keyword txl_keyword contained remql* remqual remqual* rename-path
syn keyword txl_keyword contained repeat replace replace-list replace-str
syn keyword txl_keyword contained replace-vec rest ret retf
syn keyword txl_keyword contained return return-from reverse rlcp
syn keyword txl_keyword contained rperm rplaca rplacd run
syn keyword txl_keyword contained s-ifblk s-ifchr s-ifdir s-ififo
syn keyword txl_keyword contained s-iflnk s-ifmt s-ifreg s-ifsock
syn keyword txl_keyword contained s-irgrp s-iroth s-irusr s-irwxg
syn keyword txl_keyword contained s-irwxo s-irwxu s-isgid s-isuid
syn keyword txl_keyword contained s-isvtx s-iwgrp s-iwoth s-iwusr
syn keyword txl_keyword contained s-ixgrp s-ixoth s-ixusr search
syn keyword txl_keyword contained search-regex search-regst search-str search-str-tree
syn keyword txl_keyword contained second seek-stream select seqp
syn keyword txl_keyword contained set set-diff set-hash-userdata set-sig-handler
syn keyword txl_keyword contained sethash setitimer setlogmask sh
syn keyword txl_keyword contained sig-abrt sig-alrm sig-bus sig-check
syn keyword txl_keyword contained sig-chld sig-cont sig-fpe sig-hup
syn keyword txl_keyword contained sig-ill sig-int sig-io sig-iot
syn keyword txl_keyword contained sig-kill sig-lost sig-pipe sig-poll
syn keyword txl_keyword contained sig-prof sig-pwr sig-quit sig-segv
syn keyword txl_keyword contained sig-stkflt sig-stop sig-sys sig-term
syn keyword txl_keyword contained sig-trap sig-tstp sig-ttin sig-ttou
syn keyword txl_keyword contained sig-urg sig-usr1 sig-usr2 sig-vtalrm
syn keyword txl_keyword contained sig-winch sig-xcpu sig-xfsz sign-extend
syn keyword txl_keyword contained sin sixth size-vec some
syn keyword txl_keyword contained sort sort-group source-loc source-loc-str
syn keyword txl_keyword contained span-str splice split-str split-str-set
syn keyword txl_keyword contained sqrt sssucc ssucc stat
syn keyword txl_keyword contained stdlib str< str<= str=
syn keyword txl_keyword contained str> str>= stream-get-prop stream-set-prop
syn keyword txl_keyword contained streamp string-extend string-lt stringp
syn keyword txl_keyword contained sub sub-list sub-str sub-vec
syn keyword txl_keyword contained succ symacrolet symbol-function symbol-name
syn keyword txl_keyword contained symbol-package symbol-value symbolp symlink
syn keyword txl_keyword contained sys-qquote sys-splice sys-unquote syslog
syn keyword txl_keyword contained tan tb tc tf
syn keyword txl_keyword contained third throw throwf time
syn keyword txl_keyword contained time-fields-local time-fields-utc time-string-local time-string-utc
syn keyword txl_keyword contained time-usec tofloat toint tok-str
syn keyword txl_keyword contained tok-where tostring tostringp tprint
syn keyword txl_keyword contained transpose tree-bind tree-case tree-find
syn keyword txl_keyword contained trie-add trie-compress trie-lookup-begin trie-lookup-feed-char
syn keyword txl_keyword contained trie-value-at trim-str true trunc
syn keyword txl_keyword contained trunc-rem tuples txr-case txr-if
syn keyword txl_keyword contained txr-when typeof unget-byte unget-char
syn keyword txl_keyword contained uniq unique unless unquote
syn keyword txl_keyword contained until upcase-str update url-decode
syn keyword txl_keyword contained url-encode usleep uw-protect vec
syn keyword txl_keyword contained vec-push vec-set-length vecref vector
syn keyword txl_keyword contained vector-list vectorp w-continued w-coredump
syn keyword txl_keyword contained w-exitstatus w-ifcontinued w-ifexited w-ifsignaled
syn keyword txl_keyword contained w-ifstopped w-nohang w-stopsig w-termsig
syn keyword txl_keyword contained w-untraced wait weave when
syn keyword txl_keyword contained whenlet where while whilet
syn keyword txl_keyword contained width with-saved-vars wrap wrap*
syn keyword txl_keyword contained zap zerop zip
syn match txr_metanum "@[0-9]\+"
syn match txr_nested_error "[^\t `]\+" contained

syn match txr_chr "#\\x[A-Fa-f0-9]\+"
syn match txr_chr "#\\o[0-9]\+"
syn match txr_chr "#\\[^ \t\nA-Za-z0-9_]"
syn match txr_chr "#\\[A-Za-z0-9_]\+"
syn match txr_ncomment ";.*"

syn match txr_dot "\." contained
syn match txr_num "#x[+\-]\?[0-9A-Fa-f]\+"
syn match txr_num "#o[+\-]\?[0-7]\+"
syn match txr_num "#b[+\-]\?[0-1]\+"
syn match txr_ident "[A-Za-z0-9!$%&*+\-<=>?\\_~]*[A-Za-z!$#%&*+\-<=>?\\^_~][A-Za-z0-9!$#%&*+\-<=>?\\^_~]*" contained
syn match txl_ident "[:@][A-Za-z0-9!$%&*+\-<=>?\\\^_~/]\+"
syn match txr_braced_ident "[:][A-Za-z0-9!$%&*+\-<=>?\\\^_~/]\+" contained
syn match txl_ident "[A-Za-z0-9!$%&*+\-<=>?\\_~/]*[A-Za-z!$#%&*+\-<=>?\\^_~/][A-Za-z0-9!$#%&*+\-<=>?\\^_~/]*"
syn match txr_num "[+\-]\?[0-9]\+\([^A-Za-z0-9!$#%&*+\-<=>?\\^_~/]\|\n\)"me=e-1
syn match txr_badnum "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?[A-Za-z!$#%&*+\-<=>?\\^_~/]\+"
syn match txr_num "[+\-]\?[0-9]*[.][0-9]\+\([eE][+\-]\?[0-9]\+\)\?\([^A-Za-z0-9!$#%&*+\-<=>?\\^_~/]\|\n\)"me=e-1
syn match txr_num "[+\-]\?[0-9]\+\([eE][+\-]\?[0-9]\+\)\([^A-Za-z0-9!$#%&*+\-<=>?\\^_~/]\|\n\)"me=e-1
syn match txl_ident ":"
syn match txl_splice "[ \t,]\|,[*]"

syn match txr_unquote "," contained
syn match txr_splice ",\*" contained
syn match txr_quote "'" contained
syn match txr_quote "\^" contained
syn match txr_dotdot "\.\." contained
syn match txr_metaat "@" contained

syn region txr_list matchgroup=Delimiter start="#\?H\?(" matchgroup=Delimiter end=")" contains=txl_keyword,txr_string,txl_regex,txr_num,txr_badnum,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_bracket matchgroup=Delimiter start="\[" matchgroup=Delimiter end="\]" contains=txl_keyword,txr_string,txl_regex,txr_num,txr_badnum,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_mlist matchgroup=Delimiter start="@[ \t]*(" matchgroup=Delimiter end=")" contains=txl_keyword,txr_string,txl_regex,txr_num,txr_badnum,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_mbracket matchgroup=Delimiter start="@[ \t]*\[" matchgroup=Delimiter end="\]" contains=txl_keyword,txr_string,txl_regex,txr_num,txr_badnum,txl_ident,txr_metanum,txr_list,txr_bracket,txr_mlist,txr_mbracket,txr_quasilit,txr_chr,txr_quote,txr_unquote,txr_splice,txr_dot,txr_dotdot,txr_metaat,txr_ncomment,txr_nested_error
syn region txr_string start=+#\?\*\?"+ skip=+\\\\\|\\"\|\\\n+ end=+"\|\n+
syn region txr_quasilit start=+#\?\*\?`+ skip=+\\\\\|\\`\|\\\n+ end=+`\|\n+ contains=txr_splicevar,txr_metanum,txr_bracevar,txr_mlist,txr_mbracket
syn region txr_regex start="/" skip="\\\\\|\\/\|\\\n" end="/\|\n"
syn region txl_regex start="#/" skip="\\\\\|\\/\|\\\n" end="/\|\n"

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
hi def link txr_splicevar Identifier
hi def link txr_metanum Identifier
hi def link txr_ident Identifier
hi def link txl_ident Identifier
hi def link txr_num Number
hi def link txr_badnum Error
hi def link txr_quote Special
hi def link txr_unquote Special
hi def link txr_splice Special
hi def link txr_dot Special
hi def link txr_dotdot Special
hi def link txr_metaat Special
hi def link txr_munqspl Special
hi def link txl_splice Special
hi def link txr_error Error
hi def link txr_nested_error Error

let b:current_syntax = "lisp"
