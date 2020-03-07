/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "serialport.h"
#include <QMutexLocker>
#include <QElapsedTimer>

SerialPort::SerialPort(QObject *parent) : QThread(parent),
    m_port(new QSerialPort), m_serialLockMutex(new QMutex)
{
    m_interByteSendDelay = 0;
}

SerialPort::~SerialPort()
{
    m_port->close();
    m_port->deleteLater();
}

void SerialPort::setPort(const QString &portname)
{
    m_portName = portname;
}

void SerialPort::setBaud(int baudrate)
{
    m_baud = baudrate;
}

void SerialPort::setInterByteSendDelay(int milliseconds)
{
    m_interByteSendDelay = milliseconds;
}

int SerialPort::writeBytes(const QByteArray &buf)
{
    int len = m_port->write(buf);
    m_port->waitForBytesWritten(1);
    return len;
}

void SerialPort::readUntilEmpty()
{
    m_port->clear(QSerialPort::Input);
}

void SerialPort::run()
{
    m_port->close();
    m_port->setPortName(m_portName);
    m_port->open(QIODevice::ReadWrite);

    if (!m_port->isOpen())
    {
        qWarning() << "Error opening port:";
        emit unableToConnect("Error opening port");
        return;
    }

    m_port->setBaudRate(115200);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setFlowControl(QSerialPort::NoFlowControl);

    QByteArray buffer;
    m_port->clear(QSerialPort::AllDirections);

    qDebug() << "Connected";
    emit connected();

    while(m_port->isOpen() && !this->isInterruptionRequested())
    {
        if (m_port->bytesAvailable() >= 2)
        {
            m_serialLockMutex->lock();
            buffer = m_port->read(2);
            uint16_t packetsize = (buffer[0] << 8) + buffer[1];

            QByteArray toemit;
            QElapsedTimer timer;
            timer.start();
            while (toemit.size() < packetsize + 4)
            {
                //Hang here, sleep maybe?
                if (m_port->bytesAvailable() == 0)
                    m_port->waitForReadyRead(10);
                toemit.append(m_port->readAll());

                if (timer.elapsed() > 2000)
                {
                    //Two second timeout, clear the buffer and reset.
                    while (m_port->bytesAvailable())
                    {
                        m_port->clear(QSerialPort::Input);
                    }
                    toemit.clear();
                    break;
                }
            }
            emit bytesReady(toemit);
            m_serialLockMutex->unlock();
        }
        else
        {
            m_port->waitForReadyRead(10);
        }
    }
}

void SerialPort::requestPacket(const QByteArray &request, int responselength)
{
    QMutexLocker lock(m_serialLockMutex);
    m_requestedBytes = responselength;
    writeBytes(request);
}

QStringList SerialPort::ports()
{
    QStringList list;
    auto ports = QSerialPortInfo::availablePorts();

    for (int i = 0; i < ports.size(); i++)
    {
        list.append(ports.at(i).portName());
    }

    return list;
}

int SerialPort::writePacket(const QByteArray &packet)
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

int SerialPort::openPort(const QString &portName, int baudrate, bool oddparity)
{
    Q_UNUSED(oddparity);
    m_portName = portName;
    m_baud = baudrate;
    start();

    return 0;
}
