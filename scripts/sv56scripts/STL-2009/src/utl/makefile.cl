# -----------------------------------------------------------------------------
#	MS Visual C compiler makefile for compiling and testing the 
#       utility tool module for the UGST distribution.
#	Define SP as the executable file for spdemo
#	Implemented by <simao@ctd.comsat.com> -- 01.Feb.95
#       Updates:
#       05.Sep.95 - Added instances for scaldemo.c <simao>
# -----------------------------------------------------------------------------

#------------------------------------------------
# Choose compiler and reference/test files
#------------------------------------------------
GCC = gcc -fno-builtin
CC_OPT = -I../utl -W1

#------------------------------------------------
# Define reference/test files
#------------------------------------------------
REF=../sv56/voice.ltl ../is54/bin/voice.src \
    spref-l.p12 spref-l.s12 spref-r.p12 spref-r.s12 spref-s.p16 \
    spref-l.p15 spref-l.s15 spref-r.p15 spref-r.s15 spref-s.s16 \
    spref-l.p16 spref-l.s16 spref-r.p16 spref-r.s16 spref.src
TST=sptst*.* voice.rou voice.tru voice.rnp

#------------------------------------------------
# Define executables
#------------------------------------------------
RUN=
SP = $(RUN) spdemo -q
SCALE = $(RUN) scaldemo -q
RM = -rm -f

#------------------------------------------------
# Choose a file comparison utility: 
#------------------------------------------------
DIFF = cf -q
#DIFF2 = diff # Unix tool
#DIFF2 = fc/b # in MSDOS
DIFF2 = cmp  # Unix tool
SUB = sub # unsupported STL program (see directory ../unsup)

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip -) -- -+ -3 -^
UNZIP = unzip -o

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$$6~/[0-9]+:[0-9][0-9]/ {print "sb -over",$$NF};END{print "exit"}'

#------------------------------------
# General rules
#------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

#------------------------------------
# General Targets
#------------------------------------
all:	spdemo scaldemo

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) $(TST) $(REF)

veryclean: clean cleantest
	$(RM) spdemo.exe scaldemo.exe

#------------------------------------
# Specific rules
#------------------------------------
spdemo:	spdemo.obj ugst-utl.obj ugst-utl.h ugstdemo.h
	$(CC) $(CC_OPT) -o spdemo spdemo.obj ugst-utl.obj 

scaldemo: scaldemo.obj ugst-utl.obj
	$(CC) $(CC_OPT) -o scaldemo scaldemo.obj ugst-utl.obj


#------------------------------------
# Test all
# NOTE: there are no compliance tests associated with this module
#------------------------------------
test: test-scale test-sp
proc: proc-scale proc-sp
comp: comp-scale comp-sp

#------------------------------------
# Test scale() demo program
#------------------------------------
test-scale: proc-scale comp-scale

proc-scale: ../is54/bin/voice.src
	$(SCALE) -trunc ../is54/bin/voice.src voice.tru 256 1 0 0.5941352
	$(SCALE) -premask -round ../is54/bin/voice.src voice.rou 256 1 0 0.5941352
	$(SCALE) -round ../is54/bin/voice.src voice.rnp 256 1 0 0.5941352

comp-scale: ../sv56/voice.ltl
#	For the first comparison, files should be identical
	$(DIFF) voice.tru ../sv56/voice.ltl
#	For the second comparison, files should be equivalent at a +-1 level
	$(SUB) -equiv 1 voice.rou ../sv56/voice.ltl
#	For the third comparison, files should be identical
	$(SUB) -equiv 1 voice.rnp voice.rou

#---------------------------------------------
# Test serial/parallel function demo programs
#---------------------------------------------
test-sp: proc-sp comp-sp

proc-sp: serialize parallelize

serialize: spref.src
#----------------------------------------------------------------------
# Generate serialized files
#----------------------------------------------------------------------
	$(SP) -r 16 -right ps spref.src sptst-r.s16 100
	$(SP) -r 15 -right ps spref.src sptst-r.s15 100
	$(SP) -r 12 -right ps spref.src sptst-r.s12 100
	$(SP) -r 16 -left  ps spref.src sptst-l.s16 100
	$(SP) -r 15 -left  ps spref.src sptst-l.s15 100
	$(SP) -r 12 -left  ps spref.src sptst-l.s12 100
	$(SP) -r 16 -nosync ps spref.src sptst-s.s16 100

