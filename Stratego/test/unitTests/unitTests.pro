TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console

include(../../config.pri)

INCLUDEPATH += ../../libs/

SOURCES += \
    main.cpp \
    tst_board.cpp \
    tst_eventMgr.cpp \
    tst_fileParser.cpp \
    tst_history.cpp \
    tst_player.cpp \
    tst_model.cpp \
    tst_piece.cpp \
    tst_properties.cpp
