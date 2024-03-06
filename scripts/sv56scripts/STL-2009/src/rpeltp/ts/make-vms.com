$goto start
$!-----------------------------------------------------------------------------
$!	VMS DCL for compiling and testing the RPE-LTP implementation
$!	The executable must be defined by verb RPEDEMO below.  
$!
$!	Usage:
$!	@make-vms 
$!
$!	Implemented by <simao@ctd.comsat.com> -- 01.Feb.95
$!-----------------------------------------------------------------------------
$start:
$ def d20:[tdsimao.rpe.c]
$ verb rpedemo rpedemo
$ def [.ts]
$ file := "sine"
$
$!Process all files
$ DIFF = "cf -h"
$ if ("''P1'".EQS."TEST") then goto only_test
$ if ("''P1'".EQS."CLEAN") then goto delete_proc
$ 
$ ! ==============================================================
$ ! Generate test file
$ g711demo a lilo 'file'.lin 'file'.a 160 1 20
$ g711demo a loli 'file'.a 'file'.a-a 160 1 20
$ g711demo u lilo 'file'.lin 'file'.u 160 1 20
$ g711demo u loli 'file'.u 'file'.u-u 160 1 20
$ 
$ ! ==============================================================
$ ! Tests for u-law
$ ! ... testing encode-only
$ rpedemo -enc 'file'.u-u 'file'.u-u_rpe
$ rpedemo -enc -u 'file'.u 'file'.u_rpe
$ cf -h 'file'.u-u_rpe 'file'.u_rpe 160
$ 
$ ! ... testing decode-only
$ rpedemo -dec 'file'.u-u_rpe 'file'.u-u_l
$ rpedemo -dec -u 'file'.u-u_rpe 'file'.u-u_u
$ g711demo u lilo 'file'.u-u_l 'file'.u-u_l_u 160 1 20
$ cf -h 'file'.u-u_u 'file'.u-u_l_u 160
$ 
$ ! ... testing short-cut
$ rpedemo 'file'.u-u 'file'.u-u_l
$ rpedemo -u 'file'.u 'file'.u-u_u
$ g711demo u lilo 'file'.u-u_l 'file'.u-u_l_u 160 1 20
$ cf -h 'file'.u-u_u 'file'.u-u_l_u 160
$ 
$ 
$ ! ==============================================================
$ ! Tests for A-law
$ ! ... testing encode-only
$ rpedemo -enc 'file'.a-a 'file'.a-a_rpe
$ rpedemo -enc -a 'file'.a 'file'.a_rpe
$ cf -h 'file'.a-a_rpe 'file'.a_rpe 160
$ 
$ ! ... testing decode-only
$ rpedemo -dec 'file'.a-a_rpe 'file'.a-a_l
$ rpedemo -dec -a 'file'.a-a_rpe 'file'.a-a_a
$ g711demo a lilo 'file'.a-a_l 'file'.a-a_l_a 160 1 20
$ cf -h 'file'.a-a_a 'file'.a-a_l_a 160
$ 
$ ! ... testing short-cut
$ rpedemo 'file'.a-a 'file'.a-a_l
$ rpedemo -a 'file'.a 'file'.a-a_a
$ g711demo a lilo 'file'.a-a_l 'file'.a-a_l_a 160 1 20
$ cf -h 'file'.a-a_a 'file'.a-a_l_a 160
$ exit
$ 
$!========================================
$ delete_proc:
$ delete/log 'file'.u*.*,'file'.a*.*
$ exit 
