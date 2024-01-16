# -----------------------------------------------------------------------------
#	Borland-make makefile for testing the vselp implementation
#	The executable must be defined by variable IS54 below. The root of 
#	the name of the test file to be processed is defined by variable 
#	FR (below). The file deletion command is defined by variable RM.
#	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
#	Revised:
#	12.Feb.95	added handling of ZIP file with test files
# -----------------------------------------------------------------------------

# ------------------------------------------------
# Choose a file comparison utility: 
# ------------------------------------------------
DIFF = sub -q -equiv 1

# ------------------------------------------------
# Choose an archiving utility: 
#	- public domain unzip, or [PC/Unix/VMS]
#	- shareware pkunzip [PC only]
# ------------------------------------------------
###UNZIP = -pkunzip -3 -+ -- -)
#UNZIP = -pkunzip
UNZIP = unzip -o

# ------------------------------------------------
# Choose an AWK; suggest use GNU version 
#                (available via anonymous ftp) 
# ------------------------------------------------
AWK = gawk
AWK_CMD = '$$6~/[0-9]+:[0-9][0-9]/ {print "sb -over",$$NF};END{print "exit"}'
###AWK_CMD = '$6~/[0-9]+:[0-9][0-9]/{{print "sb -over",$NF;print "touch",$NF}};END{print "exit"}'

# ------------------------------------------------
# General purpose symbols
# ------------------------------------------------
IS54 = ../uvselp -q
RM = rm -f
FR = voice

# ------------------------------------------------
# Targets
# ------------------------------------------------
all:: test comp

clean:
	$(RM) *.hbs *.bbs *.rec *.rek *.bec enc.log dec*.log
	$(RM) $(FR).src $(FR).cod $(FR).out $(FR)-pf.out $(FR).hex
#	for %%f in (*.hbs *.bbs *.rec *.rek *.bec enc.log dec*.log) do $(RM) %%f
cleantest: clean
veryclean: clean


test: clean proc comp
proc: encode decode both

encode: $(FR).src
#	Encode for ascii and binary bitstream, repectively
	$(IS54) -enc $(FR).src $(FR).hbs enc.log
	$(IS54) -enc -nolog -bin $(FR).src $(FR).bbs

decode:
#	Decode with and without post-filter for an ASCII hex bitstream
	$(IS54) -dec $(FR).hbs $(FR).rec dec.log
	$(IS54) -dec -pf $(FR).hbs $(FR)-pf.rec dec-pf.log
#	Decode with and without post-filter for a binary bitstream
	$(IS54) -dec -bin -nolog $(FR).bbs $(FR).bec 
	$(IS54) -dec -pf -bin -nolog $(FR).bbs $(FR)-pf.bec 

both:
#	Encode and decode with and without post-filter
	$(IS54) -nolog $(FR).src $(FR).rek
	$(IS54) -nolog -pf $(FR).src $(FR)-pf.rek

comp:	$(FR).out
#	Compare ascii bitstreams (test x reference)
#	Expect: voice.hbs x voice.hex: 1295 different samples out of 6750
	$(DIFF) $(FR).hbs  $(FR).hex 27
#	Compare binary bitstreams (test x reference)
#	Expect: voice.bbs x voice.cod: 1284 different samples out of 8910
	$(DIFF) $(FR).bbs  $(FR).cod 27
#	Compare decoded files (test x reference), w/o post-filter
#	Expect: voice.rec x voice.out: 17428 different samples out of 52800
#	Expect: voice.bec x voice.out: 17428 different samples out of 52800
	$(DIFF) $(FR).rec $(FR).out 160
	$(DIFF) $(FR).bec $(FR).out 160
#	Compare decoded files (test x reference), with post-filter
#	Expect: voice-pf.rec x voice-pf.out: 17455 diff. samples out of 52800
#	Expect: voice-pf.bec x voice-pf.out: 17455 diff. samples out of 52800
	$(DIFF) $(FR)-pf.rec $(FR)-pf.out 160
	$(DIFF) $(FR)-pf.bec $(FR)-pf.out 160
#	Compare enc+dec files (test x reference), with and w/o post-filter
#	Expect: voice.rek x voice.out: 17428 diff. samples out of 52800
#	Expect: voice-pf.rek x voice-pf.out: 17455 diff. samples out of 52800
	$(DIFF) $(FR).rek $(FR).out 160
	$(DIFF) $(FR)-pf.rek $(FR)-pf.out 160

$(FR).src:	is54-tst.zip
	$(UNZIP) is54-tst.zip $(FR).src
	swapover $(FR).src
#	$(UNZIP) -v is54-tst.zip $(FR).src > x
#	$(AWK) $(AWK_CMD) x > y.bat
#	y
#	for %%f in (x y.bat) do $(RM) %%f

$(FR).out:	is54-tst.zip
	$(UNZIP) is54-tst.zip $(FR).hex $(FR).cod $(FR).out $(FR)-pf.out
	swapover $(FR).cod $(FR).out $(FR)-pf.out
#	$(UNZIP) -v is54-tst.zip $(FR).cod $(FR).out $(FR)-pf.out > x
#	$(AWK) $(AWK_CMD) x > y.bat
#	y
#	for %%f in (x y.bat) do $(RM) %%f
