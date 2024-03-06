#-----------------------------------------------------------------------------
# MS Visual C makefile for compiling and testing the G.726
# implementation. 
# The executable must be defined by variable G726 below.
# History:
# 31.Jan.2000 - Implemented by <simao.campos@labs.comsat.com>
# NOTE: you need to get (purchase) the G.726 test vectors from the ITU in 
#       order to perform the (optional) compliance test.
#-----------------------------------------------------------------------------

# ------------------------------------------------
# Choose compiler. 
# ------------------------------------------------
CC=cl
CC_OPT = -I../utl /Yd /DEBUG

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
G726 = g726demo
VBR = vbr-g726 -q
DIFF = cf -q
RM=rm -f

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = pkunzip 
UNZIP = unzip -o

# ------------------------------------------------
# File lists
# ------------------------------------------------
TEST_VECTORS =  *.?16 *.?24 *.?32 *.?40 *.rec

DEMO_OBJ = g726demo.obj g726.obj
VBR_OBJ = vbr-g726.obj g726.obj g711.obj

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: g726demo vbr-g726

anyway: clean all

clean:
	$(RM) *.obj 

cleantest:
	$(RM) $(TEST_VECTORS) voicvbra.tst voicvbru.tst voicvbrl.tst
	$(RM) voice.src voicevbr.arf voicevbr.lrf voicevbr.urf

veryclean: clean cleantest
	$(RM) g726demo.exe vbr-g726.exe

# -----------------------------------------------------------------------------
# Generic rules
# -----------------------------------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# -----------------------------------------------------------------------------
# Specific rules
# -----------------------------------------------------------------------------
vbr-g726: vbr-g726.exe
g726demo: g726demo.exe
vbr-g726.exe: $(VBR_OBJ)
	$(CC) -o vbr-g726 $(CC_OPT) $(VBR_OBJ) 

g726demo.exe: $(DEMO_OBJ)
	$(CC) -o g726demo $(CC_OPT) $(DEMO_OBJ) 

g726demo.obj: g726demo.c
	$(CC) -c $(CC_OPT) -I../g711 g726demo.c

g726.obj:	g726.c
	$(CC) -c $(CC_OPT) g726.c

g711.obj:	../g711/g711.c
	$(CC) -c $(CC_OPT) -I../g711 ../g711/g711.c

vbr-g726.obj: vbr-g726.c
	$(CC) -c $(CC_OPT) -I../g711 vbr-g726.c

# ----------------------------------------
#  Very simple portability test
# ----------------------------------------
test: test-vbr-quick
proc: proc-vbr-quick
comp: comp-vbr-quick

test-vbr-quick: proc-vbr-quick comp-vbr-quick
proc-vbr-quick: voice.src
	$(VBR) -q -law A -rate 16-24-32-40-32-24 voice.src voicvbra.tst
	$(VBR) -q -law l -rate 16-24-32-40-32-24 voice.src voicvbrl.tst
	$(VBR) -q -law u -rate 16-24-32-40-32-24 voice.src voicvbru.tst
comp-vbr-quick: voicevbr.arf
	$(DIFF) voicvbra.tst voicevbr.arf
	$(DIFF) voicvbrl.tst voicevbr.lrf
	$(DIFF) voicvbru.tst voicevbr.urf

voice.src: tst-g726.zip
	$(UNZIP) tst-g726.zip voice.src
	sb -over voice.src

voicevbr.arf: tst-g726.zip
	$(UNZIP) tst-g726.zip voicevbr.arf voicevbr.lrf voicevbr.urf
	swapover voicevbr.arf voicevbr.lrf voicevbr.urf

