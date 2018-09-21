"""
Xmdf Conanfile and Support
"""
import os
from conans import ConanFile, CMake
from conans.errors import ConanException


class XmdfConan(ConanFile):
    """Xmdf Conanfile"""
    name = "xmdf"
    version = None
    license = "See LICENSE file for Software License"
    url = "https://github.com/Aquaveo/xmdf"
    description = "Library providing a standard format for geometry and time data storage in the HDF5 format."
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "xms": [True, False],
        "testing": [True, False],
    }
    default_options = "xms=False", "testing=False"
    generators = "cmake"
    build_requires = "cxxtest/4.4@aquaveo/stable"
    exports = "CMakeLists.txt", "LICENSE", "test_files/*"
    exports_sources = "xmdf/*", "test_files/*"

    def configure(self):
        # Set version dynamically using XMS_VERSION env variable.
        self.version = self.env.get('XMS_VERSION', 'master')

        # Raise ConanExceptions for Unsupported Versions
        s_os = self.settings.os
        s_compiler = self.settings.compiler
        s_compiler_version = self.settings.compiler.version

    def requirements(self):
        """Requirements"""

        # Use the dev version of XMSCore, XMSInterp, XMSGrid, XMSExtractor
        self.requires("hdf5/1.8.1@aquaveo/stable")

    def build(self):
        cmake = CMake(self)

        if self.settings.compiler == 'Visual Studio' \
           and self.settings.compiler.version == "12":
            cmake.definitions["XMS_BUILD"] = self.options.xms

        # It would be nice to not
        # have tests in release code. Thus, if we want to run tests, we will
        # build a test version , run the tests, and then (on
        # sucess) rebuild the library without tests.
        cmake.definitions["BUILD_TESTING"] = self.options.testing
        cmake.definitions["XMDF_TEST_PATH"] = "test_files"
        cmake.configure(source_folder=".")
        cmake.build()

        if self.options.testing:
            print("***********(0.0)*************")
            try:
                cmake.test()
            except ConanException:
                raise
            finally:
                if os.path.isfile("TEST-cxxtest.xml"):
                    with open("TEST-cxxtest.xml", "r") as f:
                        for line in f.readlines():
                            no_newline = line.strip('\n')
                            print(no_newline)
                print("***********(0.0)*************")

    def package(self):
        self.copy("*.h", dst="include/xmdf", src="xmdf")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("license", dst="licenses", ignore_case=True, keep_path=False)

    def package_info(self):
        if self.settings.build_type == 'Debug':
            self.cpp_info.libs = ["xmdf_d"]
        else:
            self.cpp_info.libs = ["xmdf"]
