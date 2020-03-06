/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "femsparser.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#define NAK 0x02

FEMSParser::FEMSParser(QObject *parent) : QThread(parent)
{
	isPartialPacketWaiting = false;
	loadDataFieldsFromValues();
}
void FEMSParser::run()
{
	//decoder = new FEDataPacketDecoder();
	//connect(decoder,SIGNAL(payloadDecoded(QVariantMap)),this,SIGNAL(payloadDecoded(QVariantMap)));
	m_badChecksums=0;
	m_falseStarts=0;
	m_outOfPacketBytes=0;
	m_lengthMismatch=0;
	m_badLengthHeader=0;
	m_badEscapeChar=0;
	m_locationIdInfoReq=0;
	m_locationIdInfoReply=0;
	int m_totalPackets = 0;
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
            //msleep(10); //Need a sleep now, due to select timeout.
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
					Packet p = parseBuffer(qbuffer.mid(0,qbuffer.length()));
					if (p.isValid && p.payloadid == 0x0191)
					{
						//emit incomingDatalogPacket(p.payload);
						decodePayload(p.payload);
					}
					else if (p.isValid)
					{
						if (!m_payloadIdCounts.contains(p.payloadid))
						{
							m_payloadIdCounts[p.payloadid] = 0;
						}
						m_payloadIdCounts[p.payloadid]++;
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
						else if (p.payloadid == 0x0301)
						{
							if (!isPartialPacketWaiting)
							{
								isPartialPacketWaiting = true;
								m_partialPacketBuffer.clear();
								m_currentPartialPacketSequence = p.partialSequence-1;
								qDebug() << "Partial Packet";
							}
							if (m_currentPartialPacketSequence != p.partialSequence-1)
							{
								//Current is not directly after!!!
								qDebug() << "Invalid partial packet sequence";
							}
							m_partialPacketBuffer.append(p.payload);
							m_currentPartialPacketSequence = p.partialSequence;
							if (p.isComplete)
							{
								qDebug() << "Partial Packet Complete";
								//Final in sequence
								datalogDescriptor(m_partialPacketBuffer);
								isPartialPacketWaiting=false;
							}
						}
					}
					else
					{
						//Invalid packet
						qDebug() << "Invalid packet" << "0x" + QString::number(p.payloadid,16).toUpper();
					}
					m_totalPackets++;
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
	emit logData("Bad Checksums",QString::number(m_badChecksums));
	emit logData("False Starts:" ,QString::number( m_falseStarts));
	emit logData("Out of Packet Bytes:",QString::number(m_outOfPacketBytes));
	emit logData("Length Mismatch:",QString::number(m_lengthMismatch));
	emit logData("Header too short:",QString::number(m_badLengthHeader));
	emit logData("Bad Escape Char:",QString::number(m_badEscapeChar));
	emit logData("Total valid packets:",QString::number(m_totalPackets));
	emit logData("Location ID Requests:",QString::number(m_locationIdInfoReq));
	emit logData("Location ID Replies:",QString::number(m_locationIdInfoReply));
	for (QMap<unsigned short,int>::const_iterator i = m_payloadIdCounts.constBegin();i!=m_payloadIdCounts.constEnd();i++)
	{
		QString string = getStringFromEnum((RequestType)(i.key()-1));
		if (string == "")
		{
			emit logData("Payload ID 0x" + QString::number(i.key(),16).toUpper(),QString::number(i.value()));
		}
		else
		{
			emit logData("Payload ID " + string,QString::number(i.value()));
		}
	}
	return;
}
FEMSParser::Packet FEMSParser::parseBuffer(QByteArray buffer)
{
	if (buffer.size() <= 2)
	{

		qDebug() << "Not long enough to even contain a header!";
		//emit decoderFailure(buffer);
		return Packet(false);
	}
	buffer = buffer.mid(0,buffer.size()-1);


	Packet retval;
	QByteArray header;
	//Parse the packet here
	int headersize = 3;
	int iloc = 0;
	bool seq = false;
	bool len = false;
	bool ispartial = false;
	bool iscomplete = false;
	if (buffer[iloc] & 0x4)
	{
		//Has seq
		seq = true;
		headersize += 1;
	}
	if (buffer[iloc] & 0x1)
	{
		//Has length
		len = true;
		headersize += 2;
	}
	if (buffer[iloc] & 0x8)
	{
		//Is partial packet
		ispartial = true;
		headersize += 1;

	}
	if (buffer[iloc] & 0x16)
	{
		//Is final packet of a partial sequence
		iscomplete = true;
		headersize += 1;
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
	}
	else
	{
		retval.haslength = false;
	}
	if (ispartial)
	{
		retval.isPartial = true;
		retval.partialSequence = (unsigned char)buffer[iloc];
		iloc += 1;
	}
	else
	{
		retval.isPartial = false;
	}
	if (iscomplete)
	{
		retval.isComplete = true;
		retval.partialSequence = (unsigned char)buffer[iloc];
		iloc += 1;
	}
	else
	{
		retval.isComplete = false;
	}
	if (retval.haslength)
	{
		if ((unsigned int)buffer.length() > (unsigned int)(retval.length + iloc))
		{
			qDebug() << "Packet length should be:" << retval.length + iloc << "But it is" << buffer.length();
			//emit decoderFailure(buffer);
			return Packet(false);
		}
		payload.append(buffer.mid(iloc,retval.length));
	}
	else
	{
		payload.append(buffer.mid(iloc),(buffer.length()-iloc));
	}

	/*QString output;
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
	}*/
	//Last byte of currPacket should be out checksum.
	retval.header = header;
	retval.payload = payload;
	if (header[0] & NAK)
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
		qDebug() << "Packet header under minimum header size" << retval.header.size();
		return Packet(false);
	}
}
void FEMSParser::loadLog(QString log)
{
	m_fileName = log;
	start();
}
void FEMSParser::decodePayload(QByteArray payload)
{
	QVariantMap m_valueMap;
	for (int i=0;i<m_dataFieldList.size();i++)
	{
		if (m_dataFieldList[i].isFlag())
		{
			bool value = m_dataFieldList[i].flagValue(&payload);
			m_valueMap[m_dataFieldList[i].name()] = value;
		}
		else
		{
			double value = m_dataFieldList[i].getValue(&payload);
			m_valueMap[m_dataFieldList[i].name()] = value;
		}
	}
	emit payloadDecoded(m_valueMap);
}
QString FEMSParser::getStringFromEnum(RequestType type)
{
	switch (type)
	{
		case GET_INTERFACE_VERSION:
			return "GET_INTERFACE_VERSION";
		case GET_FIRMWARE_VERSION:
			return "GET_FIRMWARE_VERSION";
		case GET_MAX_PACKET_SIZE:
			return "GET_MAX_PACKET_SIZE";
		case ECHO_PACKET:
			return "ECHO_PACKET";
		case SOFT_RESET:
			return "SOFT_RESET";
		case HARD_RESET:
			return "HARD_RESET";
		case UPDATE_BLOCK_IN_RAM:
			return "UPDATE_BLOCK_IN_RAM";
		case UPDATE_BLOCK_IN_FLASH:
			return "UPDATE_BLOCK_IN_FLASH";
		case RETRIEVE_BLOCK_IN_RAM:
			return "RETRIEVE_BLOCK_IN_RAM";
		case RETRIEVE_BLOCK_IN_FLASH:
			return "RETRIEVE_BLOCK_IN_FLASH";
		case BURN_BLOCK_FROM_RAM_TO_FLASH:
			return "BURN_BLOCK_FROM_RAM_TO_FLASH";
		case GET_DATALOG_DESCRIPTOR:
			return "GET_DATALOG_DESCRIPTOR";
		case BENCHTEST:
			return "BENCHTEST";
		case GET_LOCATION_ID_LIST:
			return "GET_LOCATION_ID_LIST";
		case GET_DECODER_NAME:
			return "GET_DECODER_NAME";
		case GET_FIRMWARE_BUILD_DATE:
			return "GET_FIRMWARE_BUILD_DATE";
		case GET_COMPILER_VERSION:
			return "GET_COMPILER_VERSION";
		case GET_OPERATING_SYSTEM:
			return "GET_OPERATING_SYSTEM";
		case GET_BUILT_BY_NAME:
			return "GET_BUILT_BY_NAME";
		case GET_SUPPORT_EMAIL:
			return "GET_SUPPORT_EMAIL";
		case GET_LOCATION_ID_INFO:
			return "GET_LOCATION_ID_INFO";
	}
	return "";
}

