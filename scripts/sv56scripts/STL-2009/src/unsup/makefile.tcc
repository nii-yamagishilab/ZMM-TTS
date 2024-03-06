# -----------------------------------------------------------------------------
#	Borland [bt]cc makefile for compiling and testing the 
#	unsupported utility programs. 
#	- x{en,de}code: xencode/xdecode uuencode/uudecode improved 
#         programs: compile with make xe xd test-xed
#	- cf,df: compare/dump programs compfile.c, dumpfile.c
#	- add/sub: executables to generate reports by adding or 
#          subtracting the contents of two files. They are both 
#          generated from the source code file add-sub.c, depending 
#          on how it is compiled.
#	- sb: byte-swap 16-bit words with program sb.c
#	- measure.c: statistical measurement tool measure.c to
#         report max,min,DC,AC, and 16-bit CRCs for files
#	- sine: sine wave generation
#       - oper: arithmetic operation over two files (+,-,/,*)
#       - chr2sh,sh2chr: conversion between byte-oriented and 16-bit 
#         word oriented bitstream files.
#       - endian: tells the endian type for the target computer
#       - bin2asc,asc2bin: conversion between ascii and binary file formats
#       - crc: 32-bit CRC calculations
#       - fdelay: flexible delay line program
#       - concat: concatenate files in binary mode.
#
#  *************************************************************************
#  WARNING:
#    NOT ALL PROGRAMS HAVE BEEN THROROUGLY TESTED IN ALL THE "PREFERRED" 
#    PLATFORMS!!
#  *************************************************************************
#
#	Implemented by <simao@ctd.comsat.com> -- 15.Jan.95
#       Revised - 
#       21.Aug.95 - Added new stuff
#       18.Sep.95 - Adding external control for user email in x*code, 
#                   see make variable WHO below. 
# -----------------------------------------------------------------------------
#CC = bcc
CC = tcc
WILDCARDS_OBJ = c:\tc\lib\wildargs.obj # This depends on your instalation!
CC_OPT = -I../utl
WHO=-DUSER_NAME="[simao@ctd.comsat.com]" # Define your email here
RM=rm -f
XE=xencode
XD=xdecode

# ----------------------------------------------------------------------
# Targets and test files
# ----------------------------------------------------------------------
EXE=add.exe asc2bin.exe astrip.exe bin2asc.exe cf.exe chr2sh.exe concat.exe \
	crc.exe df.exe fdelay.exe endian.exe measure.exe oper.exe sb.exe \
	sh2chr.exe sine.exe sub.exe xdecode.exe xencode.exe
REF_XED=voice.uue voice01.uue printme.eps printme.uue
REF_SB=bigend.src litend.src
REF_DELAY=delay-15.ref delay-a.ref delay-u.ref delaydft.ref delayfil.ref
TST_DELAY=delay-15.tst delay-0f.tst delay-a.tst delay-u.tst delaydft.tst \
	delayfil.tst
REF_CF=cftest1.dat cftest2.dat cftest3.dat
TST_ASTRIP=astrip.smp astrip.blk astrip.inp
REF_ASTRIP=astrip.ref
REF_CONCAT=concat1.txt concat2.txt concat3.txt
TST_CONCAT=concat3.tst undo.txt

# ----------------------------------------------------------------------
# Tailor to your availability
# ----------------------------------------------------------------------
#UNZIP=pkunzip	# for shareware pkunzip program
UNZIP=unzip	# if public domain unzip available
#DIFF=diff 	# if Unix diff is available
DIFF=fc	# for MSDOS version > 4.0

# ----------------------------------------------------------------------
# General rules
# ----------------------------------------------------------------------
.c.obj:
	$(CC) -c $(CC_OPT) $<

.obj.exe:
	$(CC) $(CC_OPT) $<

# ----------------------------------------------------------------------
# Macro rules
# ----------------------------------------------------------------------
all: $(EXE)

