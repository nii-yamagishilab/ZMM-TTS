@echo off
goto start

---------------------------------------------------------------------------
This batch file is to make a deletion of multiple files/wildcard files
in DOS; emulates the most basic behaivour of Unix rm. No, no options,
no recursions. If you want that, better find a "real" rm :-)

Simao - 12.Feb.95 - in a cloudy day in Geneva
---------------------------------------------------------------------------

:start
if "%1"=="-f" shift
if "%1"=="-F" shift

:again
if "%1"=="" goto quit
 if exist %1 del %1
 shift
goto again

:quit
 exit