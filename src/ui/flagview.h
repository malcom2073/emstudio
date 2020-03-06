/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/
#ifndef DATAFLAGS_H
#define DATAFLAGS_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "datapacketdecoder.h"
#include "ui_dataflags.h"
class FlagView : public QWidget
{
	Q_OBJECT

public:
	explicit FlagView(QWidget *parent = 0);
	~FlagView();
	void passData(QVariantMap data);
	void passDecoder(DataPacketDecoder *decoder);
protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::DataFlags ui;
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // DATAFLAGS_H
