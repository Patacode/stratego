CONFIG += c++17
CONFIG -= debug_and_release debug_and_release_target

# or sharedlib
LIB_MODE = staticlib

!equals(TARGET, core){
    include($${PWD}/src/core/core.pri)
}

win32-g++*{
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
}

QMAKE_CXXFLAGS += -Wall -Wextra

lessThan(QT_MAJOR_VERSION, 5){
    error("Qt version 5 or higher is required");
}

lessThan(QT_MINOR_VERSION, 12){
    QMAKE_CXXFLAGS += -std=c++17
}
