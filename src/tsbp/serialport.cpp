/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "serialport.h"
#include <QDateTime>
#include <QMutexLocker>
SerialPort::SerialPort(QObject *parent) : QThread(parent)
{
	m_serialLockMutex = new QMutex();
	m_interByteSendDelay=0;
	m_inpacket = false;
	m_inescape = false;
	m_packetErrorCount=0;
}
void SerialPort::setPort(QString portname)
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
int SerialPort::writeBytes(QByteArray buf)
{
	int len = m_port->write((const uint8_t*)buf.data(),buf.size());
	//m_port->waitForBytesWritten(1);
	return len;
}

int SerialPort::writeBytes(unsigned char *buf,int len)
{
	Q_UNUSED(buf)
	Q_UNUSED(len)
	int lenret = 0;
	lenret = m_port->write((const uint8_t*)buf,len);
	//m_port->waitForBytesWritten(1);
#ifdef Q_OS_WIN32
//	if (!::WriteFile(m_portHandle, (void*)buf, (DWORD)len, (LPDWORD)&lenret, NULL))
	{

	}
#else
//	lenret = write(m_portHandle,buf,len);
#endif
	return lenret;
}
void SerialPort::readUntilEmpty()
{
/*	while (m_port->waitForReadyRead(5))
	{
		m_port->readAll();
	}*/
}
void SerialPort::run()
{
	try
	{
		m_port = new serial::Serial();
		m_port->setPort(m_portName.toStdString());
		m_port->open();
		if (!m_port->isOpen())
		{
			qDebug() << "Error opening port:";
			emit unableToConnect("Error opening port");
			return;
		}
		m_port->setBaudrate(115200);
		m_port->setParity(serial::parity_none);
		m_port->setStopbits(serial::stopbits_one);
		m_port->setBytesize(serial::eightbits);
		m_port->setFlowcontrol(serial::flowcontrol_none);
		m_port->setTimeout(1,1,0,100,0); //1ms read timeout, 100ms write timeout.
	}
	catch (serial::IOException ex)
	{
		emit unableToConnect(ex.what());
		return;
	}
	catch (std::exception ex)
	{
		emit unableToConnect(ex.what());
		return;
	}
	uint8_t buffer[1024];
	bool isopen = true;
	msleep(1000);
	//Clear the buffer.
	for (int i=0;i<5;i++)
	{
		m_port->read(buffer,1024);
		//msleep(1000);
	}
	qDebug() << "Connected";
	emit connected();
	while(isopen)
	{
		try
		{
			m_serialLockMutex->lock();
			isopen = m_port->isOpen();
			if (m_port->available() >= 2)
			{
				int bufread = m_port->read(buffer,2);
				uint16_t packetsize = (buffer[0] << 8) + buffer[1];
				int avail = m_port->available();
				qDebug() << "SERIAL**** Reading:" << packetsize << avail;
				bufread = m_port->read(buffer,m_port->available());
				QByteArray toemit;
				toemit.append(QByteArray((char*)buffer,bufread));
				qint64 msecs = QDateTime::currentMSecsSinceEpoch();
				while (toemit.size() < packetsize+4)
				{
					//Hang here, sleep maybe?
					m_serialLockMutex->unlock();
					QThread::msleep(1);
					m_serialLockMutex->lock();
					int stopper = 1;
					avail = m_port->available();
					bufread = m_port->read(buffer,avail);
					toemit.append(QByteArray((char*)buffer,bufread));
					qDebug() << "SERIAL**** Reading:" << packetsize << avail;
					if (QDateTime::currentMSecsSinceEpoch() - msecs > 2000)
					{
						//Two second timeout, clear the buffer and reset.
						while (m_port->available())
						{
							m_port->read(buffer,1024);
						}
						toemit.clear();
						break;
					}
				}
				//int size = m_port->read(buffer,packetsize+4);
				//QByteArray toemit((char*)buffer,size);
				emit bytesReady(toemit);
				//m_cycleBuffer.append((char*)buffer,size);
				//if (m_cycleBuffer.size() >= m_requestedBytes)
				//{
				//	int bytestoreturn = ((m_requestedBytes == 0) ? m_cycleBuffer.size() : m_requestedBytes);
				//	QByteArray toemit = m_cycleBuffer.mid(0,bytestoreturn);
				//	m_cycleBuffer.remove(0,bytestoreturn);
				//	emit bytesReady(toemit);
				//}
				//emit bytesReady(QByteArray((const char*)buffer,size));
				m_serialLockMutex->unlock();
			}
			else
			{
				m_serialLockMutex->unlock();
				msleep(1);
			}

		}
		catch (serial::IOException ex)
		{
			//Error here
			m_port->close();
			delete m_port;
			emit error(ex.what());
			emit disconnected();
			return;
		}

	}
}


