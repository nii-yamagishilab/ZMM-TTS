# -----------------------------------------------------------------------------
#	MS Visual C makefile for compiling & testing the UGST G.711 module
#	31.Jan.2000 - Implemented by <simao.campos@labs.comsat.com>
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$$6~/[0-9]+:[0-9][0-9]/ {print "sb -over",$$NF};END{print "exit"}'

# ----------------------------------------------------------------------
# Choose compiler. 
# ----------------------------------------------------------------------
CC=cl
CC_OPT = -I../utl

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/MS Visual C/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose a file comparison utility: 
# ------------------------------------------------
DIFF = cf -q

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
RM=rm -f
REF_VECTORS= sweep-r.a sweep-r.a-a sweep-r.rea sweep-r.reu \
	sweep-r.u sweep-r.u-u sweep.src 
TEST_VECTORS = sweep.a sweep.a-a sweep.rea sweep.reu sweep.u sweep.u-u
G711_OBJ = g711.obj g711demo.obj

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: g711demo

anyway: clean g711demo

clean:
	$(RM) $(G711_OBJ)

cleantest:
	$(RM) $(TEST_VECTORS) $(REF_VECTORS)

veryclean: clean cleantest
	$(RM) g711demo.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
g711demo: g711demo.exe
g711demo.exe: g711demo.obj g711.obj
	$(CC) -o g711demo g711demo.obj g711.obj

shiftbit: shiftbit.exe
shiftbit.exe: shiftbit.c ugstdemo.h
	$(CC) -o shiftbit shiftbit.c

# ------------------------------------------------
# Test portability
# Note: there are no compliance test vectors associated with the FIR module
# ------------------------------------------------
test: proc comp

proc:	sweep.src
#       sweep-r.* have been generated from sweep.src in a reference environment
#       results of the comparisons shall yield 0 different samples!
	g711demo A lilo sweep.src sweep.a 256 1 256
	g711demo u lilo sweep.src sweep.u 256 1 256
	g711demo A lili sweep.src sweep.a-a 256 1 256
	g711demo u lili sweep.src sweep.u-u 256 1 256
	g711demo A loli sweep.a sweep.rea 256 1 256
	g711demo u loli sweep.u sweep.reu 256 1 256

comp: 	sweep-r.u-u
	$(DIFF) sweep.a   sweep-r.a 
	$(DIFF) sweep.a-a sweep-r.a-a 
	$(DIFF) sweep.rea sweep-r.rea 
	$(DIFF) sweep.reu sweep-r.reu 
	$(DIFF) sweep.u   sweep-r.u 
	$(DIFF) sweep.u-u sweep-r.u-u 

# ------------------------------------------------
# Extract from archive, if necessary
# ------------------------------------------------
sweep.src:	tst-g711.zip
	$(UNZIP) tst-g711.zip sweep.src 
	swapover sweep.src

sweep-r.u-u:	tst-g711.zip
	$(UNZIP) tst-g711.zip sweep-r.*
	swapover sweep-r.*
