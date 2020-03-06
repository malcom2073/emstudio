/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef FEMSPARSER_H
#define FEMSPARSER_H
#include "emslogviewplugin.h"
#include "femsdatafield.h"
#include <QVariantMap>
class FEMSParser : public QThread
{
	Q_OBJECT
public:
	FEMSParser(QObject *parent=0);
	void loadLog(QString log);
	QList<QPair<QString,QString> > getLogTypes() { return m_logTypes; }
private:
	class Packet
	{
	public:
		Packet(bool valid = true) { isValid = valid; }
		bool isNAK;
		bool isValid;
		QByteArray header;
		QByteArray payload;
		unsigned short payloadid;
		unsigned short length;
		bool haslength;
		bool hasseq;
		unsigned short sequencenum;
		bool isPartial;
		bool isComplete;
		unsigned char partialSequence;
	};
	enum RequestType
	{
		GET_INTERFACE_VERSION=0x0000,
		GET_FIRMWARE_VERSION=0x0002,
		GET_MAX_PACKET_SIZE=0x0004,
		ECHO_PACKET=0x0006,
		SOFT_RESET=0x0008,
		HARD_RESET=0x000A,
		UPDATE_BLOCK_IN_RAM=0x0100,
		UPDATE_BLOCK_IN_FLASH=0x0102,
		RETRIEVE_BLOCK_IN_RAM=0x0104,
		RETRIEVE_BLOCK_IN_FLASH=0x0106,
		BURN_BLOCK_FROM_RAM_TO_FLASH=0x0108,
		GET_DATALOG_DESCRIPTOR=0x0300,
		BENCHTEST=0x8888,
		GET_LOCATION_ID_LIST=0xDA5E,
		GET_DECODER_NAME=0xEEEE,
		GET_FIRMWARE_BUILD_DATE=0xEEF0,
		GET_COMPILER_VERSION=0xEEF2,
		GET_OPERATING_SYSTEM=0xEEF4,
		GET_BUILT_BY_NAME=0xEEF6,
		GET_SUPPORT_EMAIL=0xEEF8,
		GET_LOCATION_ID_INFO=0xF8E0,
		SERIAL_CONNECT=0xFFFF01,
		SERIAL_DISCONNECT=0xFFFF02,
		INTERROGATE_START=0xFFFF03
	};
	bool isPartialPacketWaiting;
	unsigned short m_currentPartialPacketSequence;
	QByteArray m_partialPacketBuffer;
	//FEDataPacketDecoder *decoder;
	QString m_fileName;
	int m_badChecksums;
	int m_falseStarts;
	int m_outOfPacketBytes;
	int m_lengthMismatch;
	int m_badLengthHeader;
	int m_badEscapeChar;
	int m_locationIdInfoReq;
	int m_locationIdInfoReply;
	Packet parseBuffer(QByteArray buffer);
	QList<FEMSDataField> m_dataFieldList;
	void decodePayload(QByteArray payload);
	void loadDataFieldsFromValues();
	void datalogDescriptor(QString data);
	QMap<unsigned short,int> m_payloadIdCounts;
	QString getStringFromEnum(RequestType type);
	QList<QPair<QString,QString> > m_logTypes;


protected:
	void run();
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
	void logData(QString key,QString val);
	void dataFormatChanged();
};

#endif // FEMSPARSER_H
