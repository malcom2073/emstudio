#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T13:25:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emslogview
TEMPLATE = app

win32: {
LIBS += -LC:\libs\qjson\lib -lqjson
INCLUDEPATH += C:\libs\qjson\include

} else {
LIBS += -lqjson
}
INCLUDEPATH += ../
SOURCES += main.cpp\
        mainwindow.cpp \
    egraph.cpp \
    qgraph.cpp \
    dataselectionscreen.cpp \
    mapview.cpp \
    emap.cpp

HEADERS  += mainwindow.h \
    egraph.h \
    qgraph.h \
    dataselectionscreen.h \
    mapview.h \
    emap.h \
    ../emslogviewplugin.h

FORMS    += mainwindow.ui \
    dataselectionscreen.ui \
    mapview.ui
