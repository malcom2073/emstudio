/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef WIZARDVIEW_H
#define WIZARDVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "emscomms.h"
#include "configblock.h"
#include "configdata.h"
#include "wizardwidget.h"

class WizardView : public QWidget
{
	Q_OBJECT
public:
	explicit WizardView(QWidget *parent = 0);
	void setFile(EmsComms *comms,QString filename);
	void passConfig(QMap<QString,QList<ConfigBlock> > config);
	void addConfig(QString name,ConfigData *config);
private:
	WizardWidget *m_wizardWidget;
	QWidget *widget;
protected:
	void closeEvent(QCloseEvent *event);
signals:
	void windowHiding(QMdiSubWindow *parent);
	void visibilityChanged(bool visible);

public slots:
};

#endif // WIZARDVIEW_H
