/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tsbpdatapacketdecoder.h"
#include <QDebug>
TSBPDataPacketDecoder::TSBPDataPacketDecoder() : DataPacketDecoder()
{
}
void TSBPDataPacketDecoder::populateDataFields()
{

}

int TSBPDataPacketDecoder::fieldSize()
{
	return m_dataList.size();
}
QString TSBPDataPacketDecoder::getFieldName(int num)
{
	if (num >= 0 && num < m_dataList.size())
	{
		return m_dataList[num].name();
	}
	return "";
}

QString TSBPDataPacketDecoder::getFieldDescription(int num)
{
	if (num >= 0 && num < m_dataList.size())
	{
		return m_dataList[num].description();
	}
	return "";
}

DataField TSBPDataPacketDecoder::getField(int num)
{
	if (num >= 0 && num < m_dataList.size())
	{
		return m_dataList[num];
	}
	return DataField();
}
void TSBPDataPacketDecoder::setDataDefinitions(QList<DataField> datalist)
{
	m_dataList = datalist;
}

void TSBPDataPacketDecoder::decodePayload(QByteArray payload)
{
	/*QString val = "";
	for (int i=0;i<10;i++)
	{
		val += QString::number((unsigned char)payload[i]) + ",";
	}
	QLOG_DEBUG() << val;

	double rpm = (double)((((unsigned char)payload[1]) << 8) + (unsigned char)payload[2]);
	if (rpm == 0)
	{
//		map["RPM"] = 1;
	}
	else
	{
		map["RPM"] = rpm;
	}*/
	//unsigned short seconds = (((unsigned char)payload[0]) << 8) + ((unsigned char)payload[1]);
	//QVariantMap map;
	//map["Advance"] = seconds;
	//emit payloadDecoded(map);
	//return;
	QVariantMap map;
	for (int i=0;i<m_dataList.size();i++)
	{
		if (m_dataList[i].size() + m_dataList[i].offset() < payload.size())
		{
			double val = m_dataList[i].getValue(&payload,true);
			map[m_dataList[i].name()] = val;
			//QLOG_DEBUG() << "Decoding:" << m_dataList[i].name() << val;
		}

	}
	//QLOG_DEBUG() << "Decoding RPM:" << map["rpm"];
	emit payloadDecoded(map);
}
