#include "eblplugin.h"
#include <QFile>
#include <QDebug>

EBLPlugin::EBLPlugin(QObject *parent) : EmsLogViewPlugin(parent)
{
}
void EBLPlugin::run()
{
	//EBL log
	QFile logfile(m_fileName);
	logfile.open(QIODevice::ReadOnly);
	QByteArray buf;
	QByteArray packet;
	bool firstbyte = false;
	bool secondbyte = false;
	int packetcounter = 0;
	int logsignalcount=0;
	while (!logfile.atEnd())
	{
		buf = logfile.read(1024);
		logsignalcount++;
		if (logsignalcount >= 5)
		{
			emit loadProgress(logfile.pos(),logfile.size());
		}
		for (int i=0;i<buf.size();i++)
		{
			if (((unsigned char)buf[i]) == 0x55 && !firstbyte)
			{
				firstbyte = true;
				//qDebug() << "first byte!";
				packet.append(buf[i]);
			}
			else if (((unsigned char)buf[i]) == 0xAA && !secondbyte && firstbyte)
			{
				secondbyte = true;
				packet.append(buf[i]);
				//qDebug() << "second byte";
			}
			else if (firstbyte && secondbyte)
			{
				//We're in a packet
				packetcounter++;
				if (packetcounter >= 275)
				{
					packet.append(buf[i]);
					//End of packet;
					firstbyte = false;
					secondbyte = false;
					packetcounter = 0;
					//qDebug() << "Packet len:" << packet.length();
					parseEblPacket(packet);
					packet.clear();
				}
				else
				{
					packet.append(buf[i]);
				}
			}
			else
			{
				qDebug() << "Out of packet" << QString::number(((unsigned char)buf[i]),16).toUpper();
			}
		}
	}
	emit done();
}
void EBLPlugin::parseEblPacket(QByteArray origpacket)
{
	//qDebug() << "Packet:" << datalogpacketcount;
	QByteArray packet = origpacket.mid(2); //Strip off the header
	QVariantMap propertyMap;
	//propertyMap["DATALOGCOUNT"] = datalogpacketcount;
	//propertyMap["DATALOGSIZE"] = logFile->pos() / 1000;
	propertyMap["LEANCRUISE"] = ((packet[0x3] & 0x4) ? true : false);
	propertyMap["CLOSEDTHROTTLE"] = ((packet[0x3] & 0x8) ? true : false);

	propertyMap["FAN"] = ((packet[0x5] & 0x1) ? true : false);
	propertyMap["AC"] = ((packet[0x5] & 0x8) ? true : false);
	propertyMap["TCC"] = ((packet[0x5] & 0x80) ? true : false);

	propertyMap["CCP"] = ((packet[0x6] & 0x1) ? true : false);
	propertyMap["Launch"] = ((packet[0x6] & 0x4) ? true : false);
	propertyMap["N2O"] = ((packet[0x6] & 0x8) ? true : false);
	propertyMap["Fuel Cut"] = ((packet[0x6] & 0x80) ? false : true);

	propertyMap["DFCO"] = ((packet[0x8] & 0x8) ? true : false);

	propertyMap["SPARKADVANCE"] = (unsigned char)packet[0xA];
	propertyMap["BLM"] = (unsigned char)packet[0xA3];
	propertyMap["INT"] = (unsigned char)packet[0xA4];

	propertyMap["SES"] = ((packet[0xB] & 0x1) ? true : false);

	propertyMap["DE"] = ((packet[0xD] & 0x10) ? true : false);
	propertyMap["AE"] = ((packet[0xD] & 0x40) ? true : false);
	propertyMap["PE"] = ((packet[0xD] & 0x80) ? true : false);

	propertyMap["LEARN"] = ((packet[0xE] & 0x1) ? true : false);
	propertyMap["ASYNC"] = ((packet[0xE] & 0x10) ? true : false);

	propertyMap["RPM"] = ((unsigned char)packet[0x1C]) * 25;

	propertyMap["CLOSEDLOOP"] = ((packet[0xE] & 0x80) ? true : false);

	propertyMap["ACREQ"] = ((packet[0xF] & 0x8) ? true : false);

	propertyMap["PROMID"] = (((unsigned char)packet[0x10]) << 8) + ((unsigned char)packet[0x11]);

	propertyMap["MAP"] = ((unsigned char)packet[0x26]) * (255.0/100.0);
	propertyMap["IAT"] = (unsigned char)packet[0x33];
	propertyMap["MPH"] = (unsigned char)packet[0x34];

	propertyMap["S/F"] = ((packet[0x36] & 0x10) ? true : false);

	propertyMap["O2"] = ((unsigned char)packet[0x3E]) * (255.0/1128.0);

	propertyMap["IGNVOLTS"] = ((unsigned char)packet[0x45]) * (0.1);
	propertyMap["PMPVOLTS"] = ((unsigned char)packet[0x46]) * (255.0/10.0);

	propertyMap["TPS"] = ((unsigned char)packet[0x49]) * (255.0/100.0);

	propertyMap["CHT"] = (unsigned char)packet[0xE3];
	emit payloadDecoded(propertyMap);
}
void EBLPlugin::loadLog(QString log)
{
	m_fileName = log;
	start();
}
Q_EXPORT_PLUGIN2(EBLLogViewer, EBLPlugin)
