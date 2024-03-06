#**************************************************************************
#
#               (C) Copyright 1990, Motorola Inc., U.S.A.
#
# Note:  Reproduction and use for the development of North American digital
#        cellular standards or development of digital speech coding
#        standards within the International Telecommunications Union -
#        Telecommunications Standardization Sector is authorized by 
#        Motorola Inc.  No other use is intended or authorized.
#
#        The availability of this material does not provide any license
#        by implication, estoppel, or otherwise under any patent rights
#        of Motorola Inc. or others covering any use of the contents
#        herein.
#
#        Any copies or derivative works must incude this and all other
#        proprietary notices.
#
# Systems Research Laboratories
# Chicago Corporate Research and Development Center
# Motorola Inc.
#
#**************************************************************************
#
#	makefile for vselp using MS Visual C compiler
#

.SUFFIXES: .objut .obj .c .e .r .f .y .l .s .p .objs

RM = rm -f

# Compiler options
CC_OPT= -DFLOAT_TYPE=float -I../utl -W1

# Choose a C compiler
CC = cl

#	object file dependency strings
#
VSELP_OBJ=getp.obj calcp.obj init.obj filt4.obj flatv.obj interp.obj \
	makec.obj pconv.obj t_sub.obj filters.obj lag.obj p_ex.obj decorr.obj v_srch.obj \
	b_con.obj v_con.obj rs_rr.obj g_quant.obj excite.obj wsnr.obj r_sub.obj \
	cunpack0.obj putc.obj fspace.obj


#	make options
#

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: uvselp

anyway: clear uvselp

clean:
	$(RM) *.obj

cleantest:
	echo No test file processing available in this directory

veryclean: clean
	$(RM) uvselp.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
uvselp: uvselp.exe
uvselp.exe: vselp.h vparams.h edef.i $(VSELP_OBJ) vselp.obj
	$(CC) $(CC_OPT) -o $@ $(VSELP_OBJ) vselp.obj 

getp.obj: getp.c vparams.h
	$(CC) $(CC_OPT) -c -o getp.obj getp.c

calcp.obj: calcp.c vparams.h
	$(CC) $(CC_OPT) -c -o calcp.obj calcp.c

init.obj: init.c vparams.h k_table.i basis.i gray.i 
	$(CC) $(CC_OPT) -c -o init.obj init.c 

filt4.obj: filt4.c vparams.h
	$(CC) $(CC_OPT) -c -o filt4.obj filt4.c

filters.obj: filters.c vparams.h
	$(CC) $(CC_OPT) -c -o filters.obj filters.c

flatv.obj: flatv.c vparams.h
	$(CC) $(CC_OPT) -c -o flatv.obj flatv.c 

interp.obj: interp.c vparams.h
	$(CC) $(CC_OPT) -c -o interp.obj interp.c 

makec.obj: makec.c vparams.h
	$(CC) $(CC_OPT) -c -o makec.obj makec.c

pconv.obj: pconv.c vparams.h
	$(CC) $(CC_OPT) -c -o pconv.obj pconv.c

lag.obj: lag.c vparams.h
	$(CC) $(CC_OPT) -c -o lag.obj lag.c

p_ex.obj: p_ex.c vparams.h
	$(CC) $(CC_OPT) -c -o p_ex.obj p_ex.c

decorr.obj: decorr.c vparams.h
	$(CC) $(CC_OPT) -c -o decorr.obj decorr.c

v_srch.obj: v_srch.c vparams.h
	$(CC) $(CC_OPT) -c -o v_srch.obj v_srch.c

b_con.obj: b_con.c vparams.h
	$(CC) $(CC_OPT) -c -o b_con.obj b_con.c

v_con.obj: v_con.c vparams.h
	$(CC) $(CC_OPT) -c -o v_con.obj v_con.c

rs_rr.obj: rs_rr.c vparams.h
	$(CC) $(CC_OPT) -c -o rs_rr.obj rs_rr.c 

g_quant.obj: g_quant.c vparams.h
	$(CC) $(CC_OPT) -c -o g_quant.obj g_quant.c

excite.obj: excite.c vparams.h
	$(CC) $(CC_OPT) -c -o excite.obj excite.c

wsnr.obj: wsnr.c vparams.h
	$(CC) $(CC_OPT) -c -o wsnr.obj wsnr.c 

t_sub.obj: vparams.h t_sub.h t_sub.c
	$(CC) $(CC_OPT) -c -o t_sub.obj t_sub.c

r_sub.obj: vparams.h r_sub.h r_sub.c
	$(CC) $(CC_OPT) -c -o r_sub.obj r_sub.c 

cunpack0.obj: cunpack0.c
	$(CC) $(CC_OPT) -c -o cunpack0.obj cunpack0.c

putc.obj: vparams.h putc.c
	$(CC) $(CC_OPT) -c -o putc.obj putc.c

fspace.obj: vparams.h fspace.c
	$(CC) $(CC_OPT) -c -o fspace.obj fspace.c

vselp.obj: vparams.h vselp.c
	$(CC) $(CC_OPT) -c vselp.c

