# ---------------------------------------------------------------------------------------------------------
#	MS Visual C makefile for compiling and testing the UGST truncate module 
#       implementation
#	Implemented by <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com> -- 06.Apr.05
# ---------------------------------------------------------------------------------------------------------

# ------------------------------------------------
# Choose compiler.
# ------------------------------------------------
GCC = cl
CC_OPT = $(XXX) -I../utl -I../eid -W1
# ------------------------------------------------
# Executables
# ------------------------------------------------
RM = rm -f
TRUNCATE_EXE= truncate

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
all: truncate

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) *.ref *.proc *.test

veryclean: clean cleantest
	$(RM) truncate.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
truncate: truncate.exe

truncate.exe: trunc-lib.obj ugst-utl.obj softbit.obj truncate.obj
	$(CC) -o truncate truncate.obj trunc-lib.obj softbit.obj ugst-utl.obj

ugst-utl.obj: ../utl/ugst-utl.c
	$(CC) $(CC_OPT) -c ../utl/ugst-utl.c
	
softbit.obj: ../eid/softbit.c
	$(CC) $(CC_OPT) -c ../eid/softbit.c

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
trunc-lib.obj:  trunc-lib.c trunc-lib.h
truncate.obj: truncate.c trunc-lib.h ..\utl\ugst-utl.h ..\eid\softbit.h

# ------------------------------------------------
# Testing portability
# Note: there are no compliance test vectors associated with this module
# ------------------------------------------------

test: proc comp

proc: bin_bst.test
	$(TRUNCATE_EXE) -ib 8000 -b 6000 bin_bst.test bin_bst_6k.proc
	$(TRUNCATE_EXE) -ib 8000 -b 6000 byte_bst.test byte_bst_6k.proc
	$(TRUNCATE_EXE) -b 6000 byte_sync_bst.test byte_sync_bst_6k.proc
	$(TRUNCATE_EXE) -ib 8000 -b 6000 g192_bst.test g192_bst_6k.proc
	$(TRUNCATE_EXE) -b 6000 g192_sync_bst.test g192_sync_bst_6k.proc

comp: bin_bst_6k.ref
	$(DIFF) bin_bst_6k.proc bin_bst_6k.ref
	$(DIFF) byte_bst_6k.proc byte_bst_6k.ref
	$(DIFF) byte_sync_bst_6k.proc byte_sync_bst_6k.ref
	$(DIFF) g192_bst_6k.proc g192_bst_6k.ref
	$(DIFF) g192_sync_bst_6k.proc g192_sync_bst_6k.ref

bin_bst_6k.ref:
	$(UNZIP) test-trunc.zip *.ref

bin_bst.test:
	$(UNZIP) test-trunc.zip *.test
