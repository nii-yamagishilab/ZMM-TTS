@echo off
goto start

rem # ------------------------------------------------------------------
rem # General batch file to build all ITU-T STL modules
rem # using Borland [bt]cc compiler
rem # History:
rem # 28.Mar.2000 Created <simao.campos@labs.comsat.com>
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
if not exist bin\eiddemo.exe call tccmk eid eiddemo
if not exist bin\eid-int.exe call tccmk eid eid-int
if not exist bin\eid-xor.exe call tccmk eid eid-xor
if not exist bin\bs-stats.exe call tccmk eid bs-stats
if not exist bin\ep-stats.exe call tccmk eid ep-stats
if not exist bin\gen-patt.exe call tccmk eid gen-patt

REM ------------------------------------------------
REM .. FIR module
REM ------------------------------------------------
:fir
if not exist bin\firdemo.exe call tccmk fir firdemo
if not exist bin\filter.exe call tccmk fir filter

REM ------------------------------------------------
REM .. G711 module
REM ------------------------------------------------
:g711
if not exist bin\g711demo.exe call tccmk g711 g711demo

REM ------------------------------------------------
REM .. G.722 module
REM ------------------------------------------------
:g722
if not exist bin\g722demo.exe call tccmk g722 g722demo

REM ------------------------------------------------
REM .. G.726 module
REM ------------------------------------------------
:g726
if not exist bin\g726demo.exe call tccmk g726 g726demo
if not exist bin\vbr-g726.exe call tccmk g726 vbr-g726

REM ------------------------------------------------
REM .. G.727 module
REM ------------------------------------------------
:g727
if not exist bin\g727demo.exe call tccmk g727 g727demo

REM ------------------------------------------------
REM .. IIR module
REM ------------------------------------------------
:iir
if not exist bin\pcmdemo.exe  call tccmk iir pcmdemo
if not exist bin\cirsdemo.exe call tccmk iir cirsdemo
if not exist bin\c712demo.exe call tccmk iir c712demo

REM ------------------------------------------------
REM .. VSELP module
REM ------------------------------------------------
:is54
if not exist bin\uvselp.exe call tccmk is54 uvselp

REM ------------------------------------------------
REM .. MNRU module
REM ------------------------------------------------
:mnru
if not exist bin\mnrudemo.exe call tccmk mnru mnrudemo

REM ------------------------------------------------
REM .. RPE-LTP module
REM ------------------------------------------------
:rpeltp
if not exist bin\rpedemo.exe call tccmk rpeltp rpedemo

REM ------------------------------------------------
REM .. Speech voltmeter module
REM ------------------------------------------------
:sv56
if not exist bin\sv56demo.exe call tccmk sv56 sv56demo
if not exist bin\actlev.exe call tccmk sv56 actlev

REM ------------------------------------------------
REM .. Utility module
REM ------------------------------------------------
:utl
if not exist bin\scaldemo.exe call tccmk utl scaldemo
if not exist bin\spdemo.exe call tccmk utl spdemo

REM ------------------------------------------------
REM .. Unsupported module
REM ------------------------------------------------
:unsup
if not exist bin\add.exe call tccmk unsup add
if not exist bin\asc2bin.exe call tccmk unsup asc2bin
if not exist bin\astrip.exe call tccmk unsup astrip
if not exist bin\bin2asc.exe call tccmk unsup bin2asc
if not exist bin\chr2sh.exe call tccmk unsup chr2sh
if not exist bin\cf.exe call tccmk unsup cf
if not exist bin\concat.exe call tccmk unsup concat
if not exist bin\crc.exe call tccmk unsup crc
if not exist bin\df.exe call tccmk unsup df
if not exist bin\endian.exe call tccmk unsup endian
if not exist bin\fdelay.exe call tccmk unsup fdelay
if not exist bin\measure.exe call tccmk unsup measure
if not exist bin\oper.exe call tccmk unsup oper
if not exist bin\sb.exe call tccmk unsup sb
if not exist bin\sh2chr.exe call tccmk unsup sh2chr
if not exist bin\sine.exe call tccmk unsup sine
if not exist bin\sub.exe call tccmk unsup sub 
if not exist bin\xdecode.exe call tccmk unsup xdecode
if not exist bin\xencode.exe call tccmk unsup xencode
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
make -fmakefile.tcc XXX=-DPORT_TEST veryclean all
make -fmakefile.tcc test > msdostcc.log
make -fmakefile.tcc FMT=-byte test-patt >> msdostcc.log
make -fmakefile.tcc FMT=-bit  test-patt >> msdostcc.log
make -fmakefile.tcc veryclean 
cd ..

call tccmk fir test       > fir\msdostcc.log
call tccmk g711 test      > g711\msdostcc.log
call tccmk g722 test      > g722\msdostcc.log
call tccmk g726 test      > g726\msdostcc.log
call tccmk g727 test      > g727\msdostcc.log
call tccmk iir test       > iir\msdostcc.log
call tccmk is54\bin test  > is54\bin\msdostcc.log
cd ..
call tccmk mnru test      > mnru\msdostcc.log
call tccmk rpeltp\ts test > rpeltp\ts\msdostcc.log
cd ..
call tccmk sv56 test      > sv56\msdostcc.log
call tccmk utl test       > utl\msdostcc.log
call tccmk unsup test     > unsup\msdostcc.log
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:clean
call tccmk eid clean
call tccmk fir clean
call tccmk g711 clean
call tccmk g722 clean
call tccmk g726 clean
call tccmk g727 clean
call tccmk iir clean
call tccmk is54 clean
call tccmk mnru clean
call tccmk rpeltp clean
call tccmk sv56 clean
call tccmk utl clean
call tccmk unsup clean
goto end

REM ------------------------------------------------
REM .. Clean test files in modules
REM ------------------------------------------------
:cleantest
call tccmk eid cleantest
call tccmk fir cleantest
call tccmk g711 cleantest
call tccmk g722 cleantest
call tccmk g726 cleantest
call tccmk g727 cleantest
call tccmk iir cleantest
call tccmk is54\bin cleantest
cd ..
call tccmk mnru cleantest
call tccmk rpeltp\ts cleantest
cd ..
call tccmk sv56 cleantest
call tccmk utl cleantest
call tccmk unsup cleantest
rm -f bin/*
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:veryclean
call tccmk eid veryclean
call tccmk fir veryclean
call tccmk g711 veryclean
call tccmk g722 veryclean
call tccmk g726 veryclean
call tccmk g727 veryclean
call tccmk iir veryclean
call tccmk is54 veryclean
call tccmk is54\bin veryclean
cd ..
call tccmk mnru veryclean
call tccmk rpeltp veryclean
call tccmk rpeltp\ts veryclean
cd ..
call tccmk sv56 veryclean
call tccmk utl veryclean
call tccmk unsup veryclean
rm -f bin/*.*
goto end

:end
