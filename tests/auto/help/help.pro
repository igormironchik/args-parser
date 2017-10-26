
TEMPLATE = app
CONFIG += console c++14
QT -= core gui
DEFINES += ARGS_TESTING

INCLUDEPATH = ../../..

include( ../../../config.pri )

SOURCES = main.cpp
