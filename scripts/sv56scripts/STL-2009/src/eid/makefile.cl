# -----------------------------------------------------------------------------
#  MS Visual C compiler makefile for compiling and testing the UGST EID module
#  To compile for portability test, use:
#   make -f makefile.cl XXX=-DPORT_TEST
#  01.Jul.1999 - Created <simao.campos@comsat.com>
#  08.Feb.2001 - Included bs-stat.c
#  09.Oct.2008 - Included eid-ev.c
# -----------------------------------------------------------------------------
.SUFFIXES: .c .o 

# ------------------------------------------------
# Executables
# ------------------------------------------------
RM=rm -f
RUN= # Define RUN if you need a loader (e.g. go32) to run the program
EIDDEMO= $(RUN) eiddemo -q
EID8K= $(RUN) eid8k
GEN_PATT= $(RUN) gen-patt
GEN_RATE_PROFILE= $(RUN) gen-rate-profile
EID_INT= $(RUN) eid-int
EID_XOR= $(RUN) eid-xor
EID_EV= $(RUN) eid-ev
EP_STATS= $(RUN) ep-stats

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose compiler/options. 
# ------------------------------------------------
##CC_OPT = -DPORT_TEST -I../utl
CC_OPT = $(XXX) -I../utl

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = 'BEGIN{OFS="\t"};{print $$4,$$6,$$8,$$10,$$12,$$14,$$16,$$18}'

# ------------------------------------------------
# List of files (source and object)
# ------------------------------------------------
EID_MODULE = eid.c eid.h

# ------------------------------------------------
# Generic rules
# ------------------------------------------------
.c.obj:
	$(CC) $(CC_OPT) -c $<

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: eiddemo eid8k gen-patt eid-xor ep-stats eid-int bs-stats eid-ev gen-rate-profile

anyway: clean all

clean:
	$(RM) *.obj

cleantest:
	$(RM) b*.ser p*.ser z_*.b?? ep?05g10.??? sta zero.ser zero.src dummy.bs
	$(RM) *.crc zero-crc.txt

veryclean: clean cleantest
	$(RM) eiddemo.exe eid8k.exe ??-stats.exe gen-patt.exe eid-xor.exe 

# ------------------------------------------------
# Specific rules
# ------------------------------------------------
eiddemo: eiddemo.exe
eiddemo.exe: eiddemo.obj eid.obj
	$(CC) -o eiddemo eiddemo.obj eid.obj 

eid8k: eid8k.exe
eid8k.exe: eid8k.obj eid.obj eid_io.obj
	$(CC) -o eid8k eid8k.obj eid.obj eid_io.obj

gen-patt: gen-patt.exe
gen-patt.exe: gen-patt.obj eid.obj eid_io.obj softbit.obj
	$(CC) -o gen-patt gen-patt.obj eid.obj eid_io.obj softbit.obj

gen-rate-profile: gen-rate-profile.exe
gen-rate-profile.exe: gen_rate_profile.obj
	$(CC) -o gen-rate-profile gen_rate_profile.obj
	
eid-xor: eid-xor.exe
eid-xor.exe: eid-xor.obj softbit.obj
	$(CC) -o eid-xor eid-xor.obj softbit.obj

bs-stats: bs-stats.exe
bs-stats.exe: bs-stats.obj softbit.obj
	$(CC) -o bs-stats bs-stats.obj softbit.obj

ep-stats: ep-stats.exe
ep-stats.exe: ep-stats.obj softbit.obj
	$(CC) -o ep-stats ep-stats.obj softbit.obj

eid-int: eid-int.exe
eid-int.exe: eid-int.obj softbit.obj
	$(CC) -o eid-int eid-int.obj softbit.obj

cvt-head: cvt-head.exe
cvt-head.exe: cvt-head.obj softbit.obj
	$(CC) -o cvt-head cvt-head.obj softbit.obj

