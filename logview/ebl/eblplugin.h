#ifndef EBLPLUGIN_H
#define EBLPLUGIN_H
#include "emslogviewplugin.h"
#include <QVariantMap>
class EBLPlugin : public EmsLogViewPlugin
{
	Q_OBJECT
	Q_INTERFACES(EmsLogViewPlugin)
public:
	EBLPlugin(QObject *parent=0);
	void loadLog(QString log);
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
private:
	void parseEblPacket(QByteArray origpacket);
	QString m_fileName;
protected:
	void run();
};

#endif // EBLPLUGIN_H
