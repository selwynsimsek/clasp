(in-package :cmp)

(export '(+fn-prototype+
	  *load-time-value-holder-global-var*
	  irc-intrinsic
	  irc-branch-to-and-begin-block
	  with-ltv-function-codegen
	  +fn-prototype-argument-names+
	  jit-function-name
	  *the-module*
	  *cleavir-compile-hook*
	  *cleavir-compile-file-hook*
	  *llvm-context*
	  with-irbuilder
	  *irbuilder*
	  irc-basic-block-create
	  jit-constant-i32
	  irc-ret-void
	  codegen-literal
	  irc-intrinsic
	  compile-error-if-wrong-number-of-arguments
	  *current-function*
	  *gv-current-function-name*
	  *current-function-name*
	  jit-make-global-string-ptr
	  *run-time-literal-holder*
	  cmp-log
	  cmp-log-dump
	  *run-time-execution-engine*
	  create-run-time-execution-engine
	  irc-environment-activation-frame
	  *run-time-values-table-name*
	  *all-functions-for-one-compile*
	  *dump-module-on-completion*
	  irc-intrinsic-args
	  set-associated-funcs
	  *block-name-prefix*
	  irc-switch
	  +t*+
	  +i1+
	  +size_t+
	  +i32+
	  codegen-rts/symbol
	  irc-begin-block
	  irc-basic-block-create
	  typeid-core-dynamic-go
	  make-calling-convention
	  calling-convention-nargs
	  calling-convention-register-args
	  calling-convention-args
	  calling-convention-copy-args
	  calling-convention-args.gep
	  compile-error-if-not-enough-arguments
	  dbg-set-current-debug-location-here
	  irc-t
	  ))
