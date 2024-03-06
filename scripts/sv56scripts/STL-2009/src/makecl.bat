@echo off
goto start

rem # ------------------------------------------------------------------
rem # General batch file to build all ITU-T STL modules
rem # using Microsoft Visual C compiler
rem # History:
rem # 01.Jul.99 Created <simao.campos@comsat.com>
rem # ------------------------------------------------------------------

:start
REM ------------------------------------------------
REM ... Test for special targets: test, clean
REM ------------------------------------------------
if "%1" == "test" goto test
if "%1" == "clean" goto clean
if "%1" == "cleantest" goto cleantest
if "%1" == "veryclean" goto veryclean

REM ------------------------------------------------
REM ... Create directory for binaries, if does not exist
REM ------------------------------------------------
if not exist bin\. mkdir bin

REM ------------------------------------------------
REM .. EID module
REM ------------------------------------------------
:eid
if not exist bin\eiddemo.exe call clmk eid eiddemo
if not exist bin\eid-int.exe call clmk eid eid-int
if not exist bin\eid-8k.exe call clmk eid eid-8k
if not exist bin\eid-xor.exe call clmk eid eid-xor
if not exist bin\gen-patt.exe call clmk eid eid-xor
if not exist bin\bs-stats.exe call clmk eid bs-stats
if not exist bin\ep-stats.exe call clmk eid ep-stats
if not exist bin\gen-patt.exe call clmk eid gen-patt
if not exist bin\eid-ev.exe call clmk eid eid-ev

REM ------------------------------------------------
REM .. FIR module
REM ------------------------------------------------
:fir
if not exist bin\firdemo.exe call clmk fir firdemo
if not exist bin\filter.exe call clmk fir filter

REM ------------------------------------------------
REM .. G711 module
REM ------------------------------------------------
:g711
if not exist bin\g711demo.exe call clmk g711 g711demo

REM ------------------------------------------------
REM .. G711 Appendix I PLC module
REM ------------------------------------------------
:g711iplc
if not exist bin\g711iplc.exe call clmk g711iplc g711iplc
if not exist bin\asc2g192.exe call clmk g711iplc asc2g192

REM ------------------------------------------------
REM .. G.722 module
REM ------------------------------------------------
:g722
if not exist bin\g722demo.exe call clmk g722 g722demo
if not exist bin\decg722.exe call clmk g722 decg722
if not exist bin\encg722.exe call clmk g722 encg722


REM ------------------------------------------------
REM .. G.726 module
REM ------------------------------------------------
:g726
if not exist bin\g726demo.exe call clmk g726 g726demo
if not exist bin\vbr-g726.exe call clmk g726 vbr-g726

REM ------------------------------------------------
REM .. G.727 module
REM ------------------------------------------------
:g727
if not exist bin\g727demo.exe call clmk g727 g727demo
if not exist bin\discard.exe call clmk g727 discard

REM ------------------------------------------------
REM .. G.728 module
REM ------------------------------------------------
:g728
if not exist bin\g728fp.exe call clmk g728\g728fixed
cd .. 
if not exist bin\g728.exe call clmk g728\g728float
cd ..

REM ------------------------------------------------
REM .. IIR module
REM ------------------------------------------------
:iir
if not exist bin\pcmdemo.exe call clmk iir pcmdemo
if not exist bin\cirsdemo.exe call clmk iir cirsdemo
if not exist bin\c712demo.exe call clmk iir c712demo

REM ------------------------------------------------
REM .. VSELP module
REM ------------------------------------------------
:is54
if not exist bin\uvselp.exe call clmk is54 uvselp

REM ------------------------------------------------
REM .. MNRU module
REM ------------------------------------------------
:mnru
if not exist bin\mnrudemo.exe call clmk mnru mnrudemo

REM ------------------------------------------------
REM .. RPE-LTP module
REM ------------------------------------------------
:rpeltp
if not exist bin\rpedemo.exe call clmk rpeltp rpedemo

REM ------------------------------------------------
REM .. STEREOOP module
REM ------------------------------------------------
:stereoop
if not exist bin\stereoop.exe call clmk stereoop stereoop

REM ------------------------------------------------
REM .. Speech voltmeter module
REM ------------------------------------------------
:sv56
if not exist bin\sv56demo.exe call clmk sv56 sv56demo
if not exist bin\actlev.exe call clmk sv56 actlev

REM ------------------------------------------------
REM .. Reverb module
REM ------------------------------------------------
:reverb
if not exist bin\reverb.exe call clmk reverb reverb

REM ------------------------------------------------
REM .. Bitstream truncation tool
REM ------------------------------------------------
:truncate
if not exist bin\truncate.exe call clmk truncate truncate

REM ------------------------------------------------
REM .. Frequency response measure tool
REM ------------------------------------------------
:freqresp
if not exist bin\freqresp.exe call clmk freqresp freqresp

REM ------------------------------------------------
REM .. Utility module
REM ------------------------------------------------
:utl
if not exist bin\scaldemo.exe call clmk utl scaldemo
if not exist bin\spdemo.exe call clmk utl spdemo

