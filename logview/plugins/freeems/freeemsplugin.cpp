#include "freeemsplugin.h"
#include <QFile>
#include <QDebug>

FreeEMSPlugin::FreeEMSPlugin(QObject *parent) : EmsLogViewPlugin(parent)
{
}
void FreeEMSPlugin::run()
{
	decoder = new FEDataPacketDecoder();
	connect(decoder,SIGNAL(payloadDecoded(QVariantMap)),this,SIGNAL(payloadDecoded(QVariantMap)));
	QFile logfile(m_fileName);
	logfile.open(QIODevice::ReadOnly);
	QString byteoutofpacket;
	QByteArray qbuffer;
	bool m_inpacket = false;
	bool m_inescape = false;
	int m_packetErrorCount=0;
	int readlen=0;
	int logsignalcount = 0;
	while (!logfile.atEnd())
	{
		logsignalcount++;
		if (logsignalcount >= 5)
		{
			emit loadProgress(logfile.pos(),logfile.size());
		}

		QByteArray buffer = logfile.read(1024);
		readlen = buffer.size();
		if (readlen < 0)
		{
			//Nothing on the port
			qDebug() << "Timeout";
		}
		if (readlen == 0)
		{
			//This should be an error
			msleep(10); //Need a sleep now, due to select timeout.
			continue;
		}
		for (int i=0;i<readlen;i++)
		{
			if (static_cast<unsigned char>(buffer[i]) == 0xAA)
			{
				if (m_inpacket)
				{
					//Start byte in the middle of a packet
					//Clear out the buffer and start fresh
					m_inescape = false;
					QByteArray bufToEmit;
					bufToEmit.append(0xAA);
					QByteArray nbuffer(qbuffer);
					nbuffer.replace(0xBB,QByteArray().append(0xBB).append(0x44));
					nbuffer.replace(0xAA,QByteArray().append(0xBB).append(0x55));
					nbuffer.replace(0xCC,QByteArray().append(0xBB).append(0x33));
					bufToEmit.append(nbuffer);
					bufToEmit.append(0xCC);
					//emit dataRead(bufToEmit);
					qbuffer.clear();
					qDebug() << "Buffer error";
					m_packetErrorCount++;
					m_falseStarts++;
				}
				//Start of packet
				m_inpacket = true;
			}
			else if (static_cast<unsigned char>(buffer[i]) == 0xCC && m_inpacket)
			{
				//End of packet
				m_inpacket = false;

				//New Location of checksum
				unsigned char sum = 0;
				for (int i=0;i<qbuffer.size()-1;i++)
				{
					sum += qbuffer[i];
				}
				QByteArray bufToEmit;
				bufToEmit.append(0xAA);
				QByteArray nbuffer(qbuffer);
				nbuffer.replace(0xBB,QByteArray().append(0xBB).append(0x44));
				nbuffer.replace(0xAA,QByteArray().append(0xBB).append(0x55));
				nbuffer.replace(0xCC,QByteArray().append(0xBB).append(0x33));
				bufToEmit.append(nbuffer);
				bufToEmit.append(0xCC);
				//emit dataRead(bufToEmit);
				if (sum != (unsigned char)qbuffer[qbuffer.size()-1])
				{
					qDebug() << "BAD CHECKSUM!";
					m_packetErrorCount++;
					m_badChecksums++;
				}
				else
				{
					m_packetErrorCount=0;
					//emit incomingPacket();
					Packet p = parseBuffer(qbuffer.mid(0,qbuffer.length()-1));
					if (p.isValid && p.payloadid == 0x0191)
					{
						//emit incomingDatalogPacket(p.payload);
						decoder->decodePayload(p.payload);
					}
					else if (p.isValid)
					{
						//emit incomingPacket(p);
						if (p.payloadid == 0xF8E0)
						{
							//Location ID Details request
							m_locationIdInfoReq++;
						}
						if (p.payloadid == 0xF8E1)
						{
							//Location ID Details reply
							m_locationIdInfoReply++;
						}
					}
				}
				//return qbuffer;
				QString output;
				for (int i=0;i<qbuffer.size();i++)
				{
					int num = (unsigned char)qbuffer[i];
					output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
				}
				qbuffer.clear();
			}
			else
			{
				if (m_inpacket && !m_inescape)
				{
					if (static_cast<unsigned char>(buffer[i]) == 0xBB)
					{
						//Need to escape the next byte
						m_inescape = true;
					}
					else
					{
						qbuffer.append(buffer[i]);
					}

				}
				else if (m_inpacket && m_inescape)
				{
					if (static_cast<unsigned char>(buffer[i]) == 0x55)
					{
						qbuffer.append((char)0xAA);
					}
					else if (static_cast<unsigned char>(buffer[i]) == 0x44)
					{
						qbuffer.append((char)0xBB);
					}
					else if (static_cast<unsigned char>(buffer[i]) == 0x33)
					{
						qbuffer.append((char)0xCC);
					}
					else
					{
						qDebug() << "Error, escaped character is not valid!:" << QString::number(buffer[i],16);
						m_packetErrorCount++;
						m_badEscapeChar++;
					}
					m_inescape = false;
				}
				else
				{
					byteoutofpacket += QString::number(buffer[i],16) + " ";
					m_outOfPacketBytes++;
				}
			}
		}
	}
	emit done();
	qDebug() << "Bad Checksums:" << m_badChecksums;
	qDebug() << "False Starts:" << m_falseStarts;
	qDebug() << "Out of Packet Bytes:" << m_outOfPacketBytes;
	qDebug() << "Length Mismatch:" << m_lengthMismatch;
	qDebug() << "Header too short:" << m_badLengthHeader;
	qDebug() << "Bad Escape Char:" << m_badEscapeChar;
	qDebug() << "Location ID Requests:" << m_locationIdInfoReq;
	qDebug() << "Location ID Replies:" << m_locationIdInfoReply;
	return;
}
FreeEMSPlugin::Packet FreeEMSPlugin::parseBuffer(QByteArray buffer)
{
	if (buffer.size() <= 2)
	{
		qDebug() << "Not long enough to even contain a header!";
		//emit decoderFailure(buffer);
		return Packet(false);
	}


	Packet retval;
	QByteArray header;
	//Parse the packet here
	int headersize = 3;
	int iloc = 0;
	bool seq = false;
	bool len = false;
	if (buffer[iloc] & 0x100)
	{
		//Has header
		seq = true;
		headersize += 1;
	}
	if (buffer[iloc] & 0x1)
	{
		//Has length
		len = true;
		headersize += 2;
	}
	header = buffer.mid(0,headersize);
	iloc++;
	unsigned int payloadid = (unsigned int)buffer[iloc] << 8;

	payloadid += (unsigned char)buffer[iloc+1];
	retval.payloadid = payloadid;
	iloc += 2;
	if (seq)
	{
		iloc += 1;
		retval.hasseq = true;
	}
	else
	{
		retval.hasseq = false;
	}
	QByteArray payload;
	if (len)
	{
		retval.haslength = true;
		unsigned int length = buffer[iloc] << 8;
		length += (unsigned char)buffer[iloc+1];
		retval.length = length;
		iloc += 2;
		if ((unsigned int)buffer.length() > (unsigned int)(length + iloc))
		{
			qDebug() << "Packet length should be:" << length + iloc << "But it is" << buffer.length();
			//emit decoderFailure(buffer);
			m_lengthMismatch++;
			return Packet(false);
		}
		payload.append(buffer.mid(iloc,length));
	}
	else
	{
		retval.haslength = false;
		payload.append(buffer.mid(iloc),(buffer.length()-iloc));
	}
	QString output;
	for (int i=0;i<payload.size();i++)
	{
		int num = (unsigned char)payload[i];
		output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
	}
	output.clear();
	for (int i=0;i<header.size();i++)
	{
		int num = (unsigned char)header[i];
		output.append(" ").append((num < 0xF) ? "0" : "").append(QString::number(num,16));
	}
	//Last byte of currPacket should be out checksum.
	retval.header = header;
	retval.payload = payload;
	if (header[0] & 0x02)
	{
		retval.isNAK = true;
	}
	else
	{
		retval.isNAK = false;
	}
	if (retval.header.size() >= 3)
	{
		return retval;
	}
	else
	{
		m_badLengthHeader++;
		return Packet(false);
	}
}
void FreeEMSPlugin::loadLog(QString log)
{
	m_fileName = log;
	start();
}
Q_EXPORT_PLUGIN2(FreeEMSLogViewer, FreeEMSPlugin)
