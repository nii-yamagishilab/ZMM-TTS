@echo off

REM Catch bad usage
if "%1"=="" goto error

REM Treat options
if not "%1"=="-if" goto start

REM Option -if
:optif
set opt=%1 %2
shift
shift
goto start

REM Actual byte-swapping - use "for" to expand wildcards in %1
:start
if "%1"=="" goto quit
for %%f in (%1) do sb %opt% -over %%f
rem sb %opt% -over %1
shift
goto start

:error
REM Display usage
echo Usage:
echo  %0 [-if xxx] file [file ...]
echo where xxx is one the conditional byteswapping option: big, little, or any
:quit

