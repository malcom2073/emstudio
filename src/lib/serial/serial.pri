INCLUDEPATH += $$PWD

HEADERS += $$PWD/serial/serial.h $$PWD/serial/v8stdint.h
SOURCES += $$PWD/serial.cc
win32 {
	HEADERS += $$PWD/serial/impl/win.h
	SOURCES += $$PWD/impl/win.cc
	SOURCES += $$PWD/impl/list_ports/list_ports_win.cc
	LIBS += -lAdvapi32 -lSetupapi
} else:mac {
	HEADERS += $$PWD/serial/impl/unix.h
	SOURCES += $$PWD/impl/unix.cc
	SOURCES += $$PWD/impl/list_ports/list_ports_osx.cc
} else:unix {
	HEADERS += $$PWD/serial/impl/unix.h
	SOURCES += $$PWD/impl/unix.cc
	SOURCES += $$PWD/impl/list_ports/list_ports_linux.cc
}
