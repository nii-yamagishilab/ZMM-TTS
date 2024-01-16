# -----------------------------------------------------------------------------
#	MS Visual C makefile for compiling and testing the G.727 implementation
#	The executable must be defined by variable G727 below. 
#	31.Jan.2000 - Implemented by <simao.campos@labs.comsat.com>
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose compiler. Sun: use cc. HP: gotta use cl
# ------------------------------------------------
CC=cl
CC_OPT=-I../utl -W1

# ------------------------------------------------
# Choose an archiving utility: 
#       - public domain unzip, or [PC/MS Visual C/VMS]
#       - shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip -d
UNZIP = unzip -o

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
# set TV_DIR to tstvector/ugst for big-endian systems and
#  to tstvector/ugst/pc for little-endian systems.
TV_DIR=tstvector/ugst/pc
G727=g727demo -q
DIFF=cf -q
RM=rm -f

# ------------------------------------------------
# Implicit rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# File lists
# ------------------------------------------------
CLEAN_LST = speech44.iad speech44.out speech44.ouk
REF_FILES= speech44.ais speech44.acs speech.a-s

# ------------------------------------------------
# Targets
# ------------------------------------------------
all: g727demo discard

anyway: clean all

clean:
	$(RM) *.obj

cleantest: 
	$(RM) $(CLEAN_LST) $(REF_FILES)

