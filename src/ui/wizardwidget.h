/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef WIZARDWIDGET_H
#define WIZARDWIDGET_H

#include <QtQuick/QQuickView>
#include <QtQml/QQmlPropertyMap>
#include <QTimer>
#include <emscomms.h>
class WizardWidget : public QQuickView
{
	Q_OBJECT

public:
	Q_INVOKABLE void addConfigRequest(QString name);
	explicit WizardWidget(QQuickView *parent = 0);
	Q_INVOKABLE void setMemory(unsigned short locationid,unsigned short offset,unsigned short length,QVariantList array);
	void setFile(EmsComms *comms,QString filename);
	void passConfig(QMap<QString,QList<ConfigBlock> > config);
	void addConfig(QString name,ConfigData *config);
private:
	QQmlPropertyMap m_configPropertyMap;
	QMap<QString,QList<ConfigBlock> > m_configBlock;
	QTimer *updateTimer;
	QVariantMap m_savedPayload;
	EmsComms *emscomms;
	QList<QString> m_configRequestList;
	QMap<QString,ConfigBlock*> m_configBlockMap;
protected:
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
signals:
	void payloadDecoded(QVariantMap data);
	void visibilityChanged(bool visible);
public slots:
	void configRecieved(ConfigBlock,QVariant);
	void configUpdate();
private slots:
	void decoderPayloadDecoded(QVariantMap values);
	void updateTimerTick();
};

#endif // WIZARDWIDGET_H
