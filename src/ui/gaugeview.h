/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef GAUGEVIEW_H
#define GAUGEVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "ui_datagauges.h"
#include "gaugewidget.h"
#include "datapacketdecoder.h"
class GaugeView : public QWidget
{
	Q_OBJECT

public:
	explicit GaugeView(QWidget *parent = 0);
	~GaugeView();
	void passData(QVariantMap data);
	void passDecoder(DataPacketDecoder *decoder);
	QString setFile(QString file);
private:
	QList<QString> propertiesInUse;
	QString file;
	QTimer *guiUpdateTimer;
	DataPacketDecoder *dataPacketDecoder;
	QVariantMap m_valueMap;
	Ui::DataGauges ui;
	QWidget *widget;
	GaugeWidget *m_gaugeWidget;
protected:
	void closeEvent(QCloseEvent *event);
private slots:
	void guiUpdateTimerTick();
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // GAUGEVIEW_H