eid-ev: eid-ev.exe
eid-ev.exe: eid-ev.obj softbit.obj
	$(CC) -o eid-ev eid-ev.obj softbit.obj


# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
eid-xor.obj:  eid-xor.c
eid.obj:      eid.c eid.h
eid8k.obj:    eid8k.c eid_io.c eid.h eid_io.h 
eid_io.obj:   eid_io.c eid_io.h
eiddemo.obj:  eiddemo.c eid.h 
bs-stats.obj: bs-stats.c
ep-stats.obj: ep-stats.c
	$(CC) $(CC_OPT) -DRUN -c ep-stats.c
gen-patt.obj: gen-patt.c eid_io.c eid.h eid_io.h 
softbit.obj:  softbit.c softbit.h


# -----------------------------------------------------------------------------
# Test the implementation: "classic" and Bellcore model EID
# Note: there are no compliance test vectors associated with the EID module
# -----------------------------------------------------------------------------
test: test-zero test-8k test-xor test-patt test-bs
proc: proc-zero proc-8k proc-xor proc-patt 
comp: measure
measure: measure-zero measure-8k measure-xor measure-patt

# -----------------------------------------------------------------------------
# Test the implementation: "classic" EID
# -----------------------------------------------------------------------------
test-zero: proc-zero measure-zero

proc-zero: zero.ser
	$(RM) ber fer
	$(EIDDEMO) zero.ser b3g0f0g0.ser ber fer 0.001 0 0 0 
	$(RM) ber fer
	$(EIDDEMO) zero.ser b3g2f0g0.ser ber fer 0.001 0.01 0 0 
	$(RM) ber fer
	$(EIDDEMO) zero.ser b0g0f2g0.ser ber fer 0 0 0.01 0 
	$(RM) ber fer
	$(EIDDEMO) zero.ser b0g0f2g5.ser ber fer 0 0 0.01 .5 
	$(RM) ber fer

measure-zero:
	measure -crc  b3g2f0g0.ser  > xxx
	measure -crc  b3g0f0g0.ser >> xxx
	measure -crc  b0g0f2g0.ser >> xxx
	measure -crc  b0g0f2g5.ser >> xxx
	$(AWK) $(AWK_CMD) < xxx
	$(RM) xxx

# -----------------------------------------------------------------------------
# Test the implementation: Bellcore model EID
# -----------------------------------------------------------------------------

test-8k: proc-8k measure-8k

proc-8k:
	$(RM) sta
	$(EID8K) patr10-2.ser r 100 1 sta .01
	$(RM) sta
	$(EID8K) patf10-2.ser f 1000 1 sta .01
	$(RM) sta
	$(EID8K) patb1pct.ser b 1000 1 sta 1
	$(RM) sta
	$(EID8K) patb3pct.ser b 1000 1 sta 3
	$(RM) sta
	$(EID8K) patb5pct.ser b 1000 1 sta 5
	$(RM) sta

measure-8k:
	measure -crc patb1pct.ser > xxx
	measure -crc patb3pct.ser >> xxx
	measure -crc patb5pct.ser >> xxx
	measure -crc patf10-2.ser >> xxx
	measure -crc patr10-2.ser >> xxx
	$(AWK) $(AWK_CMD) < xxx
	$(RM) xxx

# -----------------------------------------------------------------------------
# Get error pattern from archive
# -----------------------------------------------------------------------------
zero.ser: zero-eid.zip
	$(UNZIP) zero-eid.zip zero.ser
	sb -over zero.ser

zero.src: zero-eid.zip
	$(UNZIP) zero-eid.zip zero.src

