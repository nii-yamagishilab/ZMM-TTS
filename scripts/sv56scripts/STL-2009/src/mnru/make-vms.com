$!-----------------------------------------------------------------------------
$!	VMS DCL for compiling and testing the UGST MNRU implementation
$!
$!	Usage:
$!	@make-vms 
$!	or
$!	@make-vms target
$!	where target is
$!	cc:	compile usando Vax-C
$!	gcc:	compile usando VAX/VMS gcc
$!	clean:	delete object and test files
$!	test:	process reset vectors for all rates
$!	comp:	compare processed reset vectors with reference for all rates
$!	unzip:	extract reference files from ZIP archive and byte swap
$!
$!	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
$!-----------------------------------------------------------------------------
$ MNRU := "MNRUdemo"
$ DIFF := "cf -q"
$ RM   := "delete"
$
$ TEST_VECTORS :=  "sine.src.,sine*.q%%.,sine*.unx.*"
$ MNRU_OBJ := "MNRUdemo.obj.,MNRU.obj.,ugst-utl.obj."
$ MNRUdemo:==$"''f$environment(""DEFAULT"")'"MNRUdemo
$
$! General link options
$!-----------------------------
$ link_opt := ""
$ 
$! Choose a C compiler
$!-----------------------------
$! > this is for Vax-C - the default
$ if ("''p1'".nes."GCC")
$ then
$ 	cc := "cc"
$!
$! > these are for GNU-C
$ else
$ 	CC := "gcc"
$ 	libs := ",GNU_CC:[000000]GCCLIB/LIB,SYS$LIBRARY:VAXCRTL/LIB"
$ endif
$
$! General cc options
$!-----------------------------
$ CC_OPT := "/INCLUDE=[-.utl]"
$
$! Process User's option
$ if ("''p1'".nes."") 
$ then
$   goto 'p1'
$ else
$   goto all
$ endif
$
$all: 
$anyway: 
$cc:
$gcc:
$ 	goto mnrudemo
$ exit
$
$
$clean:
$	'RM' 'TEST_VECTORS','MNRU_OBJ'
$exit
$
$mnrudemo: 
$	'CC' 'CC_OPT' mnru.c
$	'CC' 'CC_OPT' mnrudemo.c
$	'CC' 'CC_OPT' [-.utl]ugst-utl.c
$	LINK 'LINK_OPT' 'MNRU_OBJ''LIBS'
$	mnrudemo:==$"''f$environment(""DEFAULT"")'"MNRUdemo
$ exit
$
$unzip:
$	unzip sine-ref.zip 
$       sb -over sine-q00.unx
$       sb -over sine-q05.unx
$       sb -over sine-q10.unx
$       sb -over sine-q15.unx
$       sb -over sine-q20.unx
$       sb -over sine-q25.unx
$       sb -over sine-q30.unx
$       sb -over sine-q35.unx
$       sb -over sine-q40.unx
$       sb -over sine-q45.unx
$       sb -over sine-q50.unx
$       sb -over sine-q99.unx
$       sb -over sine.src
$ exit
$
$test:
$	'MNRU' sine.src sine.q00 256 1 20 00
$	'MNRU' sine.src sine.q05 256 1 20 05
$	'MNRU' sine.src sine.q10 256 1 20 10
$	'MNRU' sine.src sine.q15 256 1 20 15
$	'MNRU' sine.src sine.q20 256 1 20 20
$	'MNRU' sine.src sine.q25 256 1 20 25
$	'MNRU' sine.src sine.q30 256 1 20 30
$	'MNRU' sine.src sine.q35 256 1 20 35
$	'MNRU' sine.src sine.q40 256 1 20 40
$	'MNRU' sine.src sine.q45 256 1 20 45
$	'MNRU' sine.src sine.q50 256 1 20 50
$	'MNRU' sine.src sine.q99 256 1 20 150
$ exit
$
$comp: 
$	'DIFF' sine-q00.unx sine.q00
$	'DIFF' sine-q05.unx sine.q05
$	'DIFF' sine-q10.unx sine.q10
$	'DIFF' sine-q15.unx sine.q15
$	'DIFF' sine-q20.unx sine.q20
$	'DIFF' sine-q25.unx sine.q25
$	'DIFF' sine-q30.unx sine.q30
$	'DIFF' sine-q35.unx sine.q35
$	'DIFF' sine-q40.unx sine.q40
$	'DIFF' sine-q45.unx sine.q45
$	'DIFF' sine-q50.unx sine.q50
$	'DIFF' sine-q99.unx sine.q99
$ exit
