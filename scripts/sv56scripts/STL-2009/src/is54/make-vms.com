$ goto start
$!**************************************************************************
$!
$!               (C) Copyright 1990, Motorola Inc., U.S.A.
$!
$! Note:  Reproduction and use for the development of North American digital
$!        cellular standards or development of digital speech coding
$!        standards within the International Telecommunications Union -
$!        Telecommunications Standardization Sector is authorized by 
$!	  Motorola Inc.  No other use is intended or authorized.
$!
$!        The availability of this material does not provide any license
$!        by implication, estoppel, or otherwise under any patent rights
$!        of Motorola Inc. or others covering any use of the contents
$!        herein.
$!
$!        Any copies or derivative works must incude this and all other
$!        proprietary notices.
$!
$! Systems Research Laboratories
$! Chicago Corporate Research and Development Center
$! Motorola Inc.
$!
$! -- *NOTE:* --
$!  This VMS DCL file has been written by <simao@cpqd.ansp.br> for 
$!  the UGST distribution
$!
$!**************************************************************************
$start: 
$!
$!	makefile for vselp
$!
$
$ RM := "delete"
$ 
$! General cc/gcc options
$!-----------------------------
$! normal mode with SPPACK headers
$!  CC_OPT:= "/def=(""DFLOAT_TYPE=float"",""SPPACK"")"
$! normal mode, no headers
$!  CC_OPT:= "/def=(""FLOAT_TYPE=float"")"
$! for pc data format
$!  CC_OPT:= "/def=(""FLOAT_TYPE=float"",""INTEL_FORMAT=1"")"
$! YOUR choice:
$ CC_OPT:= "/def=(""FLOAT_TYPE=float"")"
$!
$! General link options
$!-----------------------------
$ link_opt := ""
$ 
$! Choose a C compiler
$! > this is for Vax-C
$ cc := "cc /include=[-.utl]"
$!
$! > these are for GNU-C
$! CC := "gcc -I../utl"
$! libs := ",GNU_CC:[000000]GCCLIB/LIB,SYS$LIBRARY:VAXCRTL/LIB"
$!
$! Object file dependency strings
$!----------------------------------
$ VSELP_OBJ:= "getp.obj,calcp.obj,init.obj,filt4.obj,flatv.obj,interp.obj,"-
	"makec.obj,pconv.obj,t_sub.obj,filters.obj,lag.obj,p_ex.obj,"-
	"decorr.obj,v_srch.obj,b_con.obj,v_con.obj,rs_rr.obj,g_quant.obj,"-
	"excite.obj,wsnr.obj,r_sub.obj,cunpack0.obj,putc.obj,fspace.obj"
$!
$! Label processing
$!---------------------
$ if ("''p1'".eqs."")
$ then
$  goto uvselp
$ else
$  goto 'p1'
$ endif
$!
$! Make options
$!--------------------------
$!
$ all:
$ uvselp: !vselp.h vparams.h edef.i 'VSELP_OBJ' vselp.obj ugstdemo.h
$ goto here ! to avoid unnecessary compilings when debugging
$ here:
$       'CC' 'CC_OPT' getp.c
$	'CC' 'CC_OPT' calcp.c
$	'CC' 'CC_OPT' init.c
$	'CC' 'CC_OPT' filt4.c
$	'CC' 'CC_OPT' flatv.c
$	'CC' 'CC_OPT' interp.c
$	'CC' 'CC_OPT' makec.c
$	'CC' 'CC_OPT' pconv.c
$	'CC' 'CC_OPT' t_sub.c
$	'CC' 'CC_OPT' filters.c
$	'CC' 'CC_OPT' lag.c
$	'CC' 'CC_OPT' p_ex.c
$	'CC' 'CC_OPT' decorr.c
$	'CC' 'CC_OPT' v_srch.c
$	'CC' 'CC_OPT' b_con.c
$	'CC' 'CC_OPT' v_con.c
$	'CC' 'CC_OPT' rs_rr.c
$	'CC' 'CC_OPT' g_quant.c
$	'CC' 'CC_OPT' excite.c
$	'CC' 'CC_OPT' wsnr.c
$	'CC' 'CC_OPT' r_sub.c
$	'CC' 'CC_OPT' cunpack0.c
$	'CC' 'CC_OPT' putc.c
$	'CC' 'CC_OPT' fspace.c
$	'CC' 'CC_OPT' vselp.c
$ 	link 'LINK_OPT' /exe=uvselp 'VSELP_OBJ',vselp.obj'LIBS'
$	verb uvselp uvselp
$ exit
$!
$ clean:
$	goto clear
$ exit
$!
$ clear:
$ 	'RM' *.obj.*
$ exit
