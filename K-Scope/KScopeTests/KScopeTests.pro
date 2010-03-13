# -------------------------------------------------
# Project created by QtCreator 2009-08-26T21:56:33
# -------------------------------------------------
QT -= gui
TARGET = KScopeTests
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    ../Host/devicebase.cpp \
    ../Host/demodevice.cpp \
    ../Host/k_scope.cpp \
    ../Host/exception.cpp \
    ../Host/opendevice.c
HEADERS += ../Host/devicebase.h \
    ../Host/demodevice.h \
    ../Host/k_scope.h \
    ../Host/exception.h \
    ../Host/opendevice.h
LIBS += -lusb
