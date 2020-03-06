/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef FIRMWAREMETADATA_H
#define FIRMWAREMETADATA_H

#include <QWidget>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include "ui_firmwaremetadata.h"


class FirmwareMetaData : public QWidget
{
	Q_OBJECT
	
public:
	explicit FirmwareMetaData(QWidget *parent = 0);
	~FirmwareMetaData();
	void setInterrogationData(QMap<QString,QString> datamap);
protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::FirmwareMetaData ui;
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // FIRMWAREMETADATA_H
