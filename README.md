[![Build Status](https://travis-ci.org/Aquaveo/xmsgridtrace.svg?branch=master)](https://travis-ci.org/Aquaveo/xmsgridtrace)
[![Build status](https://ci.appveyor.com/api/projects/status/tkgmhrs31cc4l3ph?svg=true)](https://ci.appveyor.com/project/Aquaveo/xmsgridtrace)

XMDF
========
Library providing a standard format for geometry and time data storage in the HDF5 format.

Prerequisites
--------------
In order to build this library, you must first have a few things installed.
1. [Conan](https://conan.io)
2. [CMake](https://cmake.org)

Building
--------
Once you have cloned the repository, navigate to the `build` directory. All
of the following commands will be run from that directory unless otherwise 
noted.

1. Get External Libraries
```
    conan install .. 
```

2. Configure CMake
```
    cmake ..
```

3. Build 
```
    make
```

For Windows:

1. Get External Libraries
```
    conan install -pr ..\dev\xmsprofile_debug ..
``` 

2. Configure CMake
```
    cmake .. -G "Visual Studio 12 2013 Win64" -DXMS_BUILD=TRUE -DBUILD_TESTING=TRUE
```

3. Build - Build from generated solution file.

Documentation
-------------

See: [xmdf documentation](https://aquaveo.github.io/xmdf/)