void FEMSParser::loadDataFieldsFromValues()
{
	// CoreVars
	m_dataFieldList.append(FEMSDataField("IAT","Intake Air Temperature","",0,2,100,-273.15));
	m_dataFieldList.append(FEMSDataField("CHT","Coolant/Head Temperature","",2,2,100,-273.15));
	m_dataFieldList.append(FEMSDataField("TPS","Throttle Position Sensor","",4,2,640.0));
	m_dataFieldList.append(FEMSDataField("EGO","Exhaust Gas Oxygen","",6,2,32768.0));
	m_dataFieldList.append(FEMSDataField("MAP","Manifold Air Pressure","",8,2,100.0));
	m_dataFieldList.append(FEMSDataField("AAP","Ambient Atmosphere Pressure","",10,2,100.0));
	m_dataFieldList.append(FEMSDataField("BRV","Battery Reference Voltage","",12,2,1000.0));
	m_dataFieldList.append(FEMSDataField("MAT","Manifold Air Temperature","",14,2,100.0,-273.15));
	m_dataFieldList.append(FEMSDataField("EGO2","Exhaust Gas Oxygen 2","",16,2,32768.0));
	m_dataFieldList.append(FEMSDataField("IAP","Intercooler Absolute Pressure","",18,2,100.0));
	m_dataFieldList.append(FEMSDataField("MAF","Mass Air Flow","",20,2,204.8));
	m_dataFieldList.append(FEMSDataField("DMAP","Delta Map","",22,2,1.0));
	m_dataFieldList.append(FEMSDataField("DTPS","Delta TPS","",24,2,1.0));
	m_dataFieldList.append(FEMSDataField("RPM","Engine Speed","",26,2,2.0));
	m_dataFieldList.append(FEMSDataField("DRPM","Delta RPM","",28,2,1.0));
	m_dataFieldList.append(FEMSDataField("DDRPM","Delta Delta RPM","",30,2,1.0));

	// DerivedVars
	m_dataFieldList.append(FEMSDataField("LoadMain","Configurable Unit of Load","",32,2,512.0));
	m_dataFieldList.append(FEMSDataField("VEMain","Volumentric Efficiency","",34,2,512.0));
	m_dataFieldList.append(FEMSDataField("Lambda","Integral Lambda","",36,2,32768.0));
	m_dataFieldList.append(FEMSDataField("AirFlow","AirFlow for Alpha-N","",38,2,640.0));
	m_dataFieldList.append(FEMSDataField("densityAndFuel","raw Intermediate","",40,2,1.0));
	m_dataFieldList.append(FEMSDataField("BasePW","Raw PW Before corrections","",42,2,1250));
	m_dataFieldList.append(FEMSDataField("ETE","Engine Temp Enrichment Percentage","",44,2,16384.0/100.0));
	m_dataFieldList.append(FEMSDataField("TFCTotal","Total Transient Fuel Corrections","",46,2,1250)); // Needs to be signed short / int16
	m_dataFieldList.append(FEMSDataField("EffectivePW","Actual PW of fuel delivery","",48,2,1250.0));
	m_dataFieldList.append(FEMSDataField("IDT","PW duration before fuel flow begins","",50,2,1250.0));
	m_dataFieldList.append(FEMSDataField("RefPW","Reference electrical PW","",52,2,1250.0));
	m_dataFieldList.append(FEMSDataField("Advance","Ignition advance","",54,2,50.0));
	m_dataFieldList.append(FEMSDataField("Dwell","Dwell period","",56,2,1250.0));

	// KeyUserDebug
	m_dataFieldList.append(FEMSDataField("tempClock","Once per datalog message","",58,1,1.0));
	m_dataFieldList.append(FEMSDataField("spareChar","Unused at this time","",59,1,1.0));

	// TODO bits:
	m_dataFieldList.append(FEMSDataField("coreStatusA","Duplicate","",60,1,1.0,0,true)); // Needs flags
	m_dataFieldList.append(FEMSDataField("decoderFlags","Various decoder state flags","",61,1,1.0,0,true)); // Needs flags

	// Flaggable flags
	m_dataFieldList.append(FEMSDataField("callsToUISRs","to ensure we aren't accidentally triggering unused ISRs.","",62,2,1.0,0,0,0,true,0));
	m_dataFieldList.append(FEMSDataField("lowVoltageConditions","low voltage conditions.","",62,2,1.0,0,0,0,true,1));
	m_dataFieldList.append(FEMSDataField("decoderSyncLosses","Number of times cam, crank or combustion sync is lost.","",62,2,1.0,0,0,0,true,2));
	m_dataFieldList.append(FEMSDataField("spare","spare","",62,2,1.0,0,0,0,true,3));
	m_dataFieldList.append(FEMSDataField("decoderSyncStateClears","Sync loss called when not synced yet, thus discarding data and preventing sync.","",62,2,1.0,0,0,0,true,4));
	m_dataFieldList.append(FEMSDataField("serialNoiseErrors","Incremented when noise is detected","",62,2,1.0,0,0,0,true,5));
	m_dataFieldList.append(FEMSDataField("serialFramingErrors","Incremented when a framing error occurs","",62,2,1.0,0,0,0,true,6));
	m_dataFieldList.append(FEMSDataField("serialParityErrors","Incremented when a parity error occurs","",62,2,1.0,0,0,0,true,7));
	m_dataFieldList.append(FEMSDataField("serialOverrunErrors","Incremented when overrun occurs (count available in KeyUserDebug below","",62,2,1.0,0,0,0,true,8));
	m_dataFieldList.append(FEMSDataField("serialEscapePairMismatches","Incremented when an escape is found but not followed by an escapee","",62,2,1.0,0,0,0,true,9));
	m_dataFieldList.append(FEMSDataField("serialStartsInsideAPacket","Incremented when a start byte is found inside a packet","",62,2,1.0,0,0,0,true,10));
	m_dataFieldList.append(FEMSDataField("serialPacketsOverLength","Incremented when the buffer fills up before the end","",62,2,1.0,0,0,0,true,11));
	m_dataFieldList.append(FEMSDataField("serialChecksumMismatches","Incremented when calculated checksum did not match the received one","",62,2,1.0,0,0,0,true,12));
	m_dataFieldList.append(FEMSDataField("serialPacketsUnderLength","Incremented when a packet is found that is too short","",62,2,1.0,0,0,0,true,13));
	m_dataFieldList.append(FEMSDataField("commsDebugMessagesNotSent","Incremented when a debug message can't be sent due to the TX buffer","",62,2,1.0,0,0,0,true,14));
	m_dataFieldList.append(FEMSDataField("commsErrorMessagesNotSent","Incremented when an error message can't be sent due to the TX buffer","",62,2,1.0,0,0,0,true,15));

	m_dataFieldList.append(FEMSDataField("currentEvent","Which input event was last","",64,1,1.0));
	m_dataFieldList.append(FEMSDataField("syncLostWithThisID","UID for reason beind loss of sync","",65,1,1.0));
	m_dataFieldList.append(FEMSDataField("syncLostOnThisEvent","Where in the input pattern sync was lost","",66,1,1.0));
	m_dataFieldList.append(FEMSDataField("syncCaughtOnThisevent","Where in the input pattern sync was recovered","",67,1,1.0));
	m_dataFieldList.append(FEMSDataField("syncResetCalls","Sum of losses, corrections, and state clears","",68,1,1.0));
	m_dataFieldList.append(FEMSDataField("primaryTeethSeen","","",69,1,1.0));
	m_dataFieldList.append(FEMSDataField("secondaryTeethSeen","","",70,1,1.0));
	m_dataFieldList.append(FEMSDataField("serialOverrunErrorsCount","","",71,1,1.0)); //These three have "Count" added to the end of the name to avoid conflict with flags above.
	m_dataFieldList.append(FEMSDataField("serialHardwareErrorsCount","","",72,1,1.0));
	m_dataFieldList.append(FEMSDataField("serialAndCommsCodeErrorsCount","","",73,1,1.0));
	m_dataFieldList.append(FEMSDataField("inputEventTimeTolerance","","",74,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp10","","",76,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp9","","",78,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp8","","",80,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp7","","",82,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp6","","",84,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp5","","",86,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp4","","",88,2,1.0));
	m_dataFieldList.append(FEMSDataField("zsp3","","",90,2,1.0));
	m_dataFieldList.append(FEMSDataField("clockInMilliSeconds","Clock in milliseconds","",92,2,1.0));
	m_dataFieldList.append(FEMSDataField("clock8thMssInMillis","Clock in 8th milliseconds","",94,2,8.0));
	m_dataFieldList.append(FEMSDataField("ignitionLimiterFlags","","",96,1,1.0)); // Needs flags
	m_dataFieldList.append(FEMSDataField("injectionLimiterFlags","","",97,1,1.0)); // Needs flags
	m_logTypes.clear();
	for (int i=0;i<m_dataFieldList.size();i++)
	{
		//m_logTypes.append(m_dataFieldList.at(i).name());
	}
}
void FEMSParser::datalogDescriptor(QString data)
{
	qDebug() << data;
	QJsonDocument document = QJsonDocument::fromJson(data.toLatin1());
	if (document.isEmpty())
	{
		qDebug() << "Unable to parse datalog descriptor json";
		return;
	}
	QJsonObject toplevelobject = document.object();
	QJsonArray descriptorlist = toplevelobject.value("descriptor").toArray();
	m_dataFieldList.clear();
	m_logTypes.clear();
	for (int i=0;i<descriptorlist.size();i++)
	{
		QJsonObject itemmap = descriptorlist.at(i).toObject();
		int start = itemmap.value("start").toInt();
		int size = itemmap.value("size").toInt();
		bool issigned = itemmap.value("is_signed").toBool();
		QString name = itemmap.value("name").toString();
		QString desc = itemmap.value("description").toString();
		QString suffix = itemmap.value("suffix").toString();
		float mult = itemmap.value("multiplier").toString().toFloat();
		float adder = itemmap.value("adder").toString().toFloat();
		m_dataFieldList.append(FEMSDataField(name,desc,suffix,start,size / 8,1 / mult,adder));
		m_logTypes.append(QPair<QString,QString>(name,suffix));

	}
	qDebug() << "DATALOG IMPLEMENTATION READ AND CHANGED";
	emit dataFormatChanged();

	//m_dataFieldList.append(DataField("IAT","Intake Air Temperature",0,2,100,-273.15));
	//m_dataFieldList.append(DataField("CHT","Coolant/Head Temperature",2,2,100,-273.15));
	//m_dataFieldList.append(DataField("TPS","Throttle Position Sensor",4,2,640.0));
}