anyway:	clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) voice.rec v.uue v01.uue printme.ps print_me.uue *.tst
	$(RM) $(REF_XED) $(REF_SB) 
	$(RM) $(REF_DELAY)
	$(RM) $(REF_CF)
	$(RM) $(TST_ASTRIP) $(REF_ASTRIP)
	$(RM) $(REF_CONCAT) $(TST_CONCAT)

veryclean: clean cleantest
	$(RM) *.exe

test:   test-xed test-sb test-fdelay test-cf test-astrip test-concat

# ----------------------------------------------------------------------
# Specific targets
# ----------------------------------------------------------------------

# Abstractions
# ----------------------------------------------------------------------
add: 		add.exe
asc2bin: 	asc2bin.exe
astrip:		astrip.exe
bin2asc: 	bin2asc.exe
cf: 		cf.exe
chr2sh: 	chr2sh.exe
concat: 	concat.exe
crc:            crc.exe
df: 		df.exe
endian:		endian.exe
fdelay:		fdelay.exe
measure:	measure.exe
oper:		oper.exe
sb: 		sb.exe
sh2chr:		sh2chr.exe
sine:		sine.exe
sub: 		sub.exe
xdecode:	xdecode.exe
xencode:	xencode.exe

# Executable dependencies
# ----------------------------------------------------------------------
sb.exe: 	sb.obj
sub.exe: 	sub.obj
add.exe: 	add.obj
sine.exe:	sine.obj
chr2sh.exe: 	chr2sh.obj
sh2chr.exe:	sh2chr.obj
oper.exe:	oper.obj

measure.exe:	measure.obj
	$(CC) $(CC_OPT) measure.obj $(WILDCARDS_OBJ)

asc2bin.exe: asc2bin.c
	$(CC) $(CC_OPT) asc2bin.c

astrip.exe: astrip.c
	$(CC) $(CC_OPT) -eastrip astrip.c

bin2asc.exe: bin2asc.c
	$(CC) $(CC_OPT) bin2asc.c

crc.exe: getcrc32.c
	$(CC) $(C_OPT) -ecrc getcrc32.c $(WILDCARDS_OBJ)

concat.exe: concat.c
	$(CC) $(CC_OPT) -econcat concat.c $(WILDCARDS_OBJ)

cf.exe:		compfile.c
	$(CC) $(CC_OPT) -ecf compfile.c

df.exe:		dumpfile.c
	$(CC) $(CC_OPT) -edf dumpfile.c

endian.exe: endian.c
	$(CC) -DRUN -eendian endian.c

fdelay.exe: fdelay.c
	$(CC) $(CC_OPT) -efdelay fdelay.c

measure.obj:	measure.c

sb.obj:		sb.c

sub.obj:	sub-add.c
	$(CC) -c $(CC_OPT) -osub sub-add.c

add.obj:	sub-add.c
	$(CC) -c $(CC_OPT) -DADD_FILES -oadd sub-add.c

chr2sh.obj: chr2sh.c

sh2chr.obj: sh2chr.c

xencode.exe: xencode.c 
	$(CC) $(CC_OPT) $(WHO) xencode.c

xdecode.exe: xdecode.c 
	$(CC) $(CC_OPT) xdecode.c

#===============================================================
# Testing astrip
#===============================================================
test-astrip: cftest1.dat astrip.ref ./astrip.exe
	copy cftest1.dat astrip.inp
	sb -over -if little astrip.inp
	astrip -q -smooth -wlen 145 -sample astrip.inp astrip.smp
	$(DIFF) astrip.smp astrip.ref
	astrip -q -smooth -wlen 145 astrip.inp astrip.blk 160
	$(DIFF) astrip.blk astrip.ref

astrip.ref: tstunsup.zip
	$(UNZIP) tstunsup.zip $(REF_ASTRIP)
	touch astrip.ref
	sb -over -if little astrip.ref

#===============================================================
# Testing concat
#===============================================================
test-concat: $(REF_CONCAT)
	concat -undo undo.txt concat1.txt concat2.txt concat3.tst
	diff concat3.tst concat3.txt
