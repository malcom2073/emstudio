/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QMutex>
#include <qglobal.h>
#include "serial/serial.h"
//#include "qserialport.h"
//#ifdef Q_OS_WIN32
//#include <windows.h>
//#else
//#define HANDLE int
//#include <sys/file.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <termios.h>
//#include <unistd.h>
//#endif
#include <vector>
class SerialPort : public QThread
{
	Q_OBJECT
public:
    SerialPort(QObject *parent=0);
    void setPort(QString portname);
    void setBaud(int baudrate);
    int openPort(QString portName,int baudrate,bool oddparity = true);
    int writeBytes(QByteArray buf);
    void readUntilEmpty();
    void flush();
    //HANDLE portHandle() { return m_portHandle; }
    void closePort();
    int writePacket(QByteArray packet);
    int writeBytes(unsigned char *buf,int len);
    //int readBytes(unsigned char *buf,int maxlen);
    //void sendMessageForResponse(QByteArray header,QByteArray payload);
    int bufferSize() { return m_queuedMessages.size(); }
    int serialBufferSize() { return m_cycleBuffer.size(); }
    void setInterByteSendDelay(int milliseconds);
    void requestPacket(QByteArray request,int responselength);
private:
    int m_requestedBytes;
    void run();
    serial::Serial *m_port;
    QMutex *m_serialLockMutex;
    void openLogs();
    unsigned int m_packetErrorCount;
    bool m_logsEnabled;
    QString m_logDirectory;
    bool m_inpacket;
    bool m_inescape;
    int m_interByteSendDelay;
    QList<QByteArray> m_queuedMessages;
    //QByteArray m_buffer;
    //QFile *m_logInFile;
    //QFile *m_logOutFile;
    //QFile *m_logInOutFile;
    QString m_logFileName;
    QString m_portName;
    int m_baud;
    //HANDLE m_portHandle;
    QByteArray m_cycleBuffer;

signals:
    void packetReceived(QByteArray packet);
    void parseBuffer(QByteArray buffer);
    void dataWritten(QByteArray data);
    void bytesReady(QByteArray buffer);
    void connected();
    void unableToConnect(QString error);
    void disconnected();
    void error(QString error);
};

#endif // SERIALPORT_H
