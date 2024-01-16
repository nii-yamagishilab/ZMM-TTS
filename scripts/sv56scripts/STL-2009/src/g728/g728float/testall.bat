@echo off
set TV="..\testvector"

echo Encoder Test
set I=0
:ENC
if %I%==4 set I=6
if %I%==3 set I=4
if %I%==2 set I=3
if %I%==1 set I=2
If %I%==0 set I=1
echo Encoding Vector %I%
g728 enc %TV%\IN%I%.BIN bstream.out
fc /b %TV%\INCW%I%.BIN bstream.out

if not %I%==6 goto ENC

echo Decoder Test
set I=0
:DEC
if %I%==5 set I=6
if %I%==4 set I=5
if %I%==3 set I=4
if %I%==2 set I=3
if %I%==1 set I=2
If %I%==0 set I=1
echo Decoding Vector %I%
g728 -nopostf dec %TV%\CW%I%.BIN speech.out
fc /b %TV%\OUTA%I%.BIN speech.out

if not %I%==6 goto DEC

echo Postfilter Test
g728 dec %TV%\CW4.BIN speech.out
fc /b %TV%\OUTB4.BIN speech.out

del *.out
set TV=
set I=
