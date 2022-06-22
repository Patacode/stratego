QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../config.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    lineedit.cpp \
    main.cpp \
    qboard.cpp \
    qcell.cpp \
    qcomponent.cpp \
    qconfigwindow.cpp \
    qendgamedialog.cpp \
    qgamewindow.cpp \
    qgraveyard.cpp \
    qinputconfig.cpp \
    qpanel.cpp \
    qpiece.cpp \
    qpiecestats.cpp \
    qpiecestorage.cpp \
    qstartappdialog.cpp \
    qstartgamedialog.cpp \
    view.cpp

HEADERS += \
    lineedit.h \
    qappdialog.h \
    qappwindow.h \
    qboard.h \
    qcell.h \
    qcomponent.h \
    qgraveyard.h \
    qinputconfig.h \
    qpanel.h \
    qpiece.h \
    qpiecestats.h \
    qpiecestorage.h \
    util.h \
    vcstuff.h

# Default rules for deployment.
LIBRARY_OUT_PWD = $$clean_path($$OUT_PWD/$$relative_path($$PWD, $$_PRO_FILE_PWD_))
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = $${LIBRARY_OUT_PWD}/opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
