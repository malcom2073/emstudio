/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TSBPDATAPACKETDECODER_H
#define TSBPDATAPACKETDECODER_H
#include "datapacketdecoder.h"
#include "tsbpheaders.h"
#include "datafield.h"
class TSBPDataPacketDecoder : public DataPacketDecoder
{
	Q_OBJECT
public:
	TSBPDataPacketDecoder();
	void populateDataFields();
	int fieldSize();
	DataField getField(int num);
	void setDataDefinitions(QList<DataField> datamap);
	QString getFieldName(int num);
	QString getFieldDescription(int num);
private:
	QList<DataField> m_dataList;
signals:
	void payloadDecoded(QVariantMap data);
public slots:
	void decodePayload(QByteArray payload);

};

#endif // TSBPDATAPACKETDECODER_H
