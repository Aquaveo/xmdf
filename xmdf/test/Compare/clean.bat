echo off

REM ### Clean Directories

mkdir "test_out"
cd "test_out"

dir /w *.exe
dir /w *.dll
dir /w *.htm
dir /w *.h5
dir /w *.txt
echo ================================================================
echo = THE ABOVE FILES WILL BE DELETED BEFORE TESTING
echo ================================================================
REM pause
del /q *.exe
del /q *.dll
del /q *.htm
del /q *.h5
del /q *.txt
