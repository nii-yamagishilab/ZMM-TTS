$goto start
$!============================================================================
$! Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
$! Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
$! details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
$! File adapted by simao@cpqd.ansp.br.
$!
$! Machine dependent flags you must configure to port
$! - Define SASR if >> is a signed arithmetic shift (-1 >> 1 == -1'
$! - Define USE_FLOAT_MUL if your host multiplies floats faster than integers,
$!   e.g. on a SPARCstation.
$! - define FAST together with USE_FLOAT_MUL to enable the GSM library's
$!   approximation option for incorrect, but good-enough results.
$!   Undefine for compliant operation.
$! - Define NDEBUG to avoid the generation of debug functions along the code
$! - Choose a compiler.  The code works both with ANSI and K&R-C.
$!   Use -DNeedFunctionPrototypes to compile with, -UNeedFunctionPrototypes to
$!   compile without, function prototypes in the header files.
$!
$! - You can use the -DSTUPID_COMPILER to circumvent some compilers'
$!   static limits regarding the number of subexpressions in a statement.
$!
$!
$!			    -- *NOTE*: --
$! 
$!	 This VMS DCL file created by <simao@ctd.comsat.com>
$!		      for the UGST distribution
$!=============================================================================
$start:
$! --> if you want to use VAX-C, uncomment the CC and LIBS lines and 
$!      one of the CC_OPT below
$ CC	 = "cc /NOWARNING" !! "/DIA"
$ CC_OPT = "/INC=[]/INC=[-.utl] /DEF=(SASR,NeedFunctionPrototypes=1,NDEBUG)"
$! CC_OPT = "/INC=[]/INC=[-.utl] /DEF=(SASR,FAST,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! CC_OPT = "/INC=[/INC=[-.utl]] /DEF=(SASR,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! CC_OPT = "/INC=[]/INC=[-.utl] /DEF=(VMS,SASR,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! CC_OPT = "/INC=[]/INC=[-.utl] /STANDARD=portable /DEF=(VMS,SASR,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! LIBS = ",SYS$LIBRARY:VAXCRTL/LIB"
$! --> if you want to use GCC, uncomment the CC and LIBS lines and one of 
$!	the CC_OPT below
$! CC	 = "gcc"
$! CC_OPT = "/INC=[]/INC=[-.utl] /DEF=(VMS,SASR,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! CC_OPT = "/INC=[]/INC=[-.utl] /DEF=(SASR,USE_FLOAT_MUL,NeedFunctionPrototypes=1,NDEBUG)"
$! LIBS = ",GNU_CC:[000000]GCCLIB/LIB,SYS$LIBRARY:VAXCRTL/LIB"

$! goto here
$!
$ 'CC'  'CC_OPT' rpedemo.c 
$ 'CC'  'CC_OPT' rpeltp.c 
$ 'CC'  'CC_OPT' add.c 
$ 'CC'  'CC_OPT' code.c 
$ 'CC'  'CC_OPT' debug.c 
$ 'CC'  'CC_OPT' decode.c 
$ 'CC'  'CC_OPT' long_ter.c 
$ 'CC'  'CC_OPT' lpc.c 
$ 'CC'  'CC_OPT' preproce.c 
$ 'CC'  'CC_OPT' rpe.c 
$ 'CC'  'CC_OPT' gsm_dest.c 
$ 'CC'  'CC_OPT' gsm_deco.c 
$ 'CC'  'CC_OPT' gsm_enco.c 
$ 'CC'  'CC_OPT' gsm_expl.c 
$ 'CC'  'CC_OPT' gsm_impl.c 
$ 'CC'  'CC_OPT' gsm_crea.c 
$ 'CC'  'CC_OPT' gsm_prin.c 
$ 'CC'  'CC_OPT' gsm_opti.c 
$ 'CC'  'CC_OPT' short_te.c 
$ 'CC'  'CC_OPT' table.c
$!
$ 'CC'  'CC_OPT' g711.c 
$
$here:
$link rpedemo.obj,rpeltp.obj,add.obj,code.obj,debug.obj,decode.obj,-
	long_ter.obj,lpc.obj,preproce.obj,rpe.obj,gsm_dest.obj,gsm_deco.obj,-
	gsm_enco.obj,gsm_expl.obj,gsm_impl.obj,gsm_crea.obj,-
	gsm_prin.obj,gsm_opti.obj,short_te.obj,table.obj,g711.obj'LIBS'
$
