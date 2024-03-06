# ----------------------------------------------------------------------
# Makefile for processing the RPEDEMO program through the test vectors
# ----------------------------------------------------------------------

RM = rm -f
RPE =  -..\rpedemo
DIFF = cf -q -h
G711 = ..\..\g711\g711demo.exe
file = sine

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
#UNZIP = -pkunzip 
UNZIP = unzip -o

# -----------------------------------------------------------------
# Implicit rules
# -----------------------------------------------------------------
.rec.log:
	$(DIFF) $*.sc $*.rec 160
	$(DIFF) $*.rpe $*.cod 76
	$(DIFF) $*.out $*.rec 160

.inp.rec:
	$(RPE) -enc $*.inp $*.rpe
	$(RPE) -dec $*.rpe $*.rec
	$(RPE) $*.inp $*.sc

# -----------------------------------------------------------------
# General targets
# -----------------------------------------------------------------
all:	test

clean:  cleantest
cleantest:
	$(RM) *.sc *.rec *.rpe $(file)*.a* $(file)*.u* 
	$(RM) $(file)*.l $(file).lin
#	-for %e in (sc rpe rec sine\*.a\* sine\*.u\* sine\*.l) do del *.%e
#	-for %e in (a-a u-u a u rpe) do del sine*.%e

veryclean: cleantest

# -----------------------------------------------------------------
# Test Vectors
# -----------------------------------------------------------------
test-tv:  proc-tv comp-tv

proc-tv:  seq01 seq02 seq03 seq04 seq05

comp-tv: seq01.log seq02.log seq03.log seq04.log seq05.log

seq01:	seq01.rec

seq02:	seq02.rec

seq03:	seq03.rec

seq04:	seq04.rec

seq05:  seq05.rec

# -----------------------------------------------------------------
# Simple test
# -----------------------------------------------------------------
test:	proc comp
proc: $(file).lin $(G711)
	# ==============================================================
	# Generate test file
	$(G711) a lilo $(file).lin $(file).a 160 1 20
	$(G711) a loli $(file).a $(file).a-a 160 1 20
	$(G711) u lilo $(file).lin $(file).u 160 1 20
	$(G711) u loli $(file).u $(file).u-u 160 1 20
	# ==============================================================
	# Tests for u-law
	# ... testing encode-only
	$(RPE) -enc $(file).u-u $(file)u-u.rpe
	$(RPE) -enc -u $(file).u $(file)u.rpe
	# ... testing decode-only
	$(RPE) -dec $(file)u-u.rpe $(file)u-u.l
	$(RPE) -dec -u $(file)u-u.rpe $(file)u-u.u
	$(G711) u lilo $(file)u-u.l $(file)u-ul.u 160 1 20
	# ... testing short-cut
	$(RPE) $(file).u-u $(file)u-u.l
	$(RPE) -u $(file).u $(file)u-u.u
	$(G711) u lilo $(file)u-u.l $(file)u-ul.u 160 1 20
	# ==============================================================
	# Tests for A-law
	# ... testing encode-only
	$(RPE) -enc $(file).a-a $(file)a-a.rpe
	$(RPE) -enc -a $(file).a $(file)a.rpe
	# ... testing decode-only
	$(RPE) -dec $(file)a-a.rpe $(file)a-a.l
	$(RPE) -dec -a $(file)a-a.rpe $(file)a-a.a
	$(G711) a lilo $(file)a-a.l $(file)a-al.a 160 1 20
	# ... testing short-cut
	$(RPE) $(file).a-a $(file)a-a.l
	$(RPE) -a $(file).a $(file)a-a.a
	$(G711) a lilo $(file)a-a.l $(file)a-al.a 160 1 20

comp:
	$(DIFF) $(file)a-a.a $(file)a-al.a 160
	$(DIFF) $(file)a-a.a $(file)a-al.a 160
	$(DIFF) $(file)a-a.rpe $(file)a.rpe 160
	$(DIFF) $(file)u-u.rpe $(file)u.rpe 160
	$(DIFF) $(file)u-u.u $(file)u-ul.u 160
	$(DIFF) $(file)u-u.u $(file)u-ul.u 160

$(file).lin: tst-rpe.zip
	$(UNZIP) tst-rpe.zip $(file).lin
	swapover -if little $(file).lin

$(G711): 
	cd ..\..\g711
	make -f makefile.tcc g711demo
	cd ..\rpeltp\ts
