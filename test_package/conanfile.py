from conans import ConanFile, CMake, tools
import os


class XmsextractorTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is in "test_package"
        print('TEST PACKAGE CURRENT WORKING DIRECTORY: {}'.format(os.getcwd()))
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        # Run tests only for x86_64 builds
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%sexample" % os.sep)
        else:
            print("Cross Building: Skipping tests.")
            print(self.settings.arch.value)
            print(self.settings.build_type.value)
