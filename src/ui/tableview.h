/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef DATATABLES_H
#define DATATABLES_H

#include <QWidget>
#include <QTimer>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "ui_datatables.h"
#include "datapacketdecoder.h"
class TableView : public QWidget
{
    Q_OBJECT

public:
	explicit TableView(QWidget *parent = 0);
	~TableView();
	void passData(QVariantMap data);
	void passDecoder(DataPacketDecoder *decoder);
private:
	QTimer *guiUpdateTimer;
	DataPacketDecoder *dataPacketDecoder;
	Ui::DataTables ui;
	QVariantMap m_valueMap;
	QMap<QString,int> m_nameToIndexMap;
protected:
	void closeEvent(QCloseEvent *event);
private slots:
	void guiUpdateTimerTick();
	void dataFormatChanged();
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // DATATABLES_H
