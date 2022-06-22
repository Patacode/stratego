CONFIG -= qt

TEMPLATE = lib

include(../../config.pri)

CONFIG += $${LIB_MODE}

HEADERS += \
    config.h \
    designpatt.h \
    eventMgr.h \
    gamestuff.h \
    piece.h \
    model.h \
    pieceFactory.h \
    properties.h \
    util.h

SOURCES += \
        board.cpp \
        config.cpp \
        game_struct.cpp \
        history.cpp \
        model.cpp \
        parser.cpp \
        piece.cpp \
        eventMgr.cpp \
        pieceFactory.cpp \
        player.cpp \
        properties.cpp

DISTFILES += \
    core.pri
