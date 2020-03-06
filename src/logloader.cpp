/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "logloader.h"
#include <QFile>
#include <QDebug>
LogLoader::LogLoader(QObject *parent) : QThread(parent)
{
}
void LogLoader::loadFile(QString filename)
{
	m_filename = filename;
}

void LogLoader::run()
{
	QFile logfile(m_filename);
	logfile.open(QIODevice::ReadOnly);
	//QByteArray arr = logfile.readAll();

	//logfile.close();
	int curr = 0;
	//bool escape = false;
	bool inpacket = false;
	QByteArray currPacket;
	//while (!escape || curr >= arr.size())
	QByteArray retval;
	while (!logfile.atEnd())
	{
		//logfile.read(&retval,1);
		emit logProgress(logfile.pos(),logfile.size());
		retval = logfile.read(1);
		if (retval[0] == (char)0xAA)
		{
			if (inpacket)
			{
				//in the middle of a packet
				currPacket.clear();
			}
			currPacket.append(retval[0]);
			//Start byte
			//qDebug() << "Start byte";
			inpacket = true;
		}
		else if (retval[0] == (char)0xCC)
		{
			//currPacket
			currPacket.append(retval[0]);
			QString output;
			for (int i=0;i<currPacket.size();i++)
			{
				int num = (unsigned char)currPacket[i];
				output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
			}
            qDebug() << "Full packet:";
			//qDebug() << output;

			parseBuffer(currPacket);
			currPacket.clear();
			//qDebug() << "loop";
			msleep(25);
		}
		else if (inpacket)
		{
			if (retval[0] == (char)0xBB)
			{
				//Need to escape the next byte
				retval = logfile.read(1);
				if (retval[0] == (char)0x55)
				{
					currPacket.append((char)0xAA);
				}
				else if (retval[0] == (char)0x44)
				{
					currPacket.append((char)0xBB);
				}
				else if (retval[0] == (char)0x33)
				{
					currPacket.append((char)0xCC);
				}
			}
			else
			{
				currPacket.append(retval[0]);
			}
		}
		curr++;
	}
	emit endOfLog();
	logfile.close();
	return;
	/*for (int i=0;i<arr.size();i++)
	{
		//i++;
		//qDebug() << QString::number(arr[i],16);
		curr = i;

		curr+=3;
		curr += 1;
		i += curr-1;
		//i++;

	}*/
}
void LogLoader::parseBuffer(QByteArray buffer)
{
	if (buffer.size() <= 3)
	{
		qDebug() << "Not long enough to even contain a header!";
		return;
	}

	//Trim off 0xAA and 0xCC from the start and end
	buffer = buffer.mid(1);
	buffer = buffer.mid(0,buffer.length()-1);

	//qDebug() << "Packet:" << QString::number(buffer[1],16) << QString::number(buffer[buffer.length()-2],16);
	QByteArray header;
	//currPacket.clear();
	//Parse the packet here
	int headersize = 3;
	int iloc = 0;
	bool seq = false;
	bool len = false;
	if (buffer[iloc] & 0x4)
	{
		//Has header
		seq = true;
		//qDebug() << "Has seq";
		headersize += 1;
	}
	if (buffer[iloc] & 0x1)
	{
		//Has length
		len = true;
		//qDebug() << "Has length";
		headersize += 2;
	}
	header = buffer.mid(0,headersize);
	iloc++;
	unsigned int payloadid = (unsigned int)buffer[iloc] << 8;

	payloadid += (unsigned char)buffer[iloc+1];
	//qDebug() << QString::number(payloadid,16);
	//qDebug() << QString::number(buffer[iloc-1],16);
	//qDebug() << QString::number(buffer[iloc],16);
	//qDebug() << QString::number(buffer[iloc+1],16);
	//qDebug() << QString::number(buffer[iloc+2],16);
	iloc += 2;
	//qDebug() << QString::number(payloadid,16);
	if (seq)
	{
		//qDebug() << "Sequence number" << QString::number(currPacket[iloc]);
		iloc += 1;
	}
	QByteArray payload;
	if (len)
	{
		//qDebug() << "Length found, buffer size:" << buffer.length() << "iloc:" << QString::number(iloc);
		unsigned int length = buffer[iloc] << 8;
		length += buffer[iloc+1];
		//qDebug() << "Length:" << length;
		iloc += 2;
		//curr += length;
		payload.append(buffer.mid(iloc,length));
	}
	else
	{
		//qDebug() << "Buffer length:" << buffer.length();
		//qDebug() << "Attempted cut:" << buffer.length() - iloc;
		payload.append(buffer.mid(iloc),(buffer.length()-iloc) -1);
	}
	//qDebug() << "Payload";
	QString output;
	for (int i=0;i<payload.size();i++)
	{
		int num = (unsigned char)payload[i];
		output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
	}
	//qDebug() << output;
	output.clear();
	//qDebug() << "Header";
	for (int i=0;i<header.size();i++)
	{
		int num = (unsigned char)header[i];
		output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
	}
	//qDebug() << output;
	//Last byte of currPacket should be out checksum.
	unsigned char sum = 0;
	for (int i=0;i<header.size();i++)
	{
		sum += header[i];
	}
	for (int i=0;i<payload.size();i++)
	{
		sum += payload[i];
	}
	//qDebug() << "Payload sum:" << QString::number(sum);
	//qDebug() << "Checksum sum:" << QString::number((unsigned char)currPacket[currPacket.length()-1]);
	if (sum != (unsigned char)buffer[buffer.length()-1])
	{
		qDebug() << "BAD CHECKSUM!";
		qDebug() << "header size:" << header.size();
		qDebug() << "payload size:" << payload.size();
	}
	else
	{
		//qDebug() << "Got full packet. Header length:" << header.length() << "Payload length:" << payload.length();
		emit payloadReceived(header,payload);
		//payload is our actual data.
		//unsigned int rpm = (payload[26] << 8) + payload[27];

		//qDebug() << "f" << f.getValue(&payload);
		//qDebug() << QString::number(rpm);
		//qDebug() << QString::number(((unsigned short)payload[8] << 8) + (unsigned short)payload[9]);
	}
}
