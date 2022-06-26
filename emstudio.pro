QT       += core gui
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comboparam.cpp \
    datafield.cpp \
    emstest.cpp \
    main.cpp \
    mainwindow.cpp \
    memorylocation.cpp \
    memorylocationinfo.cpp \
    mspcomms.cpp \
    parameterview.cpp \
    parameterwidget.cpp \
    scalarparam.cpp \
    tsbpconfigdata.cpp \
    tsbpdatapacketdecoder.cpp \
    tsbpmemorymetadata.cpp \
    tsbptable2ddata.cpp \
    tsbptable3ddata.cpp

HEADERS += \
    comboparam.h \
    configblock.h \
    configdata.h \
    datafield.h \
    datatype.h \
    dialogfield.h \
    dialogitem.h \
    emstest.h \
    formattype.h \
    lookupmetadata.h \
    mainwindow.h \
    memorylocation.h \
    memorylocationinfo.h \
    menuitem.h \
    menusetup.h \
    mspcomms.h \
    parameterview.h \
    parameterwidget.h \
    rawdata.h \
    readonlyramblock.h \
    readonlyramdata.h \
    scalarparam.h \
    submenuitem.h \
    table2dmetadata.h \
    table3dmetadata.h \
    tabledata.h \
    tsbpconfigdata.h \
    tsbpdatapacketdecoder.h \
    tsbpheaders.h \
    tsbpmemorymetadata.h \
    tsbptable2ddata.h \
    tsbptable3ddata.h

FORMS += \
    comboparam.ui \
    mainwindow.ui \
    parameterview.ui \
    parameterwidget.ui \
    scalarparam.ui
OTHER_FILES += LICENSE.md README.md
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