# -----------------------------------------------------------------------------
# Test the implementation for g726demo (compliance)
# In the automatic compliance testing, g726demo.c is verified by test-tv
# For vbr-g726.c, no differences should be observed, since both use g726.c, 
# and differ only on the demo logic. For full compliance testing of the 
# vbr-g726.c program, use test-tv-vbr
# -----------------------------------------------------------------------------
test-tv: proc-tv-fix comp-tv
proc-tv-fix: bin/rn16fa.o clean proc16-fix proc24-fix proc32-fix proc40-fix
comp-tv-fix: comp-tv
comp-tv: comp16 comp24 comp32 comp40 

proc16-fix:
#
# Process ADPCM/ Coder for normal and overload sequences, A law
#
	$(G726) a load 16 bin/nrm.a nrm.a16 256 1 64
	$(G726) a load 16 bin/ovr.a ovr.a16 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, A law
#
	$(G726) a adlo 16 bin/rn16fa.i rn16fa.rec 256 1 64
	$(G726) a adlo 16 bin/rv16fa.i rv16fa.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM 16kbit/s -> mu law output
#
	$(G726) u adlo 16 bin/rn16fa.i rn16fx.rec 256 1 64
	$(G726) u adlo 16 bin/rv16fa.i rv16fx.rec 256 1 8
#
#
# Process ADPCM/ Coder for normal and overload sequences, mu law
#
	$(G726) u load 16 bin/nrm.m nrm.m16 256 1 64
	$(G726) u load 16 bin/ovr.m ovr.m16 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(G726) u adlo 16 bin/rn16fm.i rn16fm.rec 256 1 64
	$(G726) u adlo 16 bin/rv16fm.i rv16fm.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 16kbit/s -> A law output
#
	$(G726) a adlo 16 bin/rn16fm.i rn16fc.rec 256 1 64
	$(G726) a adlo 16 bin/rv16fm.i rv16fc.rec 256 1 8
#
#
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(G726) a adlo 16 bin/i16 ri16fa.rec 256 1 64
	$(G726) u adlo 16 bin/i16 ri16fm.rec 256 1 64

comp16:
#
# =================================================================
#  COMPARISON OF FILES !
# =================================================================
#
# Compare ADPCM/ Coder for normal and overload sequences, A law
#
	$(DIFF) bin/rn16fa.i nrm.a16 256 1 64 
	$(DIFF) bin/rv16fa.i ovr.a16 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, A law
#
	$(DIFF) bin/rn16fa.o rn16fa.rec 256 1 64 
	$(DIFF) bin/rv16fa.o rv16fa.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM x kbit/s -> mu law output
#
	$(DIFF) bin/rn16fx.o rn16fx.rec 256 1 64 
	$(DIFF) bin/rv16fx.o rv16fx.rec 256 1 8 
#
#
# Compare ADPCM/ Coder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn16fm.i nrm.m16 256 1 64 
	$(DIFF) bin/rv16fm.i ovr.m16 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn16fm.o rn16fm.rec 256 1 64 
	$(DIFF) bin/rv16fm.o rv16fm.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM x kbit/s -> A law output
#
	$(DIFF) bin/rn16fc.o rn16fc.rec 256 1 64 
	$(DIFF) bin/rv16fc.o rv16fc.rec 256 1 8 
#
#
# Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(DIFF) bin/ri16fa.o ri16fa.rec 256 1 64 
	$(DIFF) bin/ri16fm.o ri16fm.rec 256 1 64 

proc24-fix:
#
#
# Process ADPCM/ Coder for normal and overload sequences, A law
#
	$(G726) a load 24 bin/nrm.a nrm.a24 256 1 64
	$(G726) a load 24 bin/ovr.a ovr.a24 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, A law
#
	$(G726) a adlo 24 bin/rn24fa.i rn24fa.rec 256 1 64
	$(G726) a adlo 24 bin/rv24fa.i rv24fa.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM 24kbit/s -> mu law output
#
	$(G726) u adlo 24 bin/rn24fa.i rn24fx.rec 256 1 64
	$(G726) u adlo 24 bin/rv24fa.i rv24fx.rec 256 1 8