concat1.txt: tstunsup.zip
	$(UNZIP) tstunsup.zip $(REF_CONCAT)
	touch concat1.txt

#===============================================================
# Testing cf [compfile]
#===============================================================
cftest1.dat: tstunsup.zip
	$(UNZIP) tstunsup.zip $(REF_CF)
	touch cftest1.dat

test-cf: cftest1.dat
	# Difference in Block 1, sample 69
	cf -short cftest1.dat cftest2.dat
	# Difference in Block 1, sample 35
	cf -long cftest1.dat cftest2.dat
	# Difference in Block 1, sample 35
	cf -float cftest1.dat cftest2.dat
	# Difference in Block 1, sample 18
	cf -double cftest1.dat cftest2.dat
	# Difference in Block 1, sample 69
	cf -h cftest1.dat cftest2.dat
	# Difference in Block 1, sample 35
	cf -h -long cftest1.dat cftest2.dat
	# Difference in Block 1, sample 69
	cf -delay -2 cftest1.dat cftest3.dat
	# Difference in Block 1, sample 69
	cf -delay  2 cftest3.dat cftest1.dat
	# No differences
	cf -delay -2 cftest2.dat cftest3.dat

#===============================================================
# Testing sb
#===============================================================
bigend.src:
	$(UNZIP) tstunsup.zip bigend.src litend.src
	touch bigend.src

test-sb: cf sb bigend.src
	sb bigend.src bytswpd.tst
	cf -h litend.src bytswpd.tst
	sb -over bytswpd.tst
	cf -h bigend.src bytswpd.tst

#===============================================================
# Testing fdelay.c
#===============================================================
test-fdelay: bigend.src proc-fdelay comp-fdelay
proc-fdelay:
	fdelay bigend.src delaydft.tst
	fdelay -file litend.src bigend.src delayfil.tst
	sb -over -if little bigend.src
	fdelay -law a bigend.src delay-a.tst
	fdelay -delay 57 -law u bigend.src delay-u.tst
	fdelay -dec 15 bigend.src delay-15.tst
	fdelay -hex 0xF bigend.src delay-0f.tst
	$(RM) bigend.src

comp-fdelay: delaydft.ref
	$(DIFF) delaydft.tst delaydft.ref
	$(DIFF) delayfil.tst delayfil.ref
	$(DIFF) delay-a.tst delay-a.ref
	$(DIFF) delay-u.tst delay-u.ref
	$(DIFF) delay-15.tst delay-15.ref
	$(DIFF) delay-0f.tst delay-15.ref

delaydft.ref:
	$(UNZIP) tstunsup.zip delay*.ref
	touch delaydft.ref
	sb -over -if little delay-15.ref 
	sb -over -if little delay-a.ref
	sb -over -if little delay-u.ref

#===============================================================
# Testing xencode/xdecode
#===============================================================
voice.uue:
	$(UNZIP) tstunsup.zip voice.uue voice01.uue printme.eps printme.uue
	touch voice.uue

test-xed: voice.uue
	#----------------------------------------------------------------------
	#      **** Testing for a BINARY file ****
	# **** No differences should be found here ****
	$(XD) voice.uue
	$(DIFF) voice.rec voice.ori
	# **** Now, differences may appear only in the preamble
	#      (i.e., before the begin line)
	$(XE) voice.rec v.uue 7
	-$(DIFF) v.uue voice.uue 
	-$(DIFF) v01.uue voice01.uue 
	#
	#----------------------------------------------------------------------
	#      **** Testing for an ASCII file ****
	# **** No differences should be found here ****
	$(XD) printme.uue
	$(DIFF) printme.ps printme.eps
	# **** Now, differences may appear only in the preamble
	#      (i.e., before the begin line)
	$(XE) printme.ps print_me.uue
	-$(DIFF) print_me.uue printme.uue
	$(RM) voice.rec 
	$(RM) v.uue 
	$(RM) v01.uue 
	$(RM) printme.ps 
	$(RM) print_me.uue
#===============================================================
