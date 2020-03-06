DISTFILES += \
	$$PWD/msplugin.json

SUBDIRS += \
	$$PWD/tsbp.pro

HEADERS += \
	$$PWD/datafield.h \
	$$PWD/serialport.h \
	$$PWD/tsbpcomms.h \
	$$PWD/tsbpconfigdata.h \
	$$PWD/tsbpdatapacketdecoder.h \
	$$PWD/tsbpheaders.h \
	$$PWD/tsbpmemorymetadata.h \
	$$PWD/tsbptable2ddata.h \
	$$PWD/tsbptable3ddata.h

SOURCES += \
	$$PWD/datafield.cpp \
	$$PWD/serialport.cpp \
	$$PWD/tsbpcomms.cpp \
	$$PWD/tsbpconfigdata.cpp \
	$$PWD/tsbpdatapacketdecoder.cpp \
	$$PWD/tsbpmemorymetadata.cpp \
	$$PWD/tsbptable2ddata.cpp \
	$$PWD/tsbptable3ddata.cpp
