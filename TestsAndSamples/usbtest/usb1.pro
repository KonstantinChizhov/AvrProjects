# -------------------------------------------------
# Project created by QtCreator 2009-08-07T22:30:37
# -------------------------------------------------
QT -= core \
    gui
TARGET = usb1
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += opendevice.c \
    set-led.cpp \
    comport.cpp
HEADERS += usb.h \
    opendevice.h \
    ComPort.h
LIBS += -lusb
