# ---------------------------------------------------------------------------------------------------------
#	MS Visual C makefile for compiling and testing the UGST freqresp module 
#       implementation
#	Implemented by <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com> -- 06.Apr.05
# ---------------------------------------------------------------------------------------------------------

# ------------------------------------------------
# Choose compiler.
# ------------------------------------------------
CC = cl
CC_OPT = $(XXX) -I../utl -W3
# ------------------------------------------------
# Executables
# ------------------------------------------------
RM = rm -f
FREQRESP_EXE= freqresp

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
DIFF = fc /b
#DIFF = sub

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all: freqresp

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) *.ref *.src *.tst

veryclean: clean cleantest
	$(RM) freqresp.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
freqresp: freqresp.exe

freqresp.exe: freqresp.obj bmp_utils.obj export.obj fft.obj ugst-utl.obj
	$(CC) -o freqresp freqresp.obj bmp_utils.obj export.obj fft.obj ugst-utl.obj

ugst-utl.obj: ../utl/ugst-utl.c
	$(CC) $(CC_OPT) -c ../utl/ugst-utl.c

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
fft.obj:  fft.c fft.h
bmp_utils.obj: bmp_utils.c bmp_utils.h
export.obj: export.c export.h
freqresp.obj: freqresp.c fft.h bmp_utils.h export.h ../utl/ugstdemo.h ../utl/ugst-utl.h

# ------------------------------------------------
# Testing portability
# Note: there are no compliance test vectors associated with this module
# ------------------------------------------------

test: proc comp

proc: input.src
	$(FREQRESP_EXE) -bmp bmpOut.tst input.src input.src asciiOut.tst

comp: bmpOut.ref asciiOut.ref
	$(DIFF) bmpOut.ref bmpOut.tst
	$(DIFF) asciiOut.ref asciiOut.tst

input.src:
	$(UNZIP) test-freqresp.zip input.src

bmpOut.ref:
	$(UNZIP) test-freqresp.zip bmpOut.ref

asciiOut.ref:
	$(UNZIP) test-freqresp.zip asciiOut.ref
