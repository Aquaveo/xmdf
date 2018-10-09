echo off

echo.
echo OPTIONALLY DELETE BUILD FOLDER BEFORE MAKING IT NEXT: build_cpp
pause

echo on
mkdir build_cpp
pushd .
cd build_cpp
REM conan install -pr ..\xmsprofile_debug ..\.. --build missing
conan install -pr ..\xmsprofile_debug ..\.. --build missing
popd
echo off

echo.
echo ABORT IF ERRORS ABOVE
pause

echo on
pushd .
cd build_cpp
cmake ..\.. -G"Visual Studio 12 2013 Win64" -DIS_PYTHON_BUILD=False -DBUILD_TESTING=True
popd
echo off

echo.
echo ABORT IF ERRORS ABOVE
echo.
echo OPEN VS2015 FILE build_cpp\*.sln
echo BUILD AND RUN PROJECT runner FOR DEBUG x64
pause
