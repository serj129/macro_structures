#-------------------------------------------------
#
# Project created by QtCreator 2011-04-28T13:39:14
#
#-------------------------------------------------

QT       += core gui sql svg

TARGET = structures3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plotter.cpp \
    neuron.cpp \
    cellscene.cpp \
    struct.cpp \
    configwindow.cpp \
    struct3d.cpp \
    point3d.cpp \
    tethrascene.cpp

HEADERS  += mainwindow.h \
    plotter.h \
    neuron.h \
    cellscene.h \
    struct.h \
    configwindow.h \
    struct3d.h \
    point3d.h \
    tethrascene.h \
    abstractstructure.h

FORMS    += mainwindow.ui \
    struct.ui \
    configwindow.ui \
    struct3d.ui

include (qwt.prf)
