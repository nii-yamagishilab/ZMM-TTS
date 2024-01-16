# -----------------------------------------------------------------------------
#       MS Visual C makefile for compiling and testing the UGST STEREOOP module.
#	08.Oct.2008 
# -----------------------------------------------------------------------------
.SUFFIXES: .c .obj 


# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/MS Visual C/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$$6~/[0-9]+:[0-9][0-9]/ {print "sb -over",$$NF};END{print "exit"}'

# ------------------------------------------------
# Choose compiler options
# ------------------------------------------------
CC=cl
CC_OPT= -I../utl -W1


# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: stereoop

anyway: clean all

clean:
	$(RM) *.obj

# ------------------------------------------------
# Specific rules
# ------------------------------------------------

stereoop: stereoop.exe
stereoop.exe: stereoop.obj
	$(CC) $(CC_OPT) -o stereoop stereoop.obj 