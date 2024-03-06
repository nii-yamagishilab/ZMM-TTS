@echo off
cd %1
echo *** Processing %1\%2 ***
nmake /C -fmakefile.cl %2
if not errorlevel 0 pause
if exist %2.exe copy %2.exe ..\bin
del *.exe *.obj

cd ..
