/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QMutex>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>

class SerialPort : public QObject {
    Q_OBJECT

public:
    SerialPort(QObject* parent = 0);
    ~SerialPort();

    int serialBufferSize() { return m_readBuffer.size(); }
    void setInterByteSendDelay(int milliseconds);
    static QStringList ports();

public slots:
    int openPort(const QString& portName, int baudrate, bool oddparity = true);
    int writeBytes(const QByteArray& buf);
    void readUntilEmpty();
    int writePacket(const QByteArray& packet);
    void flush();
    void closePort();
    void requestPacket(const QByteArray& request, int responselength);

private slots:
    void onReadyRead();
    void onTimeout();
    void onError(QSerialPort::SerialPortError error_msg);

private:
    void run();
    void openLogs();

    QSerialPort* m_port;
    QMutex* m_serialLockMutex;
    QString m_portName;
    QByteArray m_readBuffer;
    QTimer m_readTimeoutTimer;
    int m_requestedBytes;
    int m_interByteSendDelay;
    int m_baud;
    quint16 m_packetsize;

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
