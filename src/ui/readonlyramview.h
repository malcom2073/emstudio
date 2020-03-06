/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef READONLYRAMVIEW_H
#define READONLYRAMVIEW_H

#include <QWidget>
#include <QTimer>
#include "readonlyramdata.h"
#include "ui_readonlyramview.h"

class ReadOnlyRamView : public QWidget
{
	Q_OBJECT
	
public:
	explicit ReadOnlyRamView(QWidget *parent = 0);
	~ReadOnlyRamView();
	void passData(unsigned short locationid,QByteArray data,QList<ReadOnlyRamData> metadata);
	QTimer *readRamTimer;
	double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
private:
	unsigned short m_locationId;
	Ui::ReadOnlyRamView ui;
private slots:
	void readRamTimerTick();
signals:
	void readRamLocation(unsigned short locationid);
};

#endif // READONLYRAMVIEW_H
