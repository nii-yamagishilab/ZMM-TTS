@echo off
cd %1
echo *** Processing %1\%2 ***
gmake -fmakefile.djc %2

if not exist %2 goto quit
coff2exe %2
copy %2.exe ..\bin
rem del %2.exe > nul

:quit
cd ..
