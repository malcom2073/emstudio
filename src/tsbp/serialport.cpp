/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "serialport.h"
#include <QMutexLocker>

SerialPort::SerialPort(QObject* parent)
    : QObject(parent), m_port(new QSerialPort), m_serialLockMutex(new QMutex)
{
    m_interByteSendDelay = 0;
    m_packetsize = 0;
    m_readTimeoutTimer.setSingleShot(true);

    connect(m_port, &QSerialPort::readyRead, this, &SerialPort::onReadyRead);
    connect(m_port, &QSerialPort::errorOccurred, this, &SerialPort::onError);
    connect(&m_readTimeoutTimer, &QTimer::timeout, this, &SerialPort::onTimeout);
}

SerialPort::~SerialPort()
{
    closePort();
    m_port->deleteLater();
}

void SerialPort::setInterByteSendDelay(int milliseconds)
{
    m_interByteSendDelay = milliseconds;
}

int SerialPort::writeBytes(const QByteArray& buf)
{
    int len = m_port->write(buf);
    m_port->waitForBytesWritten(1);
    return len;
}

void SerialPort::readUntilEmpty()
{
    m_port->clear(QSerialPort::Input);
}

void SerialPort::requestPacket(const QByteArray& request, int responselength)
{
    QMutexLocker lock(m_serialLockMutex);
    m_requestedBytes = responselength;
    writeBytes(request);
}

QStringList SerialPort::ports()
{
    QStringList list;
    auto ports = QSerialPortInfo::availablePorts();

    for (int i = 0; i < ports.size(); i++) {
        list.append(ports.at(i).portName());
    }

    return list;
}

void SerialPort::onReadyRead()
{
    QMutexLocker locker(m_serialLockMutex);
    quint32 bytesAvailable = (quint32)m_port->bytesAvailable();

    if (m_packetsize == 0 && bytesAvailable >= 2) {
        QByteArray buffer(m_port->read(2));
        m_packetsize = (buffer[0] << 8) + buffer[1] + 4;
        m_readTimeoutTimer.start(2000);
        m_readBuffer.clear();
    }

    bytesAvailable = (quint32)m_port->bytesAvailable();
    if (m_packetsize != 0 && bytesAvailable >= m_packetsize) {
        m_readBuffer.append(m_port->read(m_packetsize));
        m_packetsize = 0;
        m_readTimeoutTimer.stop();
        emit bytesReady(m_readBuffer);
    }
}

void SerialPort::onTimeout()
{
    qDebug() << "Timeout";
    m_packetsize = 0;
    flush();
}

void SerialPort::onError(QSerialPort::SerialPortError error_msg)
{
    Q_UNUSED(error_msg);
    emit error(m_port->errorString());
}


int SerialPort::writePacket(const QByteArray& packet)
{
    QMutexLocker locker(m_serialLockMutex);
    int written = writeBytes(packet);

    emit dataWritten(packet);
    return written;
}

void SerialPort::flush()
{
    m_port->flush();
}

void SerialPort::closePort()
{
    m_port->close();
}

int SerialPort::openPort(const QString& portName, int baudrate, bool oddparity)
{
    Q_UNUSED(oddparity);
    m_portName = portName;
    m_baud = baudrate;

    m_port->close();
    m_port->setPortName(m_portName);
    m_port->open(QIODevice::ReadWrite);

    if (!m_port->isOpen()) {
        qWarning() << "Error opening port:";
        emit unableToConnect("Error opening port");
        return -1;
    }

    m_port->setBaudRate(m_baud);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setFlowControl(QSerialPort::NoFlowControl);

    QByteArray buffer;
    m_port->clear(QSerialPort::AllDirections);

    qDebug() << "Connected";
    emit connected();

    return 0;
}