clean-tv:
	$(RM) prc/*.* 

veryclean: clean cleantest clean-tv
	$(RM) g727demo.exe discard.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
g711.obj:	../g711/g711.c
	$(CC) -c $(CC_OPT) -I ../g711 ../g711/g711.c

g727demo: g727demo.exe
g727demo.exe: g727.obj g711.obj g727demo.obj
	$(CC) -o g727demo g727demo.obj g711.obj g727.obj

discard: discard.exe
discard.exe: discard.obj
	$(CC) -o discard discard.obj

# -----------------------------------------------------------------------------
# Simple test for g727demo with one speech file
# -----------------------------------------------------------------------------
test: proc comp 

proc: speech.a-s
	$(G727) -core 4 -enh 0 -enc -law A speech.a-s   speech44.iad
	$(G727) -core 4 -enh 0 -dec -law A speech44.iad speech44.out
	$(G727) -core 4 -enh 0 -e_d -law A speech.a-s   speech44.ouk

comp: speech44.ais
	$(DIFF) speech44.iad speech44.ais
	$(DIFF) speech44.out speech44.acs
	$(DIFF) speech44.out speech44.ouk

speech44.ais: speech.a-s
speech.a-s: tst-g727.zip
	$(UNZIP) tst-g727.zip $(REF_FILES)
	swapover $(REF_FILES)

# -----------------------------------------------------------------------------
# Test the implementation for g727demo with test vectors
# -----------------------------------------------------------------------------
test-tv: proc-tv comp-tv 

proc-tv: proc-tv-40k proc-tv-32k proc-tv-24k proc-tv-16k 
comp-tv: comp-tv-40k comp-tv-32k comp-tv-24k comp-tv-16k 

proc-tv-40k:
	$(G727) -core 4 -enh 1 -enc -law u $(TV_DIR)/ovr.m prc/rv54_m.iad
	$(G727) -core 4 -enh 1 -enc -law u $(TV_DIR)/nrm.m prc/rn54_m.iad
	$(G727) -core 4 -enh 1 -enc -law A $(TV_DIR)/ovr.a prc/rv54_a.iad
	$(G727) -core 4 -enh 1 -enc -law A $(TV_DIR)/nrm.a prc/rn54_a.iad
	$(G727) -core 4 -enh 1 -dec -law u $(TV_DIR)/rv54_m.i prc/rv54_m.out
	$(G727) -core 4 -enh 1 -dec -law a $(TV_DIR)/rv54_a.i prc/rv54_a.out
	$(G727) -core 4 -enh 1 -dec -law u $(TV_DIR)/rn54_m.i prc/rn54_m.out
	$(G727) -core 4 -enh 1 -dec -law a $(TV_DIR)/rn54_a.i prc/rn54_a.out
	$(G727) -core 4 -enh 1 -dec -law u $(TV_DIR)/i40 prc/ri54_m.out
	$(G727) -core 4 -enh 1 -dec -law a $(TV_DIR)/i40 prc/ri54_a.out
	$(G727) -core 4 -enh 1 -dec -law A $(TV_DIR)/rv54_m.i prc/rv54_c.out
	$(G727) -core 4 -enh 1 -dec -law u $(TV_DIR)/rv54_a.i prc/rv54_x.out
	$(G727) -core 4 -enh 1 -dec -law A $(TV_DIR)/rn54_m.i prc/rn54_c.out
	$(G727) -core 4 -enh 1 -dec -law u $(TV_DIR)/rn54_a.i prc/rn54_x.out
	$(G727) -core 3 -enh 2 -enc -law u $(TV_DIR)/ovr.m prc/rv53_m.iad
	$(G727) -core 3 -enh 2 -enc -law u $(TV_DIR)/nrm.m prc/rn53_m.iad
	$(G727) -core 3 -enh 2 -enc -law A $(TV_DIR)/ovr.a prc/rv53_a.iad
	$(G727) -core 3 -enh 2 -enc -law A $(TV_DIR)/nrm.a prc/rn53_a.iad
	$(G727) -core 3 -enh 2 -dec -law u $(TV_DIR)/rv53_m.i prc/rv53_m.out
	$(G727) -core 3 -enh 2 -dec -law a $(TV_DIR)/rv53_a.i prc/rv53_a.out
	$(G727) -core 3 -enh 2 -dec -law u $(TV_DIR)/rn53_m.i prc/rn53_m.out
	$(G727) -core 3 -enh 2 -dec -law a $(TV_DIR)/rn53_a.i prc/rn53_a.out
	$(G727) -core 3 -enh 2 -dec -law u $(TV_DIR)/i40 prc/ri53_m.out
	$(G727) -core 3 -enh 2 -dec -law a $(TV_DIR)/i40 prc/ri53_a.out
	$(G727) -core 3 -enh 2 -dec -law A $(TV_DIR)/rv53_m.i prc/rv53_c.out
	$(G727) -core 3 -enh 2 -dec -law u $(TV_DIR)/rv53_a.i prc/rv53_x.out
	$(G727) -core 3 -enh 2 -dec -law A $(TV_DIR)/rn53_m.i prc/rn53_c.out
	$(G727) -core 3 -enh 2 -dec -law u $(TV_DIR)/rn53_a.i prc/rn53_x.out
	$(G727) -core 2 -enh 3 -enc -law u $(TV_DIR)/ovr.m prc/rv52_m.iad
	$(G727) -core 2 -enh 3 -enc -law u $(TV_DIR)/nrm.m prc/rn52_m.iad
	$(G727) -core 2 -enh 3 -enc -law A $(TV_DIR)/ovr.a prc/rv52_a.iad
	$(G727) -core 2 -enh 3 -enc -law A $(TV_DIR)/nrm.a prc/rn52_a.iad
	$(G727) -core 2 -enh 3 -dec -law u $(TV_DIR)/rv52_m.i prc/rv52_m.out
	$(G727) -core 2 -enh 3 -dec -law a $(TV_DIR)/rv52_a.i prc/rv52_a.out
	$(G727) -core 2 -enh 3 -dec -law u $(TV_DIR)/rn52_m.i prc/rn52_m.out
	$(G727) -core 2 -enh 3 -dec -law a $(TV_DIR)/rn52_a.i prc/rn52_a.out
	$(G727) -core 2 -enh 3 -dec -law u $(TV_DIR)/i40 prc/ri52_m.out
	$(G727) -core 2 -enh 3 -dec -law a $(TV_DIR)/i40 prc/ri52_a.out
	$(G727) -core 2 -enh 3 -dec -law A $(TV_DIR)/rv52_m.i prc/rv52_c.out
	$(G727) -core 2 -enh 3 -dec -law u $(TV_DIR)/rv52_a.i prc/rv52_x.out
	$(G727) -core 2 -enh 3 -dec -law A $(TV_DIR)/rn52_m.i prc/rn52_c.out
	$(G727) -core 2 -enh 3 -dec -law u $(TV_DIR)/rn52_a.i prc/rn52_x.out

proc-tv-32k:
	$(G727) -core 4 -enh 0 -enc -law u $(TV_DIR)/ovr.m prc/rv44_m.iad
	$(G727) -core 4 -enh 0 -enc -law u $(TV_DIR)/nrm.m prc/rn44_m.iad
	$(G727) -core 4 -enh 0 -enc -law A $(TV_DIR)/ovr.a prc/rv44_a.iad
	$(G727) -core 4 -enh 0 -enc -law A $(TV_DIR)/nrm.a prc/rn44_a.iad
	$(G727) -core 4 -enh 0 -dec -law u $(TV_DIR)/rv44_m.i prc/rv44_m.out
	$(G727) -core 4 -enh 0 -dec -law a $(TV_DIR)/rv44_a.i prc/rv44_a.out
	$(G727) -core 4 -enh 0 -dec -law u $(TV_DIR)/rn44_m.i prc/rn44_m.out
	$(G727) -core 4 -enh 0 -dec -law a $(TV_DIR)/rn44_a.i prc/rn44_a.out
	$(G727) -core 4 -enh 0 -dec -law u $(TV_DIR)/i32 prc/ri44_m.out
	$(G727) -core 4 -enh 0 -dec -law A $(TV_DIR)/i32 prc/ri44_a.out
	$(G727) -core 4 -enh 0 -dec -law A $(TV_DIR)/rv44_m.i prc/rv44_c.out
	$(G727) -core 4 -enh 0 -dec -law u $(TV_DIR)/rv44_a.i prc/rv44_x.out
	$(G727) -core 4 -enh 0 -dec -law A $(TV_DIR)/rn44_m.i prc/rn44_c.out
	$(G727) -core 4 -enh 0 -dec -law u $(TV_DIR)/rn44_a.i prc/rn44_x.out
	$(G727) -core 3 -enh 1 -enc -law u $(TV_DIR)/ovr.m prc/rv43_m.iad
	$(G727) -core 3 -enh 1 -enc -law u $(TV_DIR)/nrm.m prc/rn43_m.iad
	$(G727) -core 3 -enh 1 -enc -law A $(TV_DIR)/ovr.a prc/rv43_a.iad
	$(G727) -core 3 -enh 1 -enc -law A $(TV_DIR)/nrm.a prc/rn43_a.iad
	$(G727) -core 3 -enh 1 -dec -law u $(TV_DIR)/rv43_m.i prc/rv43_m.out
	$(G727) -core 3 -enh 1 -dec -law a $(TV_DIR)/rv43_a.i prc/rv43_a.out
	$(G727) -core 3 -enh 1 -dec -law u $(TV_DIR)/rn43_m.i prc/rn43_m.out
	$(G727) -core 3 -enh 1 -dec -law a $(TV_DIR)/rn43_a.i prc/rn43_a.out
	$(G727) -core 3 -enh 1 -dec -law u $(TV_DIR)/i32 prc/ri43_m.out
	$(G727) -core 3 -enh 1 -dec -law a $(TV_DIR)/i32 prc/ri43_a.out
	$(G727) -core 3 -enh 1 -dec -law A $(TV_DIR)/rv43_m.i prc/rv43_c.out
	$(G727) -core 3 -enh 1 -dec -law u $(TV_DIR)/rv43_a.i prc/rv43_x.out
	$(G727) -core 3 -enh 1 -dec -law A $(TV_DIR)/rn43_m.i prc/rn43_c.out
	$(G727) -core 3 -enh 1 -dec -law u $(TV_DIR)/rn43_a.i prc/rn43_x.out
	$(G727) -core 2 -enh 2 -enc -law u $(TV_DIR)/ovr.m prc/rv42_m.iad
	$(G727) -core 2 -enh 2 -enc -law u $(TV_DIR)/nrm.m prc/rn42_m.iad
	$(G727) -core 2 -enh 2 -enc -law A $(TV_DIR)/ovr.a prc/rv42_a.iad
	$(G727) -core 2 -enh 2 -enc -law A $(TV_DIR)/nrm.a prc/rn42_a.iad
	$(G727) -core 2 -enh 2 -dec -law u $(TV_DIR)/rv42_m.i prc/rv42_m.out
	$(G727) -core 2 -enh 2 -dec -law a $(TV_DIR)/rv42_a.i prc/rv42_a.out
	$(G727) -core 2 -enh 2 -dec -law u $(TV_DIR)/rn42_m.i prc/rn42_m.out
	$(G727) -core 2 -enh 2 -dec -law a $(TV_DIR)/rn42_a.i prc/rn42_a.out
	$(G727) -core 2 -enh 2 -dec -law u $(TV_DIR)/i32 prc/ri42_m.out
	$(G727) -core 2 -enh 2 -dec -law a $(TV_DIR)/i32 prc/ri42_a.out
	$(G727) -core 2 -enh 2 -dec -law A $(TV_DIR)/rv42_m.i prc/rv42_c.out
	$(G727) -core 2 -enh 2 -dec -law u $(TV_DIR)/rv42_a.i prc/rv42_x.out
	$(G727) -core 2 -enh 2 -dec -law A $(TV_DIR)/rn42_m.i prc/rn42_c.out
	$(G727) -core 2 -enh 2 -dec -law u $(TV_DIR)/rn42_a.i prc/rn42_x.out

proc-tv-24k:
	$(G727) -core 3 -enh 0 -enc -law u $(TV_DIR)/ovr.m prc/rv33_m.iad
	$(G727) -core 3 -enh 0 -enc -law u $(TV_DIR)/nrm.m prc/rn33_m.iad
	$(G727) -core 3 -enh 0 -enc -law A $(TV_DIR)/ovr.a prc/rv33_a.iad
	$(G727) -core 3 -enh 0 -enc -law A $(TV_DIR)/nrm.a prc/rn33_a.iad
	$(G727) -core 3 -enh 0 -dec -law u $(TV_DIR)/rv33_m.i prc/rv33_m.out
	$(G727) -core 3 -enh 0 -dec -law a $(TV_DIR)/rv33_a.i prc/rv33_a.out
	$(G727) -core 3 -enh 0 -dec -law u $(TV_DIR)/rn33_m.i prc/rn33_m.out
	$(G727) -core 3 -enh 0 -dec -law a $(TV_DIR)/rn33_a.i prc/rn33_a.out
	$(G727) -core 3 -enh 0 -dec -law u $(TV_DIR)/i24 prc/ri33_m.out
	$(G727) -core 3 -enh 0 -dec -law a $(TV_DIR)/i24 prc/ri33_a.out
	$(G727) -core 3 -enh 0 -dec -law A $(TV_DIR)/rv33_m.i prc/rv33_c.out
	$(G727) -core 3 -enh 0 -dec -law u $(TV_DIR)/rv33_a.i prc/rv33_x.out
	$(G727) -core 3 -enh 0 -dec -law A $(TV_DIR)/rn33_m.i prc/rn33_c.out
	$(G727) -core 3 -enh 0 -dec -law u $(TV_DIR)/rn33_a.i prc/rn33_x.out
	$(G727) -core 2 -enh 1 -enc -law u $(TV_DIR)/ovr.m prc/rv32_m.iad
	$(G727) -core 2 -enh 1 -enc -law u $(TV_DIR)/nrm.m prc/rn32_m.iad
	$(G727) -core 2 -enh 1 -enc -law A $(TV_DIR)/ovr.a prc/rv32_a.iad
	$(G727) -core 2 -enh 1 -enc -law A $(TV_DIR)/nrm.a prc/rn32_a.iad
	$(G727) -core 2 -enh 1 -dec -law u $(TV_DIR)/rv32_m.i prc/rv32_m.out
	$(G727) -core 2 -enh 1 -dec -law a $(TV_DIR)/rv32_a.i prc/rv32_a.out
	$(G727) -core 2 -enh 1 -dec -law u $(TV_DIR)/rn32_m.i prc/rn32_m.out
	$(G727) -core 2 -enh 1 -dec -law a $(TV_DIR)/rn32_a.i prc/rn32_a.out
	$(G727) -core 2 -enh 1 -dec -law u $(TV_DIR)/i24 prc/ri32_m.out
	$(G727) -core 2 -enh 1 -dec -law a $(TV_DIR)/i24 prc/ri32_a.out
	$(G727) -core 2 -enh 1 -dec -law A $(TV_DIR)/rv32_m.i prc/rv32_c.out
	$(G727) -core 2 -enh 1 -dec -law u $(TV_DIR)/rv32_a.i prc/rv32_x.out
	$(G727) -core 2 -enh 1 -dec -law A $(TV_DIR)/rn32_m.i prc/rn32_c.out
	$(G727) -core 2 -enh 1 -dec -law u $(TV_DIR)/rn32_a.i prc/rn32_x.out

proc-tv-16k:
	$(G727) -core 2 -enh 0 -enc -law u $(TV_DIR)/ovr.m prc/rv22_m.iad
	$(G727) -core 2 -enh 0 -enc -law u $(TV_DIR)/nrm.m prc/rn22_m.iad
	$(G727) -core 2 -enh 0 -enc -law A $(TV_DIR)/ovr.a prc/rv22_a.iad
	$(G727) -core 2 -enh 0 -enc -law A $(TV_DIR)/nrm.a prc/rn22_a.iad
	$(G727) -core 2 -enh 0 -dec -law u $(TV_DIR)/rv22_m.i prc/rv22_m.out
	$(G727) -core 2 -enh 0 -dec -law a $(TV_DIR)/rv22_a.i prc/rv22_a.out
	$(G727) -core 2 -enh 0 -dec -law u $(TV_DIR)/rn22_m.i prc/rn22_m.out
	$(G727) -core 2 -enh 0 -dec -law a $(TV_DIR)/rn22_a.i prc/rn22_a.out
	$(G727) -core 2 -enh 0 -dec -law u $(TV_DIR)/i16 prc/ri22_m.out
	$(G727) -core 2 -enh 0 -dec -law a $(TV_DIR)/i16 prc/ri22_a.out
	$(G727) -core 2 -enh 0 -dec -law A $(TV_DIR)/rv22_m.i prc/rv22_c.out
	$(G727) -core 2 -enh 0 -dec -law u $(TV_DIR)/rv22_a.i prc/rv22_x.out
	$(G727) -core 2 -enh 0 -dec -law A $(TV_DIR)/rn22_m.i prc/rn22_c.out
	$(G727) -core 2 -enh 0 -dec -law u $(TV_DIR)/rn22_a.i prc/rn22_x.out

comp-tv-40k:
	$(DIFF) $(TV_DIR)/rn54_x.o prc/rn54_x.out
	$(DIFF) $(TV_DIR)/rn54_m.o prc/rn54_m.out
	$(DIFF) $(TV_DIR)/rn54_m.i prc/rn54_m.iad
	$(DIFF) $(TV_DIR)/rn54_c.o prc/rn54_c.out
	$(DIFF) $(TV_DIR)/rn54_a.o prc/rn54_a.out
	$(DIFF) $(TV_DIR)/rn54_a.i prc/rn54_a.iad
	$(DIFF) $(TV_DIR)/rn53_x.o prc/rn53_x.out
	$(DIFF) $(TV_DIR)/rn53_m.o prc/rn53_m.out
	$(DIFF) $(TV_DIR)/rn53_m.i prc/rn53_m.iad
	$(DIFF) $(TV_DIR)/rn53_c.o prc/rn53_c.out
	$(DIFF) $(TV_DIR)/rn53_a.o prc/rn53_a.out
	$(DIFF) $(TV_DIR)/rn53_a.i prc/rn53_a.iad
	$(DIFF) $(TV_DIR)/rn52_x.o prc/rn52_x.out
	$(DIFF) $(TV_DIR)/rn52_m.o prc/rn52_m.out
	$(DIFF) $(TV_DIR)/rn52_m.i prc/rn52_m.iad
	$(DIFF) $(TV_DIR)/rn52_c.o prc/rn52_c.out
	$(DIFF) $(TV_DIR)/rn52_a.o prc/rn52_a.out
	$(DIFF) $(TV_DIR)/rn52_a.i prc/rn52_a.iad
	$(DIFF) $(TV_DIR)/rv54_x.o prc/rv54_x.out
	$(DIFF) $(TV_DIR)/rv54_m.o prc/rv54_m.out
	$(DIFF) $(TV_DIR)/rv54_m.i prc/rv54_m.iad
	$(DIFF) $(TV_DIR)/rv54_c.o prc/rv54_c.out
	$(DIFF) $(TV_DIR)/rv54_a.o prc/rv54_a.out
	$(DIFF) $(TV_DIR)/rv54_a.i prc/rv54_a.iad
	$(DIFF) $(TV_DIR)/rv53_x.o prc/rv53_x.out
	$(DIFF) $(TV_DIR)/rv53_m.o prc/rv53_m.out
	$(DIFF) $(TV_DIR)/rv53_m.i prc/rv53_m.iad
	$(DIFF) $(TV_DIR)/rv53_c.o prc/rv53_c.out
	$(DIFF) $(TV_DIR)/rv53_a.o prc/rv53_a.out
	$(DIFF) $(TV_DIR)/rv53_a.i prc/rv53_a.iad
	$(DIFF) $(TV_DIR)/rv52_x.o prc/rv52_x.out
	$(DIFF) $(TV_DIR)/rv52_m.o prc/rv52_m.out
	$(DIFF) $(TV_DIR)/rv52_m.i prc/rv52_m.iad
	$(DIFF) $(TV_DIR)/rv52_c.o prc/rv52_c.out
	$(DIFF) $(TV_DIR)/rv52_a.o prc/rv52_a.out
	$(DIFF) $(TV_DIR)/rv52_a.i prc/rv52_a.iad
	$(DIFF) $(TV_DIR)/ri54_m.o prc/ri54_m.out
	$(DIFF) $(TV_DIR)/ri54_a.o prc/ri54_a.out
	$(DIFF) $(TV_DIR)/ri53_m.o prc/ri53_m.out
	$(DIFF) $(TV_DIR)/ri53_a.o prc/ri53_a.out
	$(DIFF) $(TV_DIR)/ri52_m.o prc/ri52_m.out
	$(DIFF) $(TV_DIR)/ri52_a.o prc/ri52_a.out

comp-tv-32k:
	$(DIFF) $(TV_DIR)/rn44_x.o prc/rn44_x.out
	$(DIFF) $(TV_DIR)/rn44_m.o prc/rn44_m.out
	$(DIFF) $(TV_DIR)/rn44_m.i prc/rn44_m.iad
	$(DIFF) $(TV_DIR)/rn44_c.o prc/rn44_c.out
	$(DIFF) $(TV_DIR)/rn44_a.o prc/rn44_a.out
	$(DIFF) $(TV_DIR)/rn44_a.i prc/rn44_a.iad
	$(DIFF) $(TV_DIR)/rn43_x.o prc/rn43_x.out
	$(DIFF) $(TV_DIR)/rn43_m.o prc/rn43_m.out
	$(DIFF) $(TV_DIR)/rn43_m.i prc/rn43_m.iad
	$(DIFF) $(TV_DIR)/rn43_c.o prc/rn43_c.out
	$(DIFF) $(TV_DIR)/rn43_a.o prc/rn43_a.out
	$(DIFF) $(TV_DIR)/rn43_a.i prc/rn43_a.iad
	$(DIFF) $(TV_DIR)/rn42_x.o prc/rn42_x.out
	$(DIFF) $(TV_DIR)/rn42_m.o prc/rn42_m.out
	$(DIFF) $(TV_DIR)/rn42_m.i prc/rn42_m.iad
	$(DIFF) $(TV_DIR)/rn42_c.o prc/rn42_c.out
	$(DIFF) $(TV_DIR)/rn42_a.o prc/rn42_a.out
	$(DIFF) $(TV_DIR)/rn42_a.i prc/rn42_a.iad
	$(DIFF) $(TV_DIR)/rv44_x.o prc/rv44_x.out
	$(DIFF) $(TV_DIR)/rv44_m.o prc/rv44_m.out
	$(DIFF) $(TV_DIR)/rv44_m.i prc/rv44_m.iad
	$(DIFF) $(TV_DIR)/rv44_c.o prc/rv44_c.out
	$(DIFF) $(TV_DIR)/rv44_a.o prc/rv44_a.out
	$(DIFF) $(TV_DIR)/rv44_a.i prc/rv44_a.iad
	$(DIFF) $(TV_DIR)/rv43_x.o prc/rv43_x.out
	$(DIFF) $(TV_DIR)/rv43_m.o prc/rv43_m.out
	$(DIFF) $(TV_DIR)/rv43_m.i prc/rv43_m.iad
	$(DIFF) $(TV_DIR)/rv43_c.o prc/rv43_c.out
	$(DIFF) $(TV_DIR)/rv43_a.o prc/rv43_a.out
	$(DIFF) $(TV_DIR)/rv43_a.i prc/rv43_a.iad
	$(DIFF) $(TV_DIR)/rv42_x.o prc/rv42_x.out
	$(DIFF) $(TV_DIR)/rv42_m.o prc/rv42_m.out
	$(DIFF) $(TV_DIR)/rv42_m.i prc/rv42_m.iad
	$(DIFF) $(TV_DIR)/rv42_c.o prc/rv42_c.out
	$(DIFF) $(TV_DIR)/rv42_a.o prc/rv42_a.out
	$(DIFF) $(TV_DIR)/rv42_a.i prc/rv42_a.iad
	$(DIFF) $(TV_DIR)/ri44_m.o prc/ri44_m.out
	$(DIFF) $(TV_DIR)/ri44_a.o prc/ri44_a.out
	$(DIFF) $(TV_DIR)/ri43_m.o prc/ri43_m.out
	$(DIFF) $(TV_DIR)/ri43_a.o prc/ri43_a.out
	$(DIFF) $(TV_DIR)/ri42_m.o prc/ri42_m.out
	$(DIFF) $(TV_DIR)/ri42_a.o prc/ri42_a.out
	$(DIFF) $(TV_DIR)/ri33_m.o prc/ri33_m.out
	$(DIFF) $(TV_DIR)/ri33_a.o prc/ri33_a.out
	$(DIFF) $(TV_DIR)/ri32_m.o prc/ri32_m.out
	$(DIFF) $(TV_DIR)/ri32_a.o prc/ri32_a.out

comp-tv-24k:
	$(DIFF) $(TV_DIR)/rv33_x.o prc/rv33_x.out
	$(DIFF) $(TV_DIR)/rv33_m.o prc/rv33_m.out
	$(DIFF) $(TV_DIR)/rv33_m.i prc/rv33_m.iad
	$(DIFF) $(TV_DIR)/rv33_c.o prc/rv33_c.out
	$(DIFF) $(TV_DIR)/rv33_a.o prc/rv33_a.out
	$(DIFF) $(TV_DIR)/rv33_a.i prc/rv33_a.iad
	$(DIFF) $(TV_DIR)/rv32_x.o prc/rv32_x.out
	$(DIFF) $(TV_DIR)/rv32_m.o prc/rv32_m.out
	$(DIFF) $(TV_DIR)/rv32_m.i prc/rv32_m.iad
	$(DIFF) $(TV_DIR)/rv32_c.o prc/rv32_c.out
	$(DIFF) $(TV_DIR)/rv32_a.o prc/rv32_a.out
	$(DIFF) $(TV_DIR)/rv32_a.i prc/rv32_a.iad
	$(DIFF) $(TV_DIR)/rn33_x.o prc/rn33_x.out
	$(DIFF) $(TV_DIR)/rn33_m.o prc/rn33_m.out
	$(DIFF) $(TV_DIR)/rn33_m.i prc/rn33_m.iad
	$(DIFF) $(TV_DIR)/rn33_c.o prc/rn33_c.out
	$(DIFF) $(TV_DIR)/rn33_a.o prc/rn33_a.out
	$(DIFF) $(TV_DIR)/rn33_a.i prc/rn33_a.iad
	$(DIFF) $(TV_DIR)/rn32_x.o prc/rn32_x.out
	$(DIFF) $(TV_DIR)/rn32_m.o prc/rn32_m.out
	$(DIFF) $(TV_DIR)/rn32_m.i prc/rn32_m.iad
	$(DIFF) $(TV_DIR)/rn32_c.o prc/rn32_c.out
	$(DIFF) $(TV_DIR)/rn32_a.o prc/rn32_a.out
	$(DIFF) $(TV_DIR)/rn32_a.i prc/rn32_a.iad

comp-tv-16k:
	$(DIFF) $(TV_DIR)/rn22_x.o prc/rn22_x.out
	$(DIFF) $(TV_DIR)/rn22_m.o prc/rn22_m.out
	$(DIFF) $(TV_DIR)/rn22_m.i prc/rn22_m.iad
	$(DIFF) $(TV_DIR)/rn22_c.o prc/rn22_c.out
	$(DIFF) $(TV_DIR)/rn22_a.o prc/rn22_a.out
	$(DIFF) $(TV_DIR)/rn22_a.i prc/rn22_a.iad
	$(DIFF) $(TV_DIR)/ri22_m.o prc/ri22_m.out
	$(DIFF) $(TV_DIR)/ri22_a.o prc/ri22_a.out
	$(DIFF) $(TV_DIR)/rv22_x.o prc/rv22_x.out
	$(DIFF) $(TV_DIR)/rv22_m.o prc/rv22_m.out
	$(DIFF) $(TV_DIR)/rv22_m.i prc/rv22_m.iad
	$(DIFF) $(TV_DIR)/rv22_c.o prc/rv22_c.out
	$(DIFF) $(TV_DIR)/rv22_a.o prc/rv22_a.out
	$(DIFF) $(TV_DIR)/rv22_a.i prc/rv22_a.iad

#: Test encoder+decoder ("codec") operation with test vectors
test-codec-tv: proc-codec-tv comp-codec-tv

proc-codec-tv:
	$(G727) -core 2 -enh 0 -e_d -law A $(TV_DIR)/nrm.a prc/rn22_a.o-c
	$(G727) -core 2 -enh 0 -e_d -law A $(TV_DIR)/ovr.a prc/rv22_a.o-c
	$(G727) -core 2 -enh 0 -e_d -law u $(TV_DIR)/nrm.m prc/rn22_m.o-c
	$(G727) -core 2 -enh 0 -e_d -law u $(TV_DIR)/ovr.m prc/rv22_m.o-c
	$(G727) -core 2 -enh 1 -e_d -law A $(TV_DIR)/nrm.a prc/rn32_a.o-c
	$(G727) -core 2 -enh 1 -e_d -law A $(TV_DIR)/ovr.a prc/rv32_a.o-c
	$(G727) -core 2 -enh 1 -e_d -law u $(TV_DIR)/nrm.m prc/rn32_m.o-c
	$(G727) -core 2 -enh 1 -e_d -law u $(TV_DIR)/ovr.m prc/rv32_m.o-c
	$(G727) -core 2 -enh 2 -e_d -law A $(TV_DIR)/nrm.a prc/rn42_a.o-c
	$(G727) -core 2 -enh 2 -e_d -law A $(TV_DIR)/ovr.a prc/rv42_a.o-c
	$(G727) -core 2 -enh 2 -e_d -law u $(TV_DIR)/nrm.m prc/rn42_m.o-c
	$(G727) -core 2 -enh 2 -e_d -law u $(TV_DIR)/ovr.m prc/rv42_m.o-c
	$(G727) -core 2 -enh 3 -e_d -law A $(TV_DIR)/nrm.a prc/rn52_a.o-c
	$(G727) -core 2 -enh 3 -e_d -law A $(TV_DIR)/ovr.a prc/rv52_a.o-c
	$(G727) -core 2 -enh 3 -e_d -law u $(TV_DIR)/nrm.m prc/rn52_m.o-c
	$(G727) -core 2 -enh 3 -e_d -law u $(TV_DIR)/ovr.m prc/rv52_m.o-c
	$(G727) -core 3 -enh 0 -e_d -law A $(TV_DIR)/nrm.a prc/rn33_a.o-c
	$(G727) -core 3 -enh 0 -e_d -law A $(TV_DIR)/ovr.a prc/rv33_a.o-c
	$(G727) -core 3 -enh 0 -e_d -law u $(TV_DIR)/nrm.m prc/rn33_m.o-c
	$(G727) -core 3 -enh 0 -e_d -law u $(TV_DIR)/ovr.m prc/rv33_m.o-c
	$(G727) -core 3 -enh 1 -e_d -law A $(TV_DIR)/nrm.a prc/rn43_a.o-c
	$(G727) -core 3 -enh 1 -e_d -law A $(TV_DIR)/ovr.a prc/rv43_a.o-c
	$(G727) -core 3 -enh 1 -e_d -law u $(TV_DIR)/nrm.m prc/rn43_m.o-c
	$(G727) -core 3 -enh 1 -e_d -law u $(TV_DIR)/ovr.m prc/rv43_m.o-c
	$(G727) -core 3 -enh 2 -e_d -law A $(TV_DIR)/nrm.a prc/rn53_a.o-c
	$(G727) -core 3 -enh 2 -e_d -law A $(TV_DIR)/ovr.a prc/rv53_a.o-c
	$(G727) -core 3 -enh 2 -e_d -law u $(TV_DIR)/nrm.m prc/rn53_m.o-c
	$(G727) -core 3 -enh 2 -e_d -law u $(TV_DIR)/ovr.m prc/rv53_m.o-c
	$(G727) -core 4 -enh 0 -e_d -law A $(TV_DIR)/nrm.a prc/rn44_a.o-c
	$(G727) -core 4 -enh 0 -e_d -law A $(TV_DIR)/ovr.a prc/rv44_a.o-c
	$(G727) -core 4 -enh 0 -e_d -law u $(TV_DIR)/nrm.m prc/rn44_m.o-c
	$(G727) -core 4 -enh 0 -e_d -law u $(TV_DIR)/ovr.m prc/rv44_m.o-c
	$(G727) -core 4 -enh 1 -e_d -law A $(TV_DIR)/nrm.a prc/rn54_a.o-c
	$(G727) -core 4 -enh 1 -e_d -law A $(TV_DIR)/ovr.a prc/rv54_a.o-c
	$(G727) -core 4 -enh 1 -e_d -law u $(TV_DIR)/nrm.m prc/rn54_m.o-c
	$(G727) -core 4 -enh 1 -e_d -law u $(TV_DIR)/ovr.m prc/rv54_m.o-c

comp-codec-tv:
	$(DIFF) $(TV_DIR)/rn22_a.o prc/rn22_a.o-c
	$(DIFF) $(TV_DIR)/rn22_m.o prc/rn22_m.o-c
	$(DIFF) $(TV_DIR)/rn32_a.o prc/rn32_a.o-c
	$(DIFF) $(TV_DIR)/rn32_m.o prc/rn32_m.o-c
	$(DIFF) $(TV_DIR)/rn33_a.o prc/rn33_a.o-c
	$(DIFF) $(TV_DIR)/rn33_m.o prc/rn33_m.o-c
	$(DIFF) $(TV_DIR)/rn42_a.o prc/rn42_a.o-c
	$(DIFF) $(TV_DIR)/rn42_m.o prc/rn42_m.o-c
	$(DIFF) $(TV_DIR)/rn43_a.o prc/rn43_a.o-c
	$(DIFF) $(TV_DIR)/rn43_m.o prc/rn43_m.o-c
	$(DIFF) $(TV_DIR)/rn44_a.o prc/rn44_a.o-c
	$(DIFF) $(TV_DIR)/rn44_m.o prc/rn44_m.o-c
	$(DIFF) $(TV_DIR)/rn52_a.o prc/rn52_a.o-c
	$(DIFF) $(TV_DIR)/rn52_m.o prc/rn52_m.o-c
	$(DIFF) $(TV_DIR)/rn53_a.o prc/rn53_a.o-c
	$(DIFF) $(TV_DIR)/rn53_m.o prc/rn53_m.o-c
	$(DIFF) $(TV_DIR)/rn54_a.o prc/rn54_a.o-c
	$(DIFF) $(TV_DIR)/rn54_m.o prc/rn54_m.o-c
	$(DIFF) $(TV_DIR)/rv22_a.o prc/rv22_a.o-c
	$(DIFF) $(TV_DIR)/rv22_m.o prc/rv22_m.o-c
	$(DIFF) $(TV_DIR)/rv32_a.o prc/rv32_a.o-c
	$(DIFF) $(TV_DIR)/rv32_m.o prc/rv32_m.o-c
	$(DIFF) $(TV_DIR)/rv33_a.o prc/rv33_a.o-c
	$(DIFF) $(TV_DIR)/rv33_m.o prc/rv33_m.o-c
	$(DIFF) $(TV_DIR)/rv42_a.o prc/rv42_a.o-c
	$(DIFF) $(TV_DIR)/rv42_m.o prc/rv42_m.o-c
	$(DIFF) $(TV_DIR)/rv43_a.o prc/rv43_a.o-c
	$(DIFF) $(TV_DIR)/rv43_m.o prc/rv43_m.o-c
	$(DIFF) $(TV_DIR)/rv44_a.o prc/rv44_a.o-c
	$(DIFF) $(TV_DIR)/rv44_m.o prc/rv44_m.o-c
	$(DIFF) $(TV_DIR)/rv52_a.o prc/rv52_a.o-c
	$(DIFF) $(TV_DIR)/rv52_m.o prc/rv52_m.o-c
	$(DIFF) $(TV_DIR)/rv53_a.o prc/rv53_a.o-c
	$(DIFF) $(TV_DIR)/rv53_m.o prc/rv53_m.o-c
	$(DIFF) $(TV_DIR)/rv54_a.o prc/rv54_a.o-c
	$(DIFF) $(TV_DIR)/rv54_m.o prc/rv54_m.o-c

