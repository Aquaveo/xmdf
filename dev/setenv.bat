@echo off
REM This file restricts conan local builds to the following configurations:
set XMS_VERSION=1.0.0
set CONAN_VISUAL_VERSIONS=12
set CONAN_REFERENCE=xmdf/%XMS_VERSION%
set CONAN_USERNAME=aquaveo
set CONAN_CHANNEL=testing
REM set CONAN_GCC_VERSIONS=5
set CONAN_ARCHS=x86_64
REM set CONAN_BUILD_TYPES=Release
REM set CONAN_USERNAME=aquaveo
REM set CONAN_PASSWORD=aquaveo
REM set CONAN_UPLOAD=https://conan.aquaveo.com

REM export CONAN_UPLOAD="<conanserver>"
set | findstr "XMS"
set | findstr "CONAN"
