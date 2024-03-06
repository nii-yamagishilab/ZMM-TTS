rem @echo off
rem Process by either makefile.tcc or makefile.bcc
cd %1
echo *** Processing %1\%2 ***
if exist makefile.tcc make -fmakefile.tcc %2
if exist makefile.bcc make -fmakefile.bcc %2

if not exist %2.exe goto quit
copy %2.exe ..\bin
rem del %2.exe > nul

:quit
cd ..
