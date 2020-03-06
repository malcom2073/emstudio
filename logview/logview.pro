#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T13:25:25
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ../../build/bin/logview
TEMPLATE = app
OBJECTS_DIR = ../build/logview_obj
MOC_DIR = ../build/logview_obj
UI_DIR = ../build/logview_obj

win32 {
#LIBS += -LC:\libs\qjson\lib -lqjson
#INCLUDEPATH += C:\libs\qjson\include
} else {
#LIBS += -lqjson
}
win32 { #Windows based mingw build
        message("Building for win32")
        DEFINES += GIT_COMMIT=$$system(\"c:/program files (x86)/git/bin/git.exe\" describe --dirty=-DEV --always)
        DEFINES += GIT_HASH=$$system(\"c:/program files (x86)/git/bin/git.exe\" log -n 1 --pretty=format:%H)
        QMAKE_LFLAGS += -static-libgcc -static-libstdc++
} else:mac {
        message("Building for OSX")
        INCLUDEPATH += /opt/local/include
        LIBS += -L/opt/local/lib
        DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
        DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
        DEFINES += GIT_DATE=\""$$system(date)"\"
} else:unix {
        message("Building for *nix")
        isEmpty($$PREFIX) {
                PREFIX = /usr/local
                message("EMSLogView using default install prefix " $$PREFIX)
        } else {
                message("EMSLogView using custom install prefix " $$PREFIX)
        }
        DEFINES += INSTALL_PREFIX=$$PREFIX
        target.path = $$PREFIX/bin
        INSTALLS += target
        DEFINES += GIT_COMMIT=$$system(git describe --dirty=-DEV --always)
        DEFINES += GIT_HASH=$$system(git log -n 1 --pretty=format:%H)
        DEFINES += GIT_DATE=\""$$system(date)"\"
}
INCLUDEPATH += ../
SOURCES += main.cpp\
        mainwindow.cpp \
    egraph.cpp \
    qgraph.cpp \
    dataselectionscreen.cpp \
    mapview.cpp \
    emap.cpp \
    femsdatafield.cpp \
    femsparser.cpp \
    msparser.cpp \
    graphview.cpp \
    eblparser.cpp \
    aboutdialog.cpp \
    exporter_csv.cpp \
    scatterplotview.cpp

HEADERS  += mainwindow.h \
    egraph.h \
    qgraph.h \
    dataselectionscreen.h \
    mapview.h \
    emap.h \
    ../emslogviewplugin.h \
    femsdatafield.h \
    femsparser.h \
    msparser.h \
    graphview.h \
    eblparser.h \
    aboutdialog.h \
    exporter_csv.h \
    scatterplotview.h

FORMS    += mainwindow.ui \
    dataselectionscreen.ui \
    mapview.ui \
    graphview.ui \
    aboutdialog.ui \
    scatterplotview.ui
