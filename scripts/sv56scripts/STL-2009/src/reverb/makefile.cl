# -----------------------------------------------------------------------------
#	MS Visual C makefile for compiling and testing the UGST Reverb module 
#       implementation
#	Implemented by <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com> -- 06.Apr.05
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose compiler.
# ------------------------------------------------
CC = cl
CC_OPT = $(XXX) -I../utl -W1

# ------------------------------------------------
# Executables
# ------------------------------------------------
RM = rm -f
REVERB_EXE= reverb

# ------------------------------------------------
# Choose an archiving utility: 
#       - public domain unzip, or [PC/MS Visual C/VMS]
#       - shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose a file comparison utility: 
#       - cf compares, 
#       - sub shows the difference of the different samples
# Default is cf.
# ------------------------------------------------
DIFF = cf -q
#DIFF = sub

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all: reverb

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) *.ref *.IR *.src *.tst

veryclean: clean cleantest
	$(RM) reverb.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
reverb: reverb.exe
reverb.exe: reverb-lib.obj reverb.obj
	$(CC) -o reverb reverb-lib.obj reverb.obj

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
reverb-lib.obj:  reverb-lib.c reverb-lib.h
reverb.obj:      reverb.c reverb-lib.c reverb-lib.h

# ------------------------------------------------
# Testing portability
# Note: there are no compliance test vectors associated with this module
# ------------------------------------------------

test: proc comp

proc: input.src irtest_le.IR
# test with little-endian IR file
	$(REVERB_EXE) input.src irtest_le.IR output.tst
# test with big-endian IR file
#	$(REVERB_EXE) input.src irtest_be.IR output.tst

comp: output.ref
	$(DIFF) output.ref output.tst
	
irtest_le.IR:
# little-endian file
	$(UNZIP) test-rev.zip irtest_le.IR
# big-endian file
#	$(UNZIP) test-rev.zip irtest_be.IR

input.src:
	$(UNZIP) test-rev.zip input.src
	sb -over input.src

output.ref:
	$(UNZIP) test-rev.zip output.ref
	sb -over output.ref
