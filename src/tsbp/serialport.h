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
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPort : public QThread
{
    Q_OBJECT

public:
    SerialPort(QObject *parent=0);
    ~SerialPort();

    void setPort(const QString &portname);
    void setBaud(int baudrate);
    int openPort(const QString &portName, int baudrate, bool oddparity = true);
    int writeBytes(const QByteArray &buf);
    void readUntilEmpty();
    void flush();
    void closePort();
    int writePacket(const QByteArray &packet);
    int bufferSize() { return m_queuedMessages.size(); }
    int serialBufferSize() { return m_cycleBuffer.size(); }
    void setInterByteSendDelay(int milliseconds);
    void requestPacket(const QByteArray &request, int responselength);
    static QStringList ports();

private:
    void run();
    void openLogs();

    int m_requestedBytes;
    QSerialPort *m_port;
    QMutex *m_serialLockMutex;
    bool m_logsEnabled;
    QString m_logDirectory;
    int m_interByteSendDelay;
    QList<QByteArray> m_queuedMessages;
    QString m_logFileName;
    QString m_portName;
    int m_baud;
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