parallelize:
#----------------------------------------------------------------------
# Generate parallelized files
#----------------------------------------------------------------------
	$(SP) -r 16 -right sp  sptst-r.s16 sptst-r.p16 100
	$(SP) -r 15 -right sp  sptst-r.s15 sptst-r.p15 100
	$(SP) -r 12 -right sp  sptst-r.s12 sptst-r.p12 100
	$(SP) -r 16 -left  sp  sptst-l.s16 sptst-l.p16 100
	$(SP) -r 15 -left  sp  sptst-l.s15 sptst-l.p15 100
	$(SP) -r 12 -left  sp  sptst-l.s12 sptst-l.p12 100
	$(SP) -r 16 -nosync sp sptst-s.s16 sptst-s.p16 100 

comp-sp: spref-s.s16
#----------------------------------------------------------------------
# 52 samples are different in the least significant nibble (i.e., the
#   4 least significant bits): xxx0/xxxF,xxxE,xxx8, etc.
#----------------------------------------------------------------------
	$(DIFF) sptst-l.p12 spref.src 100
#
#----------------------------------------------------------------------
# 46 samples are different in the least significant bit, i.e.
#   by +1 (sptst-l.p15 + 1 = spref.src)
#----------------------------------------------------------------------
	$(DIFF) sptst-l.p15 spref.src 100
#
#----------------------------------------------------------------------
# 82 samples are different in the most significant bit: 6xxx/Exxx, 
#   7xxx/Fxxx, 0xxx/8xxx
#----------------------------------------------------------------------
	$(DIFF) sptst-r.p15 spref.src 100
#
#----------------------------------------------------------------------
# 82 samples are different in the most significant nibble (i.e., the
#   4 most significant bits): 0xxx/Exxx,Fxxx,8xxx, etc.
#----------------------------------------------------------------------
	$(DIFF) sptst-r.p12 spref.src 100
#
#----------------------------------------------------------------------
# No different samples for these
#----------------------------------------------------------------------
	$(DIFF2) sptst-s.p16 spref.src   
	$(DIFF2) sptst-l.p16 sptst-r.p16 
	$(DIFF2) sptst-l.p16 sptst-s.p16 
	$(DIFF2) spref-l.s12 sptst-l.s12 
	$(DIFF2) spref-r.s12 sptst-r.s12 
	$(DIFF2) spref-l.p12 sptst-l.p12 
	$(DIFF2) spref-r.p12 sptst-r.p12 
	$(DIFF2) spref-l.s15 sptst-l.s15 
	$(DIFF2) spref-r.s15 sptst-r.s15 
	$(DIFF2) spref-l.p15 sptst-l.p15 
	$(DIFF2) spref-r.p15 sptst-r.p15 
	$(DIFF2) spref-l.s16 sptst-l.s16 
	$(DIFF2) spref-r.s16 sptst-r.s16 
	$(DIFF2) spref-s.s16 sptst-s.s16 

#-----------------------------------------------
# Unpack and byte-swap if necessary for testing 
#-----------------------------------------------
spref-s.s16: 
	$(UNZIP) tst-sp.zip spref-*.*
	swapover spref-*.*
#	$(UNZIP) -v tst-sp.zip spref-*.* | $(AWK) $(AWK_CMD) | command

spref.src: 
	$(UNZIP) tst-sp.zip spref.src
	swapover spref.src
#	$(UNZIP) -v tst-sp.zip spref.src | $(AWK) $(AWK_CMD) | command

../sv56/voice.ltl: ../sv56/sv56-tst.zip
	cd ..\sv56
	$(UNZIP) sv56-tst voice.ltl
	swapover voice.ltl
#	$(UNZIP) -v sv56-tst voice.ltl | $(AWK) $(AWK_CMD) | command
	cd ..\utl

../is54/bin/voice.src: ../is54/bin/is54-tst.zip
	-cd ..\is54\bin
	$(UNZIP) is54-tst voice.src
	swapover voice.src
#	$(UNZIP) -v is54-tst voice.src | $(AWK) $(AWK_CMD) | command
	cd ..\..\utl
