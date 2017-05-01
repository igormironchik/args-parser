
TEMPLATE = app
TARGET = args.help_with_commands
CONFIG += console c++14
QT -= core gui
CONFIG -= qt

include( ../../Args/Args.pri )

INCLUDEPATH = ../..

SOURCES = main.cpp