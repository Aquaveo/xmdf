#!/usr/bin/env bash
export XMS_VERSION="99.99.99"
export CONAN_REFERENCE="xmdf/${XMS_VERSION}"
export CONAN_USERNAME="aquaveo"
export CONAN_CHANNEL="stable"
export CONAN_GCC_VERSIONS="6"
export CONAN_ARCHS="x86_64"
#export CONAN_BUILD_TYPES="Release"
#export CONAN_UPLOAD="1"
printenv | grep XMS
printenv | grep CONAN
