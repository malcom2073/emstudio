/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EMSINFO_H
#define EMSINFO_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "memorylocationinfo.h"
#include "datatype.h"
#include "ui_emsinfo.h"

class EmsInfoView : public QWidget
{
    Q_OBJECT

public:
	explicit EmsInfoView(QWidget *parent = 0);
	~EmsInfoView();
	void clear();

protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void locationInfoWidgetDoubleClicked(int row, int column);
	void locationIdInfo(unsigned short locationid,QString title,MemoryLocationInfo info);
	void normalViewClicked();
	void hexViewClicked();
private:
	Ui::EmsInfo ui;
signals:
	void displayLocationId(int locid, bool isram,FormatType type);
	void checkSync();
	void windowHiding(QMdiSubWindow *parent);
};

#endif // EMSINFO_H
