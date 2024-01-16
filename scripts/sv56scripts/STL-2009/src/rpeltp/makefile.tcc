#--------------------------------------------------------------------------
# RPELTP makefile for MSDOS Borland [bt]cc
#--------------------------------------------------------------------------
# Original Copyright message:
# Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
# Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
# details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
#
# File adapted by simao@ctd.comsat.com for the UGST distribution.
#--------------------------------------------------------------------------

######### MUST Define USE_FLOAT_MUL for bit exact performance to GSM Spec.
CC = tcc -mm -DSASR;USE_FLOAT_MUL;NeedFunctionPrototypes 
INC = -I../utl;..\g711;d:\tc\include
LIB = -Ld:\tc\lib

#--------------------------------------------------------------------------
# Tools
#--------------------------------------------------------------------------
RM=rm -f

#--------------------------------------------------------------------------
# Targets
#--------------------------------------------------------------------------
EXE_dependencies =  \
  add.obj \
  code.obj \
  debug.obj \
  decode.obj \
  gsm_crea.obj \
  gsm_deco.obj \
  gsm_dest.obj \
  gsm_enco.obj \
  gsm_expl.obj \
  gsm_impl.obj \
  gsm_opti.obj \
  gsm_prin.obj \
  long_ter.obj \
  lpc.obj \
  preproce.obj \
  rpe.obj \
  rpeltp.obj \
  rpedemo.obj \
  short_te.obj \
  table.obj \
  g711.obj

# ------------------------------------------------
# Implicit rules
# ------------------------------------------------
.c.obj:
  $(CC) -c {$< }

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
anyway:	clean rpedemo.exe

clean:
	$(RM) *.obj

cleantest:
	echo No test file processing available in this directory

veryclean: clean
	$(RM) rpedemo.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
rpedemo: rpedemo.exe
rpedemo.exe: rpedemo.cfg $(EXE_dependencies)
	$(CC) $(CC_OPT) -erpedemo @rpedemo.cfg

add.obj: add.c 
code.obj: code.c 
debug.obj: debug.c 
decode.obj: decode.c 
gsm_crea.obj: gsm_crea.c 
gsm_deco.obj: gsm_deco.c 
gsm_dest.obj: gsm_dest.c 
gsm_enco.obj: gsm_enco.c 
gsm_expl.obj: gsm_expl.c 
gsm_impl.obj: gsm_impl.c 
gsm_opti.obj: gsm_opti.c 
gsm_prin.obj: gsm_prin.c 
long_ter.obj: long_ter.c 
lpc.obj: lpc.c 
preproce.obj: preproce.c 
rpe.obj: rpe.c 
rpeltp.obj: rpeltp.c
rpedemo.obj: rpedemo.c 
	$(CC) -c -I../g711 rpedemo.c
short_te.obj: short_te.c 
table.obj: table.c 
g711.obj: ../g711/g711.c
	$(CC) -c -I../g711 ../g711/g711.c

# ------------------------------------------------
# Borland make-specific
# ------------------------------------------------
rpedemo.cfg: makefile.tcc
  copy &&|
-ff-
-f287
-mm
-DSASR;USE_FLOAT_MUL;NeedFunctionPrototypes
-P-.C
$(INC)
$(LIB)
$(EXE_dependencies)
| rpedemo.cfg
