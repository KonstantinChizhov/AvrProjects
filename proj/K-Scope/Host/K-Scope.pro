# -------------------------------------------------
# Project created by QtCreator 2009-08-19T00:36:06
# -------------------------------------------------
QT += xml
TARGET = K-Scope
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    mytransform.cpp \
    k_scope.cpp \
    graph.cpp \
    gr_data.cpp \
    opendevice.c \
    exception.cpp \
    devicebase.cpp \
    demodevice.cpp
HEADERS += dialog.h \
    mytransform.h \
    k_scope.h \
    graph.h \
    gr_data.h \
    opendevice.h \
    exception.h \
    devicebase.h \
    demodevice.h
FORMS += dialog.ui
LIBS += -lusb
