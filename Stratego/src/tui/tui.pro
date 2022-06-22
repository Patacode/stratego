TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../config.pri)

HEADERS += \
    action.h \
    ansiColor.h \
    asker.h \
    command.h \
    console.h \
    vcstuff.h

SOURCES += \
        action.cpp \
        ansiColor.cpp \
        asker.cpp \
        command.cpp \
        console.cpp \
        controller.cpp \
        main.cpp \
        view.cpp