#
#
# Process ADPCM/ Coder for normal and overload sequences, mu law
#
	$(G726) u load 24 bin/nrm.m nrm.m24 256 1 64
	$(G726) u load 24 bin/ovr.m ovr.m24 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(G726) u adlo 24 bin/rn24fm.i rn24fm.rec 256 1 64
	$(G726) u adlo 24 bin/rv24fm.i rv24fm.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 24kbit/s -> A law output
#
	$(G726) a adlo 24 bin/rn24fm.i rn24fc.rec 256 1 64
	$(G726) a adlo 24 bin/rv24fm.i rv24fc.rec 256 1 8
#
#
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(G726) a adlo 24 bin/i24 ri24fa.rec 256 1 64
	$(G726) u adlo 24 bin/i24 ri24fm.rec 256 1 64

comp24:
#
# =================================================================
#  COMPARISON OF FILES !
# =================================================================
#
# Compare ADPCM/ Coder for normal and overload sequences, A law
#
	$(DIFF) bin/rn24fa.i nrm.a24 256 1 64 
	$(DIFF) bin/rv24fa.i ovr.a24 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, A law
#
	$(DIFF) bin/rn24fa.o rn24fa.rec 256 1 64 
	$(DIFF) bin/rv24fa.o rv24fa.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM x kbit/s -> mu law output
#
	$(DIFF) bin/rn24fx.o rn24fx.rec 256 1 64 
	$(DIFF) bin/rv24fx.o rv24fx.rec 256 1 8 
#
#
# Compare ADPCM/ Coder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn24fm.i nrm.m24 256 1 64 
	$(DIFF) bin/rv24fm.i ovr.m24 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn24fm.o rn24fm.rec 256 1 64 
	$(DIFF) bin/rv24fm.o rv24fm.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM x kbit/s -> A law output
#
	$(DIFF) bin/rn24fc.o rn24fc.rec 256 1 64 
	$(DIFF) bin/rv24fc.o rv24fc.rec 256 1 8 
#
#
# Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(DIFF) bin/ri24fa.o ri24fa.rec 256 1 64 
	$(DIFF) bin/ri24fm.o ri24fm.rec 256 1 64 

proc32-fix:
#
# Process ADPCM/ Coder for normal and overload sequences, A law
#
	$(G726) a load 32 bin/nrm.a nrm.a32 256 1 64
	$(G726) a load 32 bin/ovr.a ovr.a32 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, A law
#
	$(G726) a adlo 32 bin/rn32fa.i rn32fa.rec 256 1 64
	$(G726) a adlo 32 bin/rv32fa.i rv32fa.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM 32kbit/s -> mu law output
#
	$(G726) u adlo 32 bin/rn32fa.i rn32fx.rec 256 1 64
	$(G726) u adlo 32 bin/rv32fa.i rv32fx.rec 256 1 8
#
#
# Process ADPCM/ Coder for normal and overload sequences, mu law
#
	$(G726) u load 32 bin/nrm.m nrm.m32 256 1 64
	$(G726) u load 32 bin/ovr.m ovr.m32 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(G726) u adlo 32 bin/rn32fm.i rn32fm.rec 256 1 64
	$(G726) u adlo 32 bin/rv32fm.i rv32fm.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 32kbit/s -> A law output
#
	$(G726) a adlo 32 bin/rn32fm.i rn32fc.rec 256 1 64
	$(G726) a adlo 32 bin/rv32fm.i rv32fc.rec 256 1 8
#
#
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(G726) a adlo 32 bin/i32 ri32fa.rec 256 1 64
	$(G726) u adlo 32 bin/i32 ri32fm.rec 256 1 64

comp32:
#
# =================================================================
#  COMPARISON OF FILES !
# =================================================================
#
# Compare ADPCM/ Coder for normal and overload sequences, A law
#
	$(DIFF) bin/rn32fa.i nrm.a32 256 1 64 
	$(DIFF) bin/rv32fa.i ovr.a32 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, A law
