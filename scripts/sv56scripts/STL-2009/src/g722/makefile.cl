# -----------------------------------------------------------------------------
# MS C makefile for compiling and testing the G.722
# The executable must be defined by variable below.
# 01.Feb.2002 - Makefile adapted by <simao.campos@labs.comsat.com> from original
#               Makefile provided by AT&T
# 30.Nov.2009 - Modified by yusuke hiwasaki <hiwasaki.yusuke@lab.ntt.co.jp>
#               to add  ../utl in the include path
# 5.Feb.2010 - Modified by j.svedberg 
#               to add  ../eid in the include path for softbit{.h,.c)
#               tested with "nmake /f makefile.cl veryclean all"   
#               cl version 15.00.21022.08 for 80x86 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# Packet Loss Concealment code for G.722
# Copyright (c) 1997-2000 AT&T Corp.
# All rights reserved.
# 
# Makefile for Microsoft Visual C++: nmake -f makefile.cl
.SUFFIXES: .o .c .cc

# ------------------------------------------------
# Choose compiler and options.
# ------------------------------------------------
CC=cl

CFLAGS = -I../basop -I../utl -I../eid

SB_LOC = ../eid

G722ENC_OBJ = funcg722.obj g722.obj ../basop/basop32.obj ../basop/control.obj ../basop/count.obj ../basop/enh1632.obj encg722.obj

G722DEC_OBJ = funcg722.obj g722.obj $(SB_LOC)/softbit.obj ../basop/basop32.obj ../basop/control.obj ../basop/count.obj ../basop/enh1632.obj decg722.obj

G722DEMO_OBJ = funcg722.obj g722.obj $(SB_LOC)/softbit.obj ../basop/basop32.obj ../basop/control.obj ../basop/count.obj ../basop/enh1632.obj g722demo.obj


# ------------------------------------
#  Lists of files
# ------------------------------------

ALL= decg722 encg722  g722demo 

# ------------------------------------------------
# Choose a file comparison utility:
#       - cf compares,
#       - sub shows the difference of the different samples
# Default is cf.
# ------------------------------------------------
DIFF = fc /b
#DIFF = sub
#DIFF_OPT = 256 1 30

# ------------------------------------------------
# Choose an archiving utility: 
#       - public domain unzip, or [PC/MS Visual C/VMS]
#       - shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Generic rules
# ------------------------------------------------

.c.obj:
	$(CC)  $(CFLAGS) -c $<

# ------------------------------------
# Targets
# ------------------------------------
all: $(ALL)

RM = del /f /q
#RM = rm -f
clean:
	$(RM)  *.obj

cleantest:
	$(RM) *.raw
	$(RM) *.g192
	$(RM) *.le

veryclean: clean cleantest
	$(RM) *.exe

# ------------------------------------
# Dependencies and rules
# ------------------------------------
encg722: encg722.exe
encg722.exe: $(G722ENC_OBJ)
	$(CC) $(CFLAGS) -o encg722 funcg722.obj g722.obj basop32.obj control.obj count.obj enh1632.obj encg722.obj  

decg722: decg722.exe
decg722.exe: $(G722DEC_OBJ) 
	$(CC) $(CFLAGS) -o decg722 funcg722.obj g722.obj softbit.obj basop32.obj control.obj count.obj enh1632.obj decg722.obj 

g722demo: g722demo.exe
g722demo.exe: $(G722DEMO_OBJ)
	$(CC) $(CFLAGS) -o g722demo funcg722.obj g722.obj softbit.obj basop32.obj control.obj count.obj enh1632.obj g722demo.obj 
 
