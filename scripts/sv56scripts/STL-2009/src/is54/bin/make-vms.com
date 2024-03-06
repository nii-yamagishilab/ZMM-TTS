$!-----------------------------------------------------------------------------
$!	VMS DCL for testing the vselp implementation
$!	The executable must be defined by variable IS54 below. The root of 
$!	the name of the test file to be processed is defined by variable 
$!	FR (below).
$!
$!	Usage:
$!	@make-vms 
$!	or
$!	@make-vms target
$!	where target is
$!	all:	run test and comp
$!	test:	run clean, encode, decode, and both
$!	clean:	delete processed files
$!	encode:	process test files with -enc option
$!	decode:	process test files with -dec option
$!	both:	encode and the decode in a single pass the test file
$!	comp:	compare the processed files with the reference ones
$!	sub:	subtract the processed files from the reference ones
$!
$!	Implemented by <simao@cpqd.ansp.br> -- 01.May.94
$!-----------------------------------------------------------------------------
$ def d20:[tdsimao.ugst.tools.is54.bin]
$ IS54 = "uvselp"
$ DIFF = "cf -q"
$ RM = "delete"
$ FR = "voice"  ! test File Root
$ 
$! Process User's option
$ if ("''p1'".nes."") 
$ then
$   goto 'p1'
$ else
$   goto all
$ endif
$
$ 
$ test: ! clean encode decode both
$ clean:
$ 	'RM' *.hbs.*,*.bbs.*,*.rec.*,*.rek.*,*.bec.*,enc.log.*,dec*.log.*
$ if ("''p1'".nes."TEST".and."''p1'".nes."DECODE".and."''p1'".nes."ENCODE".and."''p1'".nes."BOTH") then exit
$ 
$ all:  ! test comp
$ encode:
$ !	Encode for ascii and binary bitstream, repectively
$ 	'IS54' -enc 'FR'.src 'FR'.hbs enc.log
$ 	'IS54' -enc -nolog -bin 'FR'.src 'FR'.bbs
$ if ("''p1'".nes."ALL".and."''p1'".nes."TEST".and."''p1'".nes."DECODE".and."''p1'".nes."BOTH") then exit
$ 
$ decode:
$ !	Decode with and without post-filter for an ASCII hex bitstream
$ 	'IS54' -dec 'FR'.hbs 'FR'.rec dec.log
$ 	'IS54' -dec -pf 'FR'.hbs 'FR'-pf.rec dec-pf.log
$ !	Decode with and without post-filter for a binary bitstream
$ 	'IS54' -dec -bin -nolog 'FR'.bbs 'FR'.bec 
$ 	'IS54' -dec -pf -bin -nolog 'FR'.bbs 'FR'-pf.bec 
$ if ("''p1'".nes."ALL".and."''p1'".nes."TEST".and."''p1'".and."''p1'".nes."BOTH") then exit
$ 
$ both:
$ !	Encode and decode with and without post-filter
$ 	'IS54' -nolog 'FR'.src 'FR'.rek
$ 	'IS54' -nolog -pf 'FR'.src 'FR'-pf.rek
$ if ("''p1'".nes."ALL") then exit
$ 
$ comp:
$ !	Compare ascii bitstreams (test x reference'
$ 	'DIFF' 'FR'.hbs  'FR'.hex 27
$ !	Compare binary bitstreams (test x reference'
$ 	'DIFF' 'FR'.bbs  'FR'.cod 27
$ !	Compare decoded files (test x reference', w/o post-filter
$ 	'DIFF' 'FR'.rec 'FR'.out 160
$ 	'DIFF' 'FR'.bec 'FR'.out 160
$ !	Compare decoded files (test x reference', with post-filter
$ 	'DIFF' 'FR'-pf.rec 'FR'-pf.out 160
$ 	'DIFF' 'FR'-pf.bec 'FR'-pf.out 160
$ !	Compare enc+dec files (test x reference', with and w/o post-filter
$ 	'DIFF' 'FR'.rek 'FR'.out 160
$ 	'DIFF' 'FR'-pf.rek 'FR'-pf.out 160
$ exit	
$
$ sub:
$ !	Subtract decoded files (test x reference', w/o post-filter
$ 	SUB 'FR'.rec 'FR'.out 160 1 348
$ 	SUB 'FR'.bec 'FR'.out 160 1 348
$ !	Subtract decoded files (test x reference', with post-filter
$ 	SUB 'FR'-pf.rec 'FR'-pf.out 160 1 348
$ 	SUB 'FR'-pf.bec 'FR'-pf.out 160 1 348
$ !	Subtract enc+dec files (test x reference', with and w/o post-filter
$ 	SUB 'FR'.rek 'FR'.out 160 1 348
$ 	SUB 'FR'-pf.rek 'FR'-pf.out 160 1 348
$ exit	
