QT       += core gui
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/comboparam.cpp \
    src/datafield.cpp \
    src/emstest.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/memorylocation.cpp \
    src/memorylocationinfo.cpp \
    src/mspcomms.cpp \
    src/parameterview.cpp \
    src/parameterwidget.cpp \
    src/scalarparam.cpp \
    src/tsbpconfigdata.cpp \
    src/tsbpdatapacketdecoder.cpp \
    src/tsbpmemorymetadata.cpp \
    src/tsbptable2ddata.cpp \
    src/tsbptable3ddata.cpp

HEADERS += \
    src/comboparam.h \
    src/configblock.h \
    src/configdata.h \
    src/datafield.h \
    src/datatype.h \
    src/dialogfield.h \
    src/dialogitem.h \
    src/emstest.h \
    src/formattype.h \
    src/lookupmetadata.h \
    src/mainwindow.h \
    src/memorylocation.h \
    src/memorylocationinfo.h \
    src/menuitem.h \
    src/menusetup.h \
    src/mspcomms.h \
    src/parameterview.h \
    src/parameterwidget.h \
    src/rawdata.h \
    src/readonlyramblock.h \
    src/readonlyramdata.h \
    src/scalarparam.h \
    src/submenuitem.h \
    src/table2dmetadata.h \
    src/table3dmetadata.h \
    src/tabledata.h \
    src/tsbpconfigdata.h \
    src/tsbpdatapacketdecoder.h \
    src/tsbpheaders.h \
    src/tsbpmemorymetadata.h \
    src/tsbptable2ddata.h \
    src/tsbptable3ddata.h

FORMS += \
    src/ui/comboparam.ui \
    src/ui/mainwindow.ui \
    src/ui/parameterview.ui \
    src/ui/parameterwidget.ui \
    src/ui/scalarparam.ui
OTHER_FILES += LICENSE.md README.md
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
