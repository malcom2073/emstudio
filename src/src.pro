#-------------------------------------------------
#
# Project created by QtCreator 2012-05-03T11:03:53
#
#-------------------------------------------------

QT = core gui opengl printsupport widgets quick qml serialport

TARGET = ../../build/bin/emstudio
TEMPLATE = app
INCLUDEPATH += src
OBJECTS_DIR = ../build/tune_obj
MOC_DIR = ../build/tune_obj
UI_DIR = ../build/tune_obj

INCLUDEPATH += $$PWD/../lib/core
DEPENDPATH += $$PWD/../lib/core

include(lib/QsLog/QsLog.pri)
include(lib/QHexEdit/QHexEdit.pri)
win32 {
    message("Building for win32")
    #DEFINES += GIT_COMMIT=$$system("c:/progra~1/git/bin/git.exe" describe --dirty=-DEV --always)
    #DEFINES += GIT_HASH=$$system("c:/progra~1/git/bin/git.exe" log -n 1 --pretty=format:%H)
    DEFINES += GIT_COMMIT=TESTCOMMIT
    DEFINES += GIT_HASH=TESTHASH
    LIBS += opengl32.lib glu32.lib
} else:mac {
    QMAKE_CXXFLAGS += -Werror
    INCLUDEPATH += /opt/local/include
    LIBS += -L/opt/local/lib
    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
    DEFINES += GIT_DATE=\""$$system(date)"\"
} else:unix {
    message("Building for *nix")
    isEmpty($$PREFIX) {
        PREFIX = /usr/local
        message("EMSTune using default install prefix " $$PREFIX)
    } else {
        message("EMSTune using custom install prefix " $$PREFIX)
    }
    DEFINES += INSTALL_PREFIX=$$PREFIX
    target.path = $$PREFIX/bin
    dashboard.path = $$PREFIX/share/emstudio/dashboards
    dashboard.files += src/qml/gauges.qml
    dashboard.files += src/qml/oniongauges.qml
    warninglabel.path = $$PREFIX/share/emstudio/dashboards/WarningLabel
    warninglabel.files += src/qml/WarningLabel/WarningLabel.qml
    wizards.path = $$PREFIX/share/emstudio/wizards
    wizards.files += src/qml/wizards/ActualDecoderOffset.qml
    wizards.files += src/qml/wizards/BenchTest.qml
    wizards.files += src/qml/wizards/DecoderOffset.qml
    wizards.files += src/qml/wizards/EngineConfig.qml
    wizards.files += src/qml/wizards/wizard.qml
    config.path = $$PREFIX/share/emstudio/definitions
    config.files += libreems.config.json
    INSTALLS += target config dashboard wizards warninglabel
    LIBS += -lGL -lGLU -lglut
    DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
    DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
    DEFINES += GIT_DATE=\""$$system(date --iso-8601=seconds)"\"
}
include(ui/ui.pri)
include(tsbp/tsbp.pri)

SUBDIRS += plugins
OTHER_FILES += \
    README.md \
    src/qml/wizards/BenchTest.qml \
    src/qml/wizards/DecoderOffset.qml \
    src/qml/wizards/wizard.qml \
    src/qml/wizards/EngineConfig.qml \
    src/qml/wizards/ActualDecoderOffset.qml \
    src/qml/WarningLabel/WarningLabel.qml \
    src/qml/gauges.qml \
    src/qml/oniongauges.qml \
    src/qml/wizards/OutputChannelTest.qml

HEADERS += \
    configblock.h \
    configdata.h \
    datapacketdecoder.h \
    datatype.h \
    dataview.h \
    dialogfield.h \
    dialogitem.h \
    emscomms.h \
    emscore.h \
    formattype.h \
    logloader.h \
    lookupmetadata.h \
    memorylocation.h \
    memorylocationinfo.h \
    memorymetadata.h \
    menuitem.h \
    menusetup.h \
    paths.h \
    rawdata.h \
    readonlyramblock.h \
    readonlyramdata.h \
    serialportstatus.h \
    submenuitem.h \
    table2ddata.h \
    table2dmetadata.h \
    table3ddata.h \
    table3dmetadata.h \
    tabledata.h

SOURCES += \
    dataview.cpp \
    emscore.cpp \
    logloader.cpp \
    main.cpp \
    memorylocation.cpp

