/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "ui_aboutview.h"

class AboutView : public QWidget
{
	Q_OBJECT
	
public:
	explicit AboutView(QWidget *parent = 0);
	~AboutView();
	void setHash(QString hash);
	void setVersionLabel(QString version);
	void setCommit(QString commit);
	void setDate(QString date);
protected:
	void closeEvent(QCloseEvent *event);
private:
	QString m_commit;
	QString m_hash;
	Ui::AboutView ui;
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // ABOUTVIEW_H
