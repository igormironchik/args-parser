
TEMPLATE = app
CONFIG += console c++14
QT -= core gui
DEFINES += ARGS_TESTING

INCLUDEPATH = ../../.. ../../../3rdparty/doctest

include( ../../../config.pri )

SOURCES = main.cpp