#
	$(DIFF) bin/rn32fa.o rn32fa.rec 256 1 64 
	$(DIFF) bin/rv32fa.o rv32fa.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM x kbit/s -> mu law output
#
	$(DIFF) bin/rn32fx.o rn32fx.rec 256 1 64 
	$(DIFF) bin/rv32fx.o rv32fx.rec 256 1 8 
#
#
# Compare ADPCM/ Coder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn32fm.i nrm.m32 256 1 64 
	$(DIFF) bin/rv32fm.i ovr.m32 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn32fm.o rn32fm.rec 256 1 64 
	$(DIFF) bin/rv32fm.o rv32fm.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM x kbit/s -> A law output
#
	$(DIFF) bin/rn32fc.o rn32fc.rec 256 1 64 
	$(DIFF) bin/rv32fc.o rv32fc.rec 256 1 8 
#
#
# Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(DIFF) bin/ri32fa.o ri32fa.rec 256 1 64 
	$(DIFF) bin/ri32fm.o ri32fm.rec 256 1 64 

proc40-fix:
#
#
# Process ADPCM/ Coder for normal and overload sequences, A law
#
	$(G726) a load 40 bin/nrm.a nrm.a40 256 1 64
	$(G726) a load 40 bin/ovr.a ovr.a40 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, A law
#
	$(G726) a adlo 40 bin/rn40fa.i rn40fa.rec 256 1 64
	$(G726) a adlo 40 bin/rv40fa.i rv40fa.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM 40kbit/s -> mu law output
#
	$(G726) u adlo 40 bin/rn40fa.i rn40fx.rec 256 1 64
	$(G726) u adlo 40 bin/rv40fa.i rv40fx.rec 256 1 8
#
#
# Process ADPCM/ Coder for normal and overload sequences, mu law
#
	$(G726) u load 40 bin/nrm.m nrm.m40 256 1 64
	$(G726) u load 40 bin/ovr.m ovr.m40 256 1 8
#
# Process ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(G726) u adlo 40 bin/rn40fm.i rn40fm.rec 256 1 64
	$(G726) u adlo 40 bin/rv40fm.i rv40fm.rec 256 1 8
#
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 40kbit/s -> A law output
#
	$(G726) a adlo 40 bin/rn40fm.i rn40fc.rec 256 1 64
	$(G726) a adlo 40 bin/rv40fm.i rv40fc.rec 256 1 8
#
#
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(G726) a adlo 40 bin/i40 ri40fa.rec 256 1 64
	$(G726) u adlo 40 bin/i40 ri40fm.rec 256 1 64

comp40:
#
# =================================================================
#  COMPARISON OF FILES !
# =================================================================
#
# Compare ADPCM/ Coder for normal and overload sequences, A law
#
	$(DIFF) bin/rn40fa.i nrm.a40 256 1 64 
	$(DIFF) bin/rv40fa.i ovr.a40 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, A law
#
	$(DIFF) bin/rn40fa.o rn40fa.rec 256 1 64 
	$(DIFF) bin/rv40fa.o rv40fa.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                A law input -> ADPCM x kbit/s -> mu law output
#
	$(DIFF) bin/rn40fx.o rn40fx.rec 256 1 64 
	$(DIFF) bin/rv40fx.o rv40fx.rec 256 1 8 
#
#
# Compare ADPCM/ Coder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn40fm.i nrm.m40 256 1 64 
	$(DIFF) bin/rv40fm.i ovr.m40 256 1 8 
#
# Compare ADPCM/ Decoder for normal and overload sequences, mu law
#
	$(DIFF) bin/rn40fm.o rn40fm.rec 256 1 64 
	$(DIFF) bin/rv40fm.o rv40fm.rec 256 1 8 
#
# Compare ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM x kbit/s -> A law output
#
	$(DIFF) bin/rn40fc.o rn40fc.rec 256 1 64 
	$(DIFF) bin/rv40fc.o rv40fc.rec 256 1 8 
