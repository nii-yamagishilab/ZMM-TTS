# -----------------------------------------------------------------------------
# MS Visual C makefile for compiling and testing the 
# UGST P.56 speech voltmeter.
# Executable must be defined by SVP56 and ACTLEV below.
#
# Implemented by <simao.campos@labs.comsat.com> -- 01.Dec.99
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose a file comparison utility: 
# ------------------------------------------------
DIFF = cf -q

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$$6~/[0-9]+:[0-9][0-9]/ {print "sb -over",$$NF};END{print "exit"}'

# ------------------------------------------------
# Choose compiler & options
# ------------------------------------------------
CC_OPT = -I../utl -W1

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
RM = rm -f
SVP56 = sv56demo.exe
ACTLEV = actlev.exe
P56_OBJ = sv-p56.obj ugst-utl.obj

#------------------------------------------------------------------
# Implicit rules
#------------------------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

#------------------------------------------------------------------
# Targets and general rules
#------------------------------------------------------------------
all:: sv56demo actlev

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) voice.nrm voice.ltl voice.prc voice.rms voice.src

veryclean: clean cleantest
	$(RM) $(SVP56) $(ACTLEV)

#------------------------------------------------------------------
# Specific rules
#------------------------------------------------------------------
sv56demo: sv56demo.obj $(P56_OBJ)
	$(CC) $(CC_OPT) -o sv56demo sv56demo.obj $(P56_OBJ) -lm

actlev:	actlevel.obj $(P56_OBJ)
	$(CC) $(CC_OPT) -o actlev actlevel.obj $(P56_OBJ) -lm

ugst-utl.obj: ../utl/ugst-utl.c
	$(CC) $(CC_OPT) -c ../utl/ugst-utl.c

#------------------------------------------------------------------
# Test the implementation
# Note: there are no compliance test vectors associated with this module
#------------------------------------------------------------------
test:   proc comp

proc:	voice.src voice.nrm
# Process by the speech voltmeter, use active speech level
	$(SVP56) -q voice.src voice.prc 256 1 0 -30
# Process by the speech voltmeter, use RMS, long-term speech level
	$(SVP56) -q -rms voice.src voice.rms 256 1 0 -30
# Measure the new file
	$(ACTLEV) -q voice.src voice.nrm voice.prc voice.ltl voice.rms

comp:
# Compare with reference - no differences should be found.
	$(DIFF) voice.nrm voice.prc
	$(DIFF) voice.ltl voice.rms

#-----------------------------------------------
# Unpack and byte-swap if necessary for testing 
#-----------------------------------------------
voice.nrm:	sv56-tst.zip
	$(UNZIP) sv56-tst.zip voice.nrm voice.ltl
	swapover voice.nrm voice.ltl
#	$(UNZIP) -v sv56-tst.zip voice.nrm voice.ltl | $(AWK) $(AWK_CMD) | command

voice.src: ../is54/bin/is54-tst.zip
	$(UNZIP) ../is54/bin/is54-tst.zip voice.src
	swapover voice.src
#	$(UNZIP) -v ../is54/bin/is54-tst.zip voice.src | $(AWK) $(AWK_CMD) | command
