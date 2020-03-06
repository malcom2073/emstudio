#ifndef EMSLOGVIEWPLUGIN_H
#define EMSLOGVIEWPLUGIN_H

#include <QThread>
#include <QString>
#include <QtPlugin>

class EmsLogViewPlugin : public QThread
{
	Q_OBJECT
public:
	EmsLogViewPlugin(QObject *parent) : QThread(parent) {}
	virtual void loadLog(QString log)=0;
};

Q_DECLARE_INTERFACE(EmsLogViewPlugin,"EmsLogViewPlugin/1.0")
#endif // EMSLOGVIEWPLUGIN_H
