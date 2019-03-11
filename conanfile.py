from conans import ConanFile


class ArgsConan(ConanFile):
    name = "Args"
    version = "5.0.1.5"
    url = "https://github.com/igormironchik/args-parser.git"
    license = "MIT"
    description = "Small C++ header-only library for parsing command line arguments."
    exports = "Args/*.hpp", "COPYING"

    def package(self):
        self.copy("COPYING", src=".", dst=".")
        self.copy("*.hpp", src="Args", dst="Args")
        
    def package_info(self):
        self.cpp_info.includedirs = ["."]
