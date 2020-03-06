#ifndef MEGASQUIRTPLUGIN_H
#define MEGASQUIRTPLUGIN_H

#include "emslogviewplugin.h"
#include <QVariantMap>
class MegasquirtPlugin : public EmsLogViewPlugin
{
	Q_OBJECT
	Q_INTERFACES(EmsLogViewPlugin)
public:
	MegasquirtPlugin(QObject *parent=0);
	void loadLog(QString log);
private:
	QString m_fileName;
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
protected:
	void run();
};

#endif // MEGASQUIRTPLUGIN_H