#
#
# Compare ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
#
	$(DIFF) bin/ri40fa.o ri40fa.rec 256 1 64 
	$(DIFF) bin/ri40fm.o ri40fm.rec 256 1 64 

#
# ----------------------------------------------------------------------------
# Test the implementation for vbr-g726 (compliance)
# ----------------------------------------------------------------------------
test-tv-vbr: proc-tv-vbr comp-tv
proc-tv-vbr: bin/rn16fa.o clean proc16-vbr proc24-vbr proc32-vbr proc40-vbr
comp-vbr-tv: comp-tv

proc16-vbr:
# Process ADPCM/ Coder for normal and overload sequences, A law
	$(VBR) -law a -enc -rate 16 bin/nrm.a nrm.a16 16 1 1024
	$(VBR) -law a -enc -rate 16 bin/ovr.a ovr.a16 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, A law
	$(VBR) -law a -dec -rate 16 bin/rn16fa.i rn16fa.rec 16 1 1024
	$(VBR) -law a -dec -rate 16 bin/rv16fa.i rv16fa.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#               A law input -> ADPCM 16kbit/s -> mu law output
	$(VBR) -law u -dec -rate 16 bin/rn16fa.i rn16fx.rec 16 1 1024
	$(VBR) -law u -dec -rate 16 bin/rv16fa.i rv16fx.rec 16 1 128
# Process ADPCM/ Coder for normal and overload sequences, mu law
	$(VBR) -law u -enc -rate 16 bin/nrm.m nrm.m16 16 1 1024
	$(VBR) -law u -enc -rate 16 bin/ovr.m ovr.m16 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, mu law
	$(VBR) -law u -dec -rate 16 bin/rn16fm.i rn16fm.rec 16 1 1024
	$(VBR) -law u -dec -rate 16 bin/rv16fm.i rv16fm.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 16kbit/s ->A law output
	$(VBR) -law a -dec -rate 16 bin/rn16fm.i rn16fc.rec 16 1 1024
	$(VBR) -law a -dec -rate 16 bin/rv16fm.i rv16fc.rec 16 1 128
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
	$(VBR) -law a -dec -rate 16 bin/i16 ri16fa.rec 16 1 1024
	$(VBR) -law u -dec -rate 16 bin/i16 ri16fm.rec 16 1 1024

proc24-vbr:
# Process ADPCM/ Coder for normal and overload sequences, A law
	$(VBR) -law a -enc -rate 24 bin/nrm.a nrm.a24 16 1 1024
	$(VBR) -law a -enc -rate 24 bin/ovr.a ovr.a24 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, A law
	$(VBR) -law a -dec -rate 24 bin/rn24fa.i rn24fa.rec 16 1 1024
	$(VBR) -law a -dec -rate 24 bin/rv24fa.i rv24fa.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#               A law input -> ADPCM 24kbit/s -> mu law output
	$(VBR) -law u -dec -rate 24 bin/rn24fa.i rn24fx.rec 16 1 1024
	$(VBR) -law u -dec -rate 24 bin/rv24fa.i rv24fx.rec 16 1 128
# Process ADPCM/ Coder for normal and overload sequences, mu law
	$(VBR) -law u -enc -rate 24 bin/nrm.m nrm.m24 16 1 1024
	$(VBR) -law u -enc -rate 24 bin/ovr.m ovr.m24 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, mu law
	$(VBR) -law u -dec -rate 24 bin/rn24fm.i rn24fm.rec 16 1 1024
	$(VBR) -law u -dec -rate 24 bin/rv24fm.i rv24fm.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 24kbit/s ->A law output
	$(VBR) -law a -dec -rate 24 bin/rn24fm.i rn24fc.rec 16 1 1024
	$(VBR) -law a -dec -rate 24 bin/rv24fm.i rv24fc.rec 16 1 128
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
	$(VBR) -law a -dec -rate 24 bin/i24 ri24fa.rec 16 1 1024
	$(VBR) -law u -dec -rate 24 bin/i24 ri24fm.rec 16 1 1024

