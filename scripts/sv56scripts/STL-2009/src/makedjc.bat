@echo off
goto start

rem # ------------------------------------------------------------------
rem # General batch file to build all ITU-T STL modules
rem # using DJGPP MSDOS port of gcc
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
if not exist bin\eiddemo.exe call gccmk eid eiddemo
if not exist bin\eid-int.exe call gccmk eid eid-int
if not exist bin\eid-xor.exe call gccmk eid eid-xor
if not exist bin\bs-stats.exe call gccmk eid bs-stats
if not exist bin\ep-stats.exe call gccmk eid ep-stats
if not exist bin\gen-patt.exe call gccmk eid gen-patt

REM ------------------------------------------------
REM .. FIR module
REM ------------------------------------------------
:fir
if not exist bin\firdemo.exe call gccmk fir firdemo
if not exist bin\filter.exe call gccmk fir filter

REM ------------------------------------------------
REM .. G711 module
REM ------------------------------------------------
:g711
if not exist bin\g711demo.exe call gccmk g711 g711demo

REM ------------------------------------------------
REM .. G.722 module
REM ------------------------------------------------
:g722
if not exist bin\g722demo.exe call gccmk g722 g722demo

REM ------------------------------------------------
REM .. G.726 module
REM ------------------------------------------------
:g726
if not exist bin\g726demo.exe call gccmk g726 g726demo
if not exist bin\vbr-g726.exe call gccmk g726 vbr-g726

REM ------------------------------------------------
REM .. G.727 module
REM ------------------------------------------------
:g727
if not exist bin\g727demo.exe call gccmk g727 g727demo

REM ------------------------------------------------
REM .. IIR module
REM ------------------------------------------------
:iir
if not exist bin\pcmdemo.exe  call gccmk iir pcmdemo
if not exist bin\cirsdemo.exe call gccmk iir cirsdemo
if not exist bin\c712demo.exe call gccmk iir c712demo

REM ------------------------------------------------
REM .. VSELP module
REM ------------------------------------------------
:is54
if not exist bin\uvselp.exe call gccmk is54 uvselp

REM ------------------------------------------------
REM .. MNRU module
REM ------------------------------------------------
:mnru
if not exist bin\mnrudemo.exe call gccmk mnru mnrudemo

REM ------------------------------------------------
REM .. RPE-LTP module
REM ------------------------------------------------
:rpeltp
if not exist bin\rpedemo.exe call gccmk rpeltp rpedemo

REM ------------------------------------------------
REM .. Speech voltmeter module
REM ------------------------------------------------
:sv56
if not exist bin\sv56demo.exe call gccmk sv56 sv56demo
if not exist bin\actlev.exe call gccmk sv56 actlev

REM ------------------------------------------------
REM .. Utility module
REM ------------------------------------------------
:utl
if not exist bin\scaldemo.exe call gccmk utl scaldemo
if not exist bin\spdemo.exe call gccmk utl spdemo

REM ------------------------------------------------
REM .. Unsupported module
REM ------------------------------------------------
:unsup
if not exist bin\add.exe call gccmk unsup add
if not exist bin\asc2bin.exe call gccmk unsup asc2bin
if not exist bin\astrip.exe call gccmk unsup astrip
if not exist bin\bin2asc.exe call gccmk unsup bin2asc
if not exist bin\chr2sh.exe call gccmk unsup chr2sh
if not exist bin\cf.exe call gccmk unsup cf
if not exist bin\concat.exe call gccmk unsup concat
if not exist bin\crc.exe call gccmk unsup crc
if not exist bin\df.exe call gccmk unsup df
if not exist bin\endian.exe call gccmk unsup endian
if not exist bin\fdelay.exe call gccmk unsup fdelay
if not exist bin\measure.exe call gccmk unsup measure
if not exist bin\oper.exe call gccmk unsup oper
if not exist bin\sb.exe call gccmk unsup sb
if not exist bin\sh2chr.exe call gccmk unsup sh2chr
if not exist bin\sine.exe call gccmk unsup sine
if not exist bin\sub.exe call gccmk unsup sub 
if not exist bin\xdecode.exe call gccmk unsup xdecode
if not exist bin\xencode.exe call gccmk unsup xencode
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
gmake -fmakefile.djc XXX=-DPORT_TEST veryclean all
gmake -fmakefile.djc test > msdosgcc.log
gmake -fmakefile.djc FMT=-byte test-patt >> msdosgcc.log
gmake -fmakefile.djc FMT=-bit  test-patt >> msdosgcc.log
gmake -fmakefile.djc veryclean 
cd ..

call gccmk fir test       > fir\msdosgcc.log
call gccmk g711 test      > g711\msdosgcc.log
call gccmk g722 test      > g722\msdosgcc.log
call gccmk g726 test      > g726\msdosgcc.log
call gccmk g727 test      > g727\msdosgcc.log
call gccmk iir test       > iir\msdosgcc.log
call gccmk is54\bin test  > is54\bin\msdosgcc.log
cd ..
call gccmk mnru test      > mnru\msdosgcc.log
call gccmk rpeltp\ts test > rpeltp\ts\msdosgcc.log
cd ..
call gccmk sv56 test      > sv56\msdosgcc.log
call gccmk utl test       > utl\msdosgcc.log
call gccmk unsup test     > unsup\msdosgcc.log
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:clean
call gccmk eid clean
call gccmk fir clean
call gccmk g711 clean
call gccmk g722 clean
call gccmk g726 clean
call gccmk g727 clean
call gccmk iir clean
call gccmk is54 clean
call gccmk mnru clean
call gccmk rpeltp clean
call gccmk sv56 clean
call gccmk utl clean
call gccmk unsup clean
goto end

REM ------------------------------------------------
REM .. Clean test files in modules
REM ------------------------------------------------
:cleantest
call gccmk eid cleantest
call gccmk fir cleantest
call gccmk g711 cleantest
call gccmk g722 cleantest
call gccmk g726 cleantest
call gccmk g727 cleantest
call gccmk iir cleantest
call gccmk is54\bin cleantest
cd ..
call gccmk mnru cleantest
call gccmk rpeltp\ts cleantest
cd ..
call gccmk sv56 cleantest
call gccmk utl cleantest
call gccmk unsup cleantest
rm -f bin/*
goto end

REM ------------------------------------------------
REM .. Clean files in modules
REM ------------------------------------------------
:veryclean
call gccmk eid veryclean
call gccmk fir veryclean
call gccmk g711 veryclean
call gccmk g722 veryclean
call gccmk g726 veryclean
call gccmk g727 veryclean
call gccmk iir veryclean
call gccmk is54 veryclean
call gccmk is54\bin veryclean
cd ..
call gccmk mnru veryclean
call gccmk rpeltp veryclean
call gccmk rpeltp\ts veryclean
cd ..
call gccmk sv56 veryclean
call gccmk utl veryclean
call gccmk unsup veryclean
rm -f bin/*
goto end

:end
