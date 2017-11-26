
TEMPLATE = subdirs

SUBDIRS = samples \
	tests

OTHER_FILES = README.md \
	doc/mainpage.dox \
	doc/example.cpp \
	doc/help.txt \
	.travis.yml \
	doc/Doxyfile \
	doc/oneline.cpp \
	ArgsConfig.cmake.in

include( Args/Args.pri )
