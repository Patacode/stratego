TEMPLATE = subdirs
SUBDIRS += \
    src/core \
    src/tui \
    src/gui \
    test/unitTests

src-tui.depends = src/core
src-gui.depends = src/core
test-unitTests.depends = src/core

OTHER_FILES += config.pri

LIBRARY_OUT_PWD = $$clean_path($$OUT_PWD/$$relative_path($$PWD, $$_PRO_FILE_PWD_))
resources.path = $${LIBRARY_OUT_PWD}/resources
resources.files = $${PWD}/resources/*

INSTALLS += resources
