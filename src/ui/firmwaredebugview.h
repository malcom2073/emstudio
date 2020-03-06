/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef FIRMWAREDEBUGVIEW_H
#define FIRMWAREDEBUGVIEW_H

#include <QWidget>
#include "ui_firmwaredebugview.h"
#include <QCloseEvent>
#include <QMdiSubWindow>

class FirmwareDebugView : public QWidget
{
	Q_OBJECT

public:
	explicit FirmwareDebugView(QWidget *parent = 0);
	~FirmwareDebugView();
protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::FirmwareDebugView ui;
public slots:
	void firmwareMessage(QString text);
private slots:
	void clearButtonClicked();
};

#endif // FIRMWAREDEBUGVIEW_H
