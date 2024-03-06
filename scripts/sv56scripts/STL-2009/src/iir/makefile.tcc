# -----------------------------------------------------------------------------
#       Unix makefile for compiling and testing the UGST IIR module.
#	30.Oct.94 - Created - <simao@ctd.comsat.com>
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose a file comparison utility: 
#	- cf compares, 
#	- sub shows the difference of the different samples
# Default is cf.
# ------------------------------------------------
#CF = cf -q
CF = sub -q -equiv 1
CF_OPT = 256 1 30

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose compiler & options
# ------------------------------------------------
CC = tcc
CC_OPT = -I../utl 

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$6~/[0-9]+:[0-9][0-9]/{{print "sb -over",$NF;print "touch",$NF}};END{print "exit"}'

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
TEST_FILES = testg712.* cascg712.flt iir-irs.flt
CASC_REF=cascg712.ref iir-irs.ref testpcm1.ref testpcmu.ref testpcmd.ref
OBJ_FILES = iir-lib.obj ugst-utl.obj
PCM = pcmdemo
IRS = cirsdemo
CASC_G712 = c712demo
RM = -rm -f

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: pcmdemo cirsdemo c712demo

anyway: clean all

clean:
	$(RM) *.obj 

cleantest:
	$(RM) $(TEST_FILES) $(CASC_REF) test.src

veryclean: clean cleantest
	$(RM) pcmdemo.exe cirsdemo.exe c712demo.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
ugst-utl.obj: ../utl/ugst-utl.c
	$(CC) $(CC_OPT) -c ../utl/ugst-utl.c

# The *original* parallel-form log-PCM demo
pcmdemo: pcmdemo.exe
pcmdemo.exe: pcmdemo.obj iir-g712.obj $(OBJ_FILES)
	$(CC) $(CC_OPT) -epcmdemo pcmdemo.obj iir-g712.obj $(OBJ_FILES)

# Bellcore IRS proposed for the 8kbit/s qualification test plan
cirsdemo: cirsdemo.exe
cirsdemo.exe: cirsdemo.obj iir-irs.obj $(OBJ_FILES)
	$(CC) $(CC_OPT) -ecirsdemo cirsdemo.obj iir-irs.obj $(OBJ_FILES)

# Bellcore G.712 proposed for the 8kbit/s qualification test plan
c712demo: c712demo.exe
c712demo.exe: cascg712.obj c712demo.obj $(OBJ_FILES)
	$(CC) $(CC_OPT) -ec712demo c712demo.obj cascg712.obj $(OBJ_FILES)


# ------------------------------------------------
# Testing the portability of the implementation!
# Note: there are no compliance test vectors associated with the IIR module
# ------------------------------------------------
test:	proc comp

proc:	test.src
	$(PCM) test.src testg712.100 1_1 0 0
	$(PCM) test.src testg712.u00 1_2 0 0
	$(PCM) test.src testg712.d00 2_1 0 0
	$(PCM) test.src testg712.010 0 1_1 0
	$(PCM) test.src testg712.0u0 0 1_2 0
	$(PCM) test.src testg712.0d0 0 2_1 0
	$(PCM) test.src testg712.001 0 0 1_1
	$(PCM) test.src testg712.00u 0 0 1_2
	$(PCM) test.src testg712.00d 0 0 2_1
	$(IRS) test.src iir-irs.flt
	$(CASC_G712) test.src cascg712.flt

comp: cascg712.ref
	$(CF) testpcm1.ref testg712.100 $(CF_OPT)
	$(CF) testpcmu.ref testg712.u00 $(CF_OPT)
	$(CF) testpcmd.ref testg712.d00 $(CF_OPT)
	$(CF) testg712.100 testg712.010 $(CF_OPT)
	$(CF) testg712.u00 testg712.0u0 $(CF_OPT)
	$(CF) testg712.d00 testg712.0d0 $(CF_OPT)
	$(CF) testg712.100 testg712.001 $(CF_OPT)
	$(CF) testg712.u00 testg712.00u $(CF_OPT)
	$(CF) testg712.d00 testg712.00d $(CF_OPT)
	$(CF) iir-irs.ref  iir-irs.flt  $(CF_OPT)
	$(CF) cascg712.ref cascg712.flt $(CF_OPT)

test.src: test-iir.zip
	$(UNZIP) -n test-iir.zip test.src
	swapover test.src
#	$(UNZIP) -v test-iir.zip test.src > x
#	$(AWK) $(AWK_CMD) x > y
#	command < y
#	$(RM) x y

cascg712.ref: test-iir.zip
	$(UNZIP) -n test-iir.zip $(CASC_REF)
	swapover $(CASC_REF)
#	$(UNZIP) -v test-iir.zip *.ref > x
#	$(AWK) $(AWK_CMD) x > y
#	command < y
#	$(RM) x y
