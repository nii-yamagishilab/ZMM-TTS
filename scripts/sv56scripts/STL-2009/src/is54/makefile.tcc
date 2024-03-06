#**************************************************************************
#
#               (C) Copyright 1990, Motorola Inc., U.S.A.
#
# Note:  Reproduction and use for the development of North American digital
#        cellular standards or development of digital speech coding
#        standards within the International Telecommunications Union -
#        Telecommunications Standardization Sector is authorized by Motorola 
#        Inc.  No other use is intended or authorized.
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
# -- *NOTE*: --
# Makefile for [bt]cc created by <simao@ctd.comsat.com> for the 
# UGST distribution
#
#**************************************************************************
#
#	makefile for vselp using Borland [bt]cc compiler
#

RM = rm -f
INC = -I../utl #-Ic:\bc\include
#LIB = -Lc:\bc\lib
TABLE = "c:/users/simao/is54/k_table.i"
BASIS = "c:/users/simao/is54/basis.i"

# normal mode with SPPACK headers
#CC_OPT= -DFLOAT_TYPE=float -O2 -DSPPACK
# normal mode, no headers
#CC_OPT= -DFLOAT_TYPE=float -O2
# for pc data format
#CC_OPT= -DFLOAT_TYPE=float -DINTEL_FORMAT=1
# YOUR choice:
CC_OPT= -DFLOAT_TYPE=float $(INC) $(LIB)

# Choose a C compiler
CC = tcc

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
all: uvselp

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
uvselp.exe: vselp.h vparams.h edef.i $(VSELP_OBJ) vselp.obj uvselp.cfg
	$(CC) $(CC_OPT) -euvselp @uvselp.cfg vselp.obj 

getp.obj: getp.c vparams.h
	$(CC) $(CC_OPT) -c -ogetp.obj getp.c

calcp.obj: calcp.c vparams.h
	$(CC) $(CC_OPT) -c -ocalcp.obj calcp.c

init.obj: init.c vparams.h k_table.i basis.i gray.i init.cfg 
	$(CC) @init.cfg -c init.c 

init.cfg: makefile.tcc
  copy &&|
$(INC)
$(LIB)
-DBASIS_FILE=$(BASIS);K_TABLE_FILE=$(TABLE);FLOAT_TYPE=float
| init.cfg

filt4.obj: filt4.c vparams.h
	$(CC) $(CC_OPT) -c -ofilt4.obj filt4.c

filters.obj: filters.c vparams.h
	$(CC) $(CC_OPT) -c -ofilters.obj filters.c

flatv.obj: flatv.c vparams.h
	$(CC) $(CC_OPT) -c -oflatv.obj flatv.c 

interp.obj: interp.c vparams.h
	$(CC) $(CC_OPT) -c -ointerp.obj interp.c 

makec.obj: makec.c vparams.h
	$(CC) $(CC_OPT) -c -omakec.obj makec.c

pconv.obj: pconv.c vparams.h
	$(CC) $(CC_OPT) -c -opconv.obj pconv.c

lag.obj: lag.c vparams.h
	$(CC) $(CC_OPT) -c -olag.obj lag.c

p_ex.obj: p_ex.c vparams.h
	$(CC) $(CC_OPT) -c -op_ex.obj p_ex.c

decorr.obj: decorr.c vparams.h
	$(CC) $(CC_OPT) -c -odecorr.obj decorr.c

v_srch.obj: v_srch.c vparams.h
	$(CC) $(CC_OPT) -c -ov_srch.obj v_srch.c

b_con.obj: b_con.c vparams.h
	$(CC) $(CC_OPT) -c -ob_con.obj b_con.c

v_con.obj: v_con.c vparams.h
	$(CC) $(CC_OPT) -c -ov_con.obj v_con.c

rs_rr.obj: rs_rr.c vparams.h
	$(CC) $(CC_OPT) -c -ors_rr.obj rs_rr.c 

g_quant.obj: g_quant.c vparams.h
	$(CC) $(CC_OPT) -c -og_quant.obj g_quant.c

excite.obj: excite.c vparams.h
	$(CC) $(CC_OPT) -c -oexcite.obj excite.c

wsnr.obj: wsnr.c vparams.h
	$(CC) $(CC_OPT) -c -owsnr.obj wsnr.c 

t_sub.obj: vparams.h t_sub.h t_sub.c
	$(CC) $(CC_OPT) -c -ot_sub.obj t_sub.c

r_sub.obj: vparams.h r_sub.h r_sub.c
	$(CC) $(CC_OPT) -c -or_sub.obj r_sub.c 

cunpack0.obj: cunpack0.c
	$(CC) $(CC_OPT) -c -ocunpack0.obj cunpack0.c

putc.obj: vparams.h putc.c
	$(CC) $(CC_OPT) -c -oputc.obj putc.c

fspace.obj: vparams.h fspace.c
	$(CC) $(CC_OPT) -c -ofspace.obj fspace.c

vselp.obj: vparams.h vselp.c
	$(CC) $(CC_OPT) -c vselp.c

# ------------------------------------------------
# Borland make-specific
# ------------------------------------------------
uvselp.cfg: makefile.tcc
  copy &&|
$(INC)
$(LIB)
$(VSELP_OBJ)
| uvselp.cfg

