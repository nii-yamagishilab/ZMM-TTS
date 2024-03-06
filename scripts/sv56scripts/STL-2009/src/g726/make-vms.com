$!-----------------------------------------------------------------------------
$!	VMS DCL for compiling and testing the G.726 implementation
$!	The executable must be defined by variable G726 below.  
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
$!	test16:	process reset vectors for 16 kbit/s
$!	test24:	process reset vectors for 24 kbit/s
$!	test32:	process reset vectors for 32 kbit/s
$!	test40:	process reset vectors for 40 kbit/s
$!	comp:	compare processed reset vectors with reference for all rates
$!	comp16:	compare processed reset vectors with reference for 16 kbit/s
$!	comp24:	compare processed reset vectors with reference for 24 kbit/s
$!	comp32:	compare processed reset vectors with reference for 32 kbit/s
$!	comp40:	compare processed reset vectors with reference for 40 kbit/s
$!
$!	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
$!-----------------------------------------------------------------------------
$ G726 := "g726demo"
$ DIFF := "cf -q"
$ RM   := "delete"
$
$ TEST_VECTORS :=  "*.%16.*,*.%24.*,*.%32.*,*.%40.*,*.rec.*"
$ G726_OBJ := "g726demo.obj.,g726.obj."
$ g726demo:==$"''f$environment(""DEFAULT"")'"g726demo
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
$ 	cc := "cc /include=[-.utl]"
$!
$! > these are for GNU-C
$ else
$ 	CC := "gcc -I../utl"
$ 	libs := ",GNU_CC:[000000]GCCLIB/LIB,SYS$LIBRARY:VAXCRTL/LIB"
$ endif
$
$! General cc options
$!-----------------------------
$ CC_OPT := ""
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
$ 	goto g726demo
$ exit
$
$
$clean:
$	'RM' 'TEST_VECTORS','G726_OBJ'
$exit
$
$g726demo: 
$	'CC' 'CC_OPT' g726.c
$	'CC' 'CC_OPT' g726demo.c
$	LINK 'LINK_OPT' 'G726_OBJ''LIBS'
$	g726demo:==$"''f$environment(""DEFAULT"")'"g726demo
$ exit
$
$
$test:	! clean test16 test24 test32 test40
$test16:
$!
$! Process ADPCM/ Coder for normal and overload sequences, A law
$!
$	'G726' a load 16 bin/nrm.a nrm.a16 256 1 64
$	'G726' a load 16 bin/ovr.a ovr.a16 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'G726' a adlo 16 bin/rn16fa.i rn16fa.rec 256 1 64
$	'G726' a adlo 16 bin/rv16fa.i rv16fa.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM 16kbit/s -> mu law output
$!
$	'G726' u adlo 16 bin/rn16fa.i rn16fx.rec 256 1 64
$	'G726' u adlo 16 bin/rv16fa.i rv16fx.rec 256 1 8
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'G726' u load 16 bin/nrm.m nrm.m16 256 1 64
$	'G726' u load 16 bin/ovr.m ovr.m16 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'G726' u adlo 16 bin/rn16fm.i rn16fm.rec 256 1 64
$	'G726' u adlo 16 bin/rv16fm.i rv16fm.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM 16kbit/s -> A law output
$!
$	'G726' a adlo 16 bin/rn16fm.i rn16fc.rec 256 1 64
$	'G726' a adlo 16 bin/rv16fm.i rv16fc.rec 256 1 8
$!
$!
$! Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'G726' a adlo 16 bin/i16 ri16fa.rec 256 1 64
$	'G726' u adlo 16 bin/i16 ri16fm.rec 256 1 64
$ if ("''p1'".nes."TEST") then exit
$
$test24:
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, A law
$!
$	'G726' a load 24 bin/nrm.a nrm.a24 256 1 64
$	'G726' a load 24 bin/ovr.a ovr.a24 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'G726' a adlo 24 bin/rn24fa.i rn24fa.rec 256 1 64
$	'G726' a adlo 24 bin/rv24fa.i rv24fa.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM 24kbit/s -> mu law output
$!
$	'G726' u adlo 24 bin/rn24fa.i rn24fx.rec 256 1 64
$	'G726' u adlo 24 bin/rv24fa.i rv24fx.rec 256 1 8
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'G726' u load 24 bin/nrm.m nrm.m24 256 1 64
$	'G726' u load 24 bin/ovr.m ovr.m24 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'G726' u adlo 24 bin/rn24fm.i rn24fm.rec 256 1 64
$	'G726' u adlo 24 bin/rv24fm.i rv24fm.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM 24kbit/s -> A law output
$!
$	'G726' a adlo 24 bin/rn24fm.i rn24fc.rec 256 1 64
$	'G726' a adlo 24 bin/rv24fm.i rv24fc.rec 256 1 8
$!
$!
$! Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'G726' a adlo 24 bin/i24 ri24fa.rec 256 1 64
$	'G726' u adlo 24 bin/i24 ri24fm.rec 256 1 64
$ if ("''p1'".nes."TEST") then exit
$
$test32:
$!
$! Process ADPCM/ Coder for normal and overload sequences, A law
$!
$	'G726' a load 32 bin/nrm.a nrm.a32 256 1 64
$	'G726' a load 32 bin/ovr.a ovr.a32 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'G726' a adlo 32 bin/rn32fa.i rn32fa.rec 256 1 64
$	'G726' a adlo 32 bin/rv32fa.i rv32fa.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM 32kbit/s -> mu law output
$!
$	'G726' u adlo 32 bin/rn32fa.i rn32fx.rec 256 1 64
$	'G726' u adlo 32 bin/rv32fa.i rv32fx.rec 256 1 8
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'G726' u load 32 bin/nrm.m nrm.m32 256 1 64
$	'G726' u load 32 bin/ovr.m ovr.m32 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'G726' u adlo 32 bin/rn32fm.i rn32fm.rec 256 1 64
$	'G726' u adlo 32 bin/rv32fm.i rv32fm.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM 32kbit/s -> A law output
$!
$	'G726' a adlo 32 bin/rn32fm.i rn32fc.rec 256 1 64
$	'G726' a adlo 32 bin/rv32fm.i rv32fc.rec 256 1 8
$!
$!
$! Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'G726' a adlo 32 bin/i32 ri32fa.rec 256 1 64
$	'G726' u adlo 32 bin/i32 ri32fm.rec 256 1 64
$ if ("''p1'".nes."TEST") then exit
$
$test40:
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, A law
$!
$	'G726' a load 40 bin/nrm.a nrm.a40 256 1 64
$	'G726' a load 40 bin/ovr.a ovr.a40 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'G726' a adlo 40 bin/rn40fa.i rn40fa.rec 256 1 64
$	'G726' a adlo 40 bin/rv40fa.i rv40fa.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM 40kbit/s -> mu law output
$!
$	'G726' u adlo 40 bin/rn40fa.i rn40fx.rec 256 1 64
$	'G726' u adlo 40 bin/rv40fa.i rv40fx.rec 256 1 8
$!
$!
$! Process ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'G726' u load 40 bin/nrm.m nrm.m40 256 1 64
$	'G726' u load 40 bin/ovr.m ovr.m40 256 1 8
$!
$! Process ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'G726' u adlo 40 bin/rn40fm.i rn40fm.rec 256 1 64
$	'G726' u adlo 40 bin/rv40fm.i rv40fm.rec 256 1 8
$!
$! Process ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM 40kbit/s -> A law output
$!
$	'G726' a adlo 40 bin/rn40fm.i rn40fc.rec 256 1 64
$	'G726' a adlo 40 bin/rv40fm.i rv40fc.rec 256 1 8
$!
$!
$! Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'G726' a adlo 40 bin/i40 ri40fa.rec 256 1 64
$	'G726' u adlo 40 bin/i40 ri40fm.rec 256 1 64
$ exit
$
$comp:	! comp16 comp24 comp32 comp40 
$comp16:
$!
$! =================================================================
$!  COMPARISON OF FILES !
$! =================================================================
$!
$! Compare ADPCM/ Coder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn16fa.i nrm.a16 256 1 64 
$	'DIFF' bin/rv16fa.i ovr.a16 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn16fa.o rn16fa.rec 256 1 64 
$	'DIFF' bin/rv16fa.o rv16fa.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM x kbit/s -> mu law output
$!
$	'DIFF' bin/rn16fx.o rn16fx.rec 256 1 64 
$	'DIFF' bin/rv16fx.o rv16fx.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn16fm.i nrm.m16 256 1 64 
$	'DIFF' bin/rv16fm.i ovr.m16 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn16fm.o rn16fm.rec 256 1 64 
$	'DIFF' bin/rv16fm.o rv16fm.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM x kbit/s -> A law output
$!
$	'DIFF' bin/rn16fc.o rn16fc.rec 256 1 64 
$	'DIFF' bin/rv16fc.o rv16fc.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'DIFF' bin/ri16fa.o ri16fa.rec 256 1 64 
$	'DIFF' bin/ri16fm.o ri16fm.rec 256 1 64 
$ if ("''p1'".nes."COMP") then exit
$
$comp24:
$!
$! =================================================================
$!  COMPARISON OF FILES !
$! =================================================================
$!
$! Compare ADPCM/ Coder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn24fa.i nrm.a24 256 1 64 
$	'DIFF' bin/rv24fa.i ovr.a24 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn24fa.o rn24fa.rec 256 1 64 
$	'DIFF' bin/rv24fa.o rv24fa.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM x kbit/s -> mu law output
$!
$	'DIFF' bin/rn24fx.o rn24fx.rec 256 1 64 
$	'DIFF' bin/rv24fx.o rv24fx.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn24fm.i nrm.m24 256 1 64 
$	'DIFF' bin/rv24fm.i ovr.m24 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn24fm.o rn24fm.rec 256 1 64 
$	'DIFF' bin/rv24fm.o rv24fm.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM x kbit/s -> A law output
$!
$	'DIFF' bin/rn24fc.o rn24fc.rec 256 1 64 
$	'DIFF' bin/rv24fc.o rv24fc.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'DIFF' bin/ri24fa.o ri24fa.rec 256 1 64 
$	'DIFF' bin/ri24fm.o ri24fm.rec 256 1 64 
$ if ("''p1'".nes."COMP") then exit
$
$comp32:
$!
$! =================================================================
$!  COMPARISON OF FILES !
$! =================================================================
$!
$! Compare ADPCM/ Coder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn32fa.i nrm.a32 256 1 64 
$	'DIFF' bin/rv32fa.i ovr.a32 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn32fa.o rn32fa.rec 256 1 64 
$	'DIFF' bin/rv32fa.o rv32fa.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM x kbit/s -> mu law output
$!
$	'DIFF' bin/rn32fx.o rn32fx.rec 256 1 64 
$	'DIFF' bin/rv32fx.o rv32fx.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn32fm.i nrm.m32 256 1 64 
$	'DIFF' bin/rv32fm.i ovr.m32 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn32fm.o rn32fm.rec 256 1 64 
$	'DIFF' bin/rv32fm.o rv32fm.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM x kbit/s -> A law output
$!
$	'DIFF' bin/rn32fc.o rn32fc.rec 256 1 64 
$	'DIFF' bin/rv32fc.o rv32fc.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'DIFF' bin/ri32fa.o ri32fa.rec 256 1 64 
$	'DIFF' bin/ri32fm.o ri32fm.rec 256 1 64 
$ if ("''p1'".nes."COMP") then exit
$
$comp40:
$!
$! =================================================================
$!  COMPARISON OF FILES !
$! =================================================================
$!
$! Compare ADPCM/ Coder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn40fa.i nrm.a40 256 1 64 
$	'DIFF' bin/rv40fa.i ovr.a40 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, A law
$!
$	'DIFF' bin/rn40fa.o rn40fa.rec 256 1 64 
$	'DIFF' bin/rv40fa.o rv40fa.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                A law input -> ADPCM x kbit/s -> mu law output
$!
$	'DIFF' bin/rn40fx.o rn40fx.rec 256 1 64 
$	'DIFF' bin/rv40fx.o rv40fx.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Coder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn40fm.i nrm.m40 256 1 64 
$	'DIFF' bin/rv40fm.i ovr.m40 256 1 8 
$!
$! Compare ADPCM/ Decoder for normal and overload sequences, mu law
$!
$	'DIFF' bin/rn40fm.o rn40fm.rec 256 1 64 
$	'DIFF' bin/rv40fm.o rv40fm.rec 256 1 8 
$!
$! Compare ADPCM/ Cross-decoder for normal and overload sequences, 
$!                mu law input -> ADPCM x kbit/s -> A law output
$!
$	'DIFF' bin/rn40fc.o rn40fc.rec 256 1 64 
$	'DIFF' bin/rv40fc.o rv40fc.rec 256 1 8 
$!
$!
$! Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
$!
$	'DIFF' bin/ri40fa.o ri40fa.rec 256 1 64 
$	'DIFF' bin/ri40fm.o ri40fm.rec 256 1 64 
$ exit