void SerialPort::requestPacket(QByteArray request,int responselength)
{
	QMutexLocker lock(m_serialLockMutex);
	m_requestedBytes = responselength;
	m_port->write((const uint8_t*)request.data(),request.length());
}

int SerialPort::writePacket(QByteArray packet)
{
	QMutexLocker locker(m_serialLockMutex);
	int written = m_port->write((const uint8_t*)packet.data(),packet.length());
	emit dataWritten(packet);
	return written;

	return 0;
}


void SerialPort::flush()
{

}

int SerialPort::openPort(QString portName,int baudrate,bool oddparity)
{
	m_portName = portName;
	start();
	return 0;
	m_portName = portName;
	m_port = new serial::Serial();
	m_port->setPort(m_portName.toStdString());
	m_port->open();
	if (!m_port->isOpen())
	{
		qDebug() << "Error opening port:";
		//emit unableToConnect("Error opening port");
		return 1;
	}
	m_port->setBaudrate(baudrate);
	if (oddparity)
	{
		m_port->setParity(serial::parity_odd);
	}
	else
	{
		m_port->setParity(serial::parity_none);

	}
	m_port->setStopbits(serial::stopbits_one);
	m_port->setBytesize(serial::eightbits);
	m_port->setFlowcontrol(serial::flowcontrol_none);
	m_port->setTimeout(1,1,0,100,0); //1ms read timeout, 100ms write timeout.
	return 0;

	/*
#ifdef Q_OS_WIN32
	m_portHandle=CreateFileA(portName.toAscii(), GENERIC_READ|GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_portHandle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	COMMCONFIG Win_CommConfig;

	unsigned long confSize = sizeof(COMMCONFIG);
	Win_CommConfig.dwSize = confSize;
	GetCommConfig(m_portHandle, &Win_CommConfig, &confSize);
	if (oddparity)
	{
		QLOG_DEBUG() << "SerialPort Odd parity selected";
		Win_CommConfig.dcb.Parity = 1; //Odd parity
	}
	else
	{
		QLOG_DEBUG() << "SerialPort No parity selected";
		Win_CommConfig.dcb.Parity = 0; //No parity
	}
	Win_CommConfig.dcb.fRtsControl = RTS_CONTROL_DISABLE;
	Win_CommConfig.dcb.fOutxCtsFlow = FALSE;
	Win_CommConfig.dcb.fOutxDsrFlow = FALSE;
	Win_CommConfig.dcb.fDtrControl = DTR_CONTROL_DISABLE;
	Win_CommConfig.dcb.fDsrSensitivity = FALSE;
	Win_CommConfig.dcb.fNull=FALSE;
	Win_CommConfig.dcb.fTXContinueOnXoff = FALSE;
	Win_CommConfig.dcb.fInX=FALSE;
	Win_CommConfig.dcb.fOutX=FALSE;
	Win_CommConfig.dcb.fBinary=TRUE;
	Win_CommConfig.dcb.DCBlength = sizeof(DCB);
	if (baudrate != -1)
	{
		Win_CommConfig.dcb.BaudRate = baudrate;
	}
	else
	{
		Win_CommConfig.dcb.BaudRate = 115200;
	}
	Win_CommConfig.dcb.ByteSize = 8;
	COMMTIMEOUTS Win_CommTimeouts;
	Win_CommTimeouts.ReadIntervalTimeout = MAXDWORD; //inter-byte timeout value (Disabled)
	Win_CommTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD; //Multiplier
	Win_CommTimeouts.ReadTotalTimeoutConstant = 1; //Total timeout, 1/10th of a second to match *nix
	Win_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	Win_CommTimeouts.WriteTotalTimeoutConstant = 0;
	SetCommConfig(m_portHandle, &Win_CommConfig, sizeof(COMMCONFIG));
	SetCommTimeouts(m_portHandle,&Win_CommTimeouts);
	SetCommMask(m_portHandle,EV_RXCHAR);
	return 0;
#else

	m_portHandle = open(portName.toAscii(),O_RDWR | O_NOCTTY | O_NDELAY); //Should open the port non blocking
	if (m_portHandle < 0)
	{
		//printf("Error opening Com: %s\n",portName);
		//debug(obdLib::DEBUG_ERROR,"Error opening com port %s",portName);
		perror("Error opening COM port Low Level");
		QLOG_DEBUG() << "Port:" << portName;
		return -1;
	}
	if (flock(m_portHandle,LOCK_EX | LOCK_NB))
	{
		QLOG_DEBUG() << "Unable to maintain lock on serial port" << portName;
		QLOG_DEBUG() << "This port is likely open in another process";
		return -2;
	}
	//printf("Com Port Opened %i\n",portHandle);
	//debug(obdLib::DEBUG_VERBOSE,"Com Port Opened %i",portHandle);
	//fcntl(m_portHandle, F_SETFL, FASYNC); //Set it to blocking. This is required? Wtf?
	//struct termios oldtio;
	struct termios newtio;
	//bzero(&newtio,sizeof(newtio));
	tcgetattr(m_portHandle,&newtio);
	long BAUD = B115200;
	switch (baudrate)
	{
		case 38400:
			BAUD = B38400;
			break;
		case 115200:
			BAUD  = B115200;
			break;
		case 19200:
			BAUD  = B19200;
			break;
		case 9600:
			BAUD  = B9600;
			break;
		case 4800:
			BAUD  = B4800;
			break;
		default:
			BAUD = B115200;
			break;
	}  //end of switch baud_rate
	if (strspn("/dev/pts",portName.toAscii()) >= 8)
	{
		//debug(obdLib::DEBUG_WARN,"PTS Detected... disabling baud rate selection on: %s",portName);
		//printf("PTS detected... disabling baud rate selection: %s\n",portName);
		baudrate = -1;
	}
	else
	{
	}
	newtio.c_cflag |=  CS8;
	newtio.c_cflag |= CLOCAL | CREAD;
	if (oddparity)
	{
		newtio.c_cflag |= (PARENB | PARODD);
	}
	else
	{
		newtio.c_cflag &= ~(PARENB | CSTOPB  | CSIZE | PARODD);
	}
	newtio.c_cflag &= ~CRTSCTS;
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_iflag=IGNBRK;
	newtio.c_iflag &= ~(IXON|IXOFF|IXANY);
	newtio.c_lflag=0;
	newtio.c_oflag=0;
	newtio.c_cc[VTIME]=10; //1/10th second timeout, to allow for quitting the read thread
	newtio.c_cc[VMIN]=0; //We want a pure timer timeout
	if (baudrate != -1)
	{
		if(cfsetispeed(&newtio, BAUD))
		{
			//perror("cfsetispeed");
		}

		if(cfsetospeed(&newtio, BAUD))
		{
			//perror("cfsetospeed");
		}
		//debug(obdLib::DEBUG_VERBOSE,"Setting baud rate to %i on port %s\n",baudrate,portName);
	}
	fcntl(m_portHandle, F_SETFL, 0); //Set to blocking
	tcsetattr(m_portHandle,TCSANOW,&newtio);
	tcflush(m_portHandle, TCIOFLUSH); // clear buffer
	return 0;
#endif //Q_OS_WIN32*/
}
