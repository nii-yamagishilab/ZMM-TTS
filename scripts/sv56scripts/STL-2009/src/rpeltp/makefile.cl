#--------------------------------------------------------------------------
# RPELTP makefile for MS Visual C (Windows 9x/NT/2000)
#--------------------------------------------------------------------------
# Original Copyright message:
# Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
# Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
# details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
#
# File adapted by simao.campos@labs.comsat.com for the UGST distribution.
#--------------------------------------------------------------------------

# Machine dependent flags you must configure to port

######### Define SASR if >> is a signed arithmetic shift (-1 >> 1 == -1)
SASR	= -DSASR

######### MUST Define USE_FLOAT_MUL for bit exact performance to GSM Spec.
MULHACK = -DUSE_FLOAT_MUL

# Choose a compiler.  The code works both with ANSI and K&R-C.
# Use -DNeedFunctionPrototypes to compile with, -UNeedFunctionPrototypes to
# compile without, function prototypes in the header files.
#
# You can use the -DSTUPID_COMPILER to circumvent some compilers'
# static limits regarding the number of subexpressions in a statement.

CC		= cl
CCFLAGS 	= -c -W1 -DNeedFunctionPrototypes=1

#--------------------------------------------------------------------------
#
#    You shouldn't have to configure below this line if you're porting.
# 
#--------------------------------------------------------------------------

# Tools
RM=rm -f

# Local Directories
INC	= .

# Flags

######### Remove -DNDEBUG to enable assertions.
DEBUG	= -DNDEBUG 

######### It's $(CC) $(CFLAGS)
CFLAGS	= $(CCFLAGS) $(SASR) $(DEBUG) $(MULHACK) $(FAST) -I$(INC) -I../utl


#--------------------------------------------------------------------------
# Headers
HEADERS	=	proto.h unproto.h config.h private.h gsm.h rpeltp.h

#--------------------------------------------------------------------------
# Sources
GSM_SOURCES =	add.c code.c debug.c decode.c long_ter.c lpc.c \
		preproce.c rpe.c gsm_dest.c gsm_deco.c gsm_enco.c \
		gsm_expl.c gsm_impl.c gsm_crea.c gsm_prin.c gsm_opti.c \
		rpeltp.c short_te.c table.c

DEMO_SOURCES = rpedemo.c ../g711/g711.c

SOURCES	=	$(GSM_SOURCES) $(DEMO_SOURCES)

#--------------------------------------------------------------------------
# Object files
GSM_OBJECTS =	add.obj code.obj debug.obj decode.obj long_ter.obj lpc.obj \
		preproce.obj rpe.obj gsm_dest.obj gsm_deco.obj gsm_enco.obj \
		gsm_expl.obj gsm_impl.obj gsm_crea.obj gsm_prin.obj \
		gsm_opti.obj rpeltp.obj short_te.obj table.obj

DEMO_OBJECTS =	rpedemo.obj g711.obj

OBJECTS =	 $(GSM_OBJECTS) $(DEMO_OBJECTS)


# ------------------------------------------------
# Implicit rules
# ------------------------------------------------
.c.obj:
		$(CC) $(CFLAGS) $?

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
all::		rpedemo

anyway:		clean rpedemo

clean:
		$(RM) $(OBJECTS)

cleantest:
	echo No test file processing available in this directory

veryclean: clean
		$(RM) rpedemo.exe

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
g711.obj:		../g711/g711.c
		$(CC) $(CFLAGS) -I../g711 ../g711/g711.c
		
rpedemo.obj:	rpedemo.c ../g711/g711.h
		$(CC) $(CFLAGS) -I../g711 rpedemo.c
		
rpedemo:	rpedemo.exe
rpedemo.exe:	$(OBJECTS)
		$(CC) -o rpedemo $(OBJECTS) 

# ------------------------------------------------
# Run add < add_test.dta to make sure the
# basic arithmetic functions work as intended.
# ------------------------------------------------
add: add.exe
add.exe:	add_test.o
	$(CC) $(LFLAGS) -o add add_test.o 

addtst:	add.exe add_test.dta
	add < add_test.dta > /dev/null
