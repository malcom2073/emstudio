#ifndef FREEEMSPLUGIN_H
#define FREEEMSPLUGIN_H
#include "emslogviewplugin.h"
#include "fedatapacketdecoder.h"
class FreeEMSPlugin : public EmsLogViewPlugin
{
	Q_OBJECT
	Q_INTERFACES(EmsLogViewPlugin)
public:
	FreeEMSPlugin(QObject *parent=0);
	void loadLog(QString log);
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
	};
	FEDataPacketDecoder *decoder;
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
protected:
	void run();
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
};

#endif // FREEEMSPLUGIN_H