proc32-vbr:
# Process ADPCM/ Coder for normal and overload sequences, A law
	$(VBR) -law a -enc -rate 32 bin/nrm.a nrm.a32 16 1 1024
	$(VBR) -law a -enc -rate 32 bin/ovr.a ovr.a32 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, A law
	$(VBR) -law a -dec -rate 32 bin/rn32fa.i rn32fa.rec 16 1 1024
	$(VBR) -law a -dec -rate 32 bin/rv32fa.i rv32fa.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#               A law input -> ADPCM 32kbit/s -> mu law output
	$(VBR) -law u -dec -rate 32 bin/rn32fa.i rn32fx.rec 16 1 1024
	$(VBR) -law u -dec -rate 32 bin/rv32fa.i rv32fx.rec 16 1 128
# Process ADPCM/ Coder for normal and overload sequences, mu law
	$(VBR) -law u -enc -rate 32 bin/nrm.m nrm.m32 16 1 1024
	$(VBR) -law u -enc -rate 32 bin/ovr.m ovr.m32 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, mu law
	$(VBR) -law u -dec -rate 32 bin/rn32fm.i rn32fm.rec 16 1 1024
	$(VBR) -law u -dec -rate 32 bin/rv32fm.i rv32fm.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 32kbit/s ->A law output
	$(VBR) -law a -dec -rate 32 bin/rn32fm.i rn32fc.rec 16 1 1024
	$(VBR) -law a -dec -rate 32 bin/rv32fm.i rv32fc.rec 16 1 128
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
	$(VBR) -law a -dec -rate 32 bin/i32 ri32fa.rec 16 1 1024
	$(VBR) -law u -dec -rate 32 bin/i32 ri32fm.rec 16 1 1024

proc40-vbr:
# Process ADPCM/ Coder for normal and overload sequences, A law
	$(VBR) -law a -enc -rate 40 bin/nrm.a nrm.a40 16 1 1024
	$(VBR) -law a -enc -rate 40 bin/ovr.a ovr.a40 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, A law
	$(VBR) -law a -dec -rate 40 bin/rn40fa.i rn40fa.rec 16 1 1024
	$(VBR) -law a -dec -rate 40 bin/rv40fa.i rv40fa.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#               A law input -> ADPCM 40kbit/s -> mu law output
	$(VBR) -law u -dec -rate 40 bin/rn40fa.i rn40fx.rec 16 1 1024
	$(VBR) -law u -dec -rate 40 bin/rv40fa.i rv40fx.rec 16 1 128
# Process ADPCM/ Coder for normal and overload sequences, mu law
	$(VBR) -law u -enc -rate 40 bin/nrm.m nrm.m40 16 1 1024
	$(VBR) -law u -enc -rate 40 bin/ovr.m ovr.m40 16 1 128
# Process ADPCM/ Decoder for normal and overload sequences, mu law
	$(VBR) -law u -dec -rate 40 bin/rn40fm.i rn40fm.rec 16 1 1024
	$(VBR) -law u -dec -rate 40 bin/rv40fm.i rv40fm.rec 16 1 128
# Process ADPCM/ Cross-decoder for normal and overload sequences, 
#                mu law input -> ADPCM 40kbit/s ->A law output
	$(VBR) -law a -dec -rate 40 bin/rn40fm.i rn40fc.rec 16 1 1024
	$(VBR) -law a -dec -rate 40 bin/rv40fm.i rv40fc.rec 16 1 128
# Process ADPCM/ Decode-only for the same input ADPCM sequence, A and mu law
	$(VBR) -law a -dec -rate 40 bin/i40 ri40fa.rec 16 1 1024
	$(VBR) -law u -dec -rate 40 bin/i40 ri40fm.rec 16 1 1024

# =========================================
# Process by batch
# =========================================
batchtest:
	batch test-g726

batchtest-vbr:
	echo make -f makefile.unx test-vbr | batch