REM ------------------------------------------------
REM .. Unsupported module
REM ------------------------------------------------
:unsup
if not exist bin\add.exe call clmk unsup add
if not exist bin\asc2bin.exe call clmk unsup asc2bin
if not exist bin\astrip.exe call clmk unsup astrip
if not exist bin\bin2asc.exe call clmk unsup bin2asc
if not exist bin\chr2sh.exe call clmk unsup chr2sh
if not exist bin\cf.exe call clmk unsup cf
if not exist bin\concat.exe call clmk unsup concat
if not exist bin\crc.exe call clmk unsup crc
if not exist bin\df.exe call clmk unsup df
if not exist bin\endian.exe call clmk unsup endian
if not exist bin\fdelay.exe call clmk unsup fdelay
if not exist bin\measure.exe call clmk unsup measure
if not exist bin\oper.exe call clmk unsup oper
if not exist bin\sb.exe call clmk unsup sb
if not exist bin\sh2chr.exe call clmk unsup sh2chr
if not exist bin\sine.exe call clmk unsup sine
if not exist bin\sub.exe call clmk unsup sub 
if not exist bin\xdecode.exe call clmk unsup xdecode
if not exist bin\xencode.exe call clmk unsup xencode
if not exist bin\g729e_convert_synch.exe call clmk unsup g729e_convert_synch
if not exist bin\rm.bat xcopy unsup\rm.bat bin
if not exist bin\swapover.bat xcopy unsup\swapover.bat bin
goto end

REM ------------------------------------------------
REM .. Test modules
REM ------------------------------------------------
:test

REM -- Special case for the EID module, since it needs 
REM -- compilation with -DPORT_TEST for portability tests
cd eid
nmake /C -fmakefile.cl XXX=-DPORT_TEST veryclean all
nmake /C -fmakefile.cl test > winntcl.log 2>&1
nmake /C -fmakefile.cl FMT=-byte test-patt >> winntcl.log 2>&1
nmake /C -fmakefile.cl FMT=-bit  test-patt >> winntcl.log 2>&1
nmake /C -fmakefile.cl veryclean 
cd ..

call clmk fir       test > fir\winntcl.log 2>&1
call clmk g711      test > g711\winntcl.log 2>&1
call clmk g711iplc  test > g711iplc\winntcl.log 2>&1
call clmk g722      test > g722\winntcl.log 2>&1
call clmk g726      test > g726\winntcl.log 2>&1
call clmk g727      test > g727\winntcl.log 2>&1
call clmk iir       test > iir\winntcl.log 2>&1
call clmk is54\bin  test > is54\bin\winntcl.log 2>&1
cd ..
call clmk mnru      test > mnru\winntcl.log 2>&1
call clmk rpeltp\ts test > rpeltp\ts\winntcl.log 2>&1
cd ..
call clmk sv56      test > sv56\winntcl.log 2>&1
call clmk utl       test > utl\winntcl.log 2>&1
call clmk unsup     test > unsup\winntcl.log 2>&1
call clmk reverb	test > reverb\winntcl.log 2>&1
call clmk freqresp	test > freqresp\winntcl.log 2>&1
call clmk truncate	test > truncate\winntcl.log 2>&1
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:clean
call clmk eid clean

call clmk fir clean
call clmk g711 clean
call clmk g711iplc clean
call clmk g722 clean
call clmk g726 clean
call clmk g727 clean
call clmk iir clean
call clmk is54 clean
call clmk mnru clean
call clmk rpeltp clean
call clmk sv56 clean
call clmk reverb clean
call clmk freqresp clean
call clmk truncate clean
call clmk utl clean
call clmk unsup clean
goto end

REM ------------------------------------------------
REM .. Clean test files in modules
REM ------------------------------------------------
:cleantest
call clmk eid cleantest
call clmk fir cleantest
call clmk g711 cleantest
call clmk g711iplc cleantest
call clmk g722 cleantest
call clmk g726 cleantest
call clmk g727 cleantest
call clmk iir cleantest
call clmk is54\bin cleantest
cd ..
call clmk mnru cleantest
call clmk rpeltp\ts cleantest
cd ..
call clmk sv56 cleantest
call clmk reverb cleantest
call clmk freqresp cleantest
call clmk truncate cleantest
call clmk utl cleantest
call clmk unsup cleantest
rm -f bin/*
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:veryclean
call clmk eid veryclean
call clmk fir veryclean
call clmk g711 veryclean
call clmk g711iplc veryclean
call clmk g722 veryclean
call clmk g726 veryclean
call clmk g727 veryclean
call clmk iir veryclean
call clmk is54 veryclean
call clmk is54\bin veryclean
cd ..
call clmk mnru veryclean
call clmk rpeltp veryclean
call clmk rpeltp\ts veryclean
cd ..
call clmk sv56 veryclean
call clmk reverb veryclean
call clmk freqresp veryclean
call clmk truncate veryclean
call clmk utl veryclean
call clmk unsup veryclean
rm -f bin/*
goto end

:end
