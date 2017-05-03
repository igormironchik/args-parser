
TEMPLATE = app
TARGET = args.help
CONFIG += console c++14
QT -= core gui
CONFIG -= qt

include( ../../Args/Args.pri )
include( ../../config.pri )

INCLUDEPATH = ../..

SOURCES = main.cpp
