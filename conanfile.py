from conans import ConanFile


class ArgsConan(ConanFile):
    name = "Args"
    version = "5.0.1.4"
    url = "https://github.com/igormironchik/args-parser"
    license = "MIT"
    description = "Small C++ header-only library for parsing command line arguments."

    def package(self):
        self.copy("COPYING", src=".", dst=".")
        self.copy("*.hpp", src="./Args", dst="./Args")
