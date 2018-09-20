@echo off
set XMS_VERSION=1.0.0
set CONAN_VISUAL_VERSIONS=14
set CONAN_REFERENCE=xmdf/%XMS_VERSION%
set CONAN_USERNAME=aquaveo
set CONAN_CHANNEL=testing
set CONAN_GCC_VERSIONS=5
set CONAN_ARCHS=x86_64
set CONAN_BUILD_TYPES=Release
REM set CONAN_USERNAME=aquaveo
REM set CONAN_PASSWORD=aquaveo
REM set CONAN_UPLOAD=https://conan.aquaveo.com

REM export CONAN_UPLOAD="<conanserver>"
set | findstr "XMS"
set | findstr "CONAN"