# -----------------------------------------------------------------------------
# Test the generic error pattern generation program
# Run with:
#	make FMT=-g192 test-patt  --> for patterns in 16-bit G.192 format
#	make FMT=-byte test-patt  --> for patterns in G.192 byte format
#	make FMT=-bit  test-patt  --> for patterns in compact bit format
# -----------------------------------------------------------------------------
test-patt: proc-patt measure-patt
proc-patt:
	$(RM) sta
	$(GEN_PATT) -g192 $(FMT) patr10-2.ser r 10000 1 sta .01
	$(RM) sta
	$(GEN_PATT) -g192 $(FMT) patf10-2.ser f 10000 1 sta .01
	$(RM) sta
	$(GEN_PATT) -g192 $(FMT) patb1pct.ser b 10000 1 sta .01
	$(RM) sta
	$(GEN_PATT) -g192 $(FMT) patb3pct.ser b 10000 1 sta .03
	$(RM) sta
	$(GEN_PATT) -g192 $(FMT) patb5pct.ser b 10000 1 sta .05
	$(RM) sta

measure-patt:
	measure -crc patb1pct.ser > xxx
	measure -crc patb3pct.ser >> xxx
	measure -crc patb5pct.ser >> xxx
	measure -crc patf10-2.ser >> xxx
	measure -crc patr10-2.ser >> xxx
	$(AWK) $(AWK_CMD) < xxx
	$(RM) xxx

# -----------------------------------------------------------------------------
# Test the error insertion program
# -----------------------------------------------------------------------------
test-xor: proc-xor measure-xor

epr05g10.192:
	$(RM) -f sta
	$(GEN_PATT) -g192 -ber -rate 0.05 -gamma 0.10 epr05g10.192 r 10000 1
	$(RM) -f sta
	$(GEN_PATT) -byte -ber -rate 0.05 -gamma 0.10 epr05g10.byt r 10000 1
	$(RM) -f sta
	$(GEN_PATT) -bit  -ber -rate 0.05 -gamma 0.10 epr05g10.bit r 10000 1
	$(RM) -f sta
	$(GEN_PATT) -g192 -fer -rate 0.05 -gamma 0.10 epf05g10.192 f 10000 1
	$(RM) -f sta
	$(GEN_PATT) -byte -fer -rate 0.05 -gamma 0.10 epf05g10.byt f 10000 1
	$(RM) -f sta
	$(GEN_PATT) -bit  -fer -rate 0.05 -gamma 0.10 epf05g10.bit f 10000 1
	$(RM) -f sta

proc-xor: epr05g10.192 zero.src
	$(EID_XOR) -ber -bs bit -ep g192 zero.src epr05g10.192 z_r05g10.bg1
	$(EID_XOR) -ber -bs bit -ep byte zero.src epr05g10.byt z_r05g10.bby
	$(EID_XOR) -ber -bs bit -ep bit  zero.src epr05g10.bit z_r05g10.bbi
	$(EID_XOR) -fer -bs bit -ep g192 zero.src epf05g10.192 z_f05g10.bg1
	$(EID_XOR) -fer -bs bit -ep byte zero.src epf05g10.byt z_f05g10.bby
	$(EID_XOR) -fer -bs bit -ep bit  zero.src epf05g10.bit z_f05g10.bbi

measure-xor:
	measure -crc z_r05g10.bg1 >  xxx
	measure -crc z_r05g10.bby >> xxx
	measure -crc z_r05g10.bbi >> xxx
	measure -crc z_f05g10.bg1 >> xxx
	measure -crc z_f05g10.bby >> xxx
	measure -crc z_f05g10.bbi >> xxx
	$(AWK) $(AWK_CMD) < xxx
	$(RM) xxx

# -----------------------------------------------------------------------------
# Test the bitstream statistics program
# -----------------------------------------------------------------------------
test-bs: dummy.bs
	bs-stats dummy.bs -

dummy.bs: zero-eid.zip
	$(UNZIP) zero-eid.zip dummy.bs
	sb -over -if little dummy.bs

gen-dummy.bs: ascii-bs.awk ascii-fr.ref
	gawk -f ascii-bs.awk ascii-fr.ref > x.x
	asc2bin -h x.x dummy.bs
	$(RM) x.x
