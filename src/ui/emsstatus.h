/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EMSSTATUS_H
#define EMSSTATUS_H

#include <QDockWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>

#include "ui_emsstatus.h"

class QTimer;

class EmsStatus : public QDockWidget
{
	Q_OBJECT
	
public:
	explicit EmsStatus(QWidget *parent = 0);
	~EmsStatus();
	void passData(QVariantMap data);
	void setFlag(QLineEdit *edit,bool green);
public slots:
	void setEmsMemoryDirty();
	void setEmsMemoryClean();
private slots:
	void emsMemoryTimerTick();
protected:
	void closeEvent(QCloseEvent *event);
signals:
	void hardResetRequest();
	void softResetRequest();
	void windowHiding(QMdiSubWindow *parent);
private:
	Ui::EmsStatus ui;
	QTimer *emsMemoryTimer;
};

#endif // EMSSTATUS_H
