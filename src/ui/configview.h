/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H

#include <QWidget>
#include "configblock.h"
#include "ui_configview.h"

class ConfigView : public QWidget
{
	Q_OBJECT
	
public:
	explicit ConfigView(QWidget *parent = 0);
	~ConfigView();
	void passConfig(QList<ConfigBlock> config,QByteArray data);
	double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
	unsigned short backConvertAxis(double val,QList<QPair<QString,double> > metadata);
private:
	Ui::ConfigView ui;
	QList<ConfigBlock> m_configList;
	QByteArray m_rawData;
signals:
	void saveData(unsigned short locationid,QByteArray data);
private slots:
	void treeItemChanged(QTreeWidgetItem *item,int column);
	void treeItemDoubleClicked(QTreeWidgetItem *item,int column);
	void treeCurrentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *previous);
};

#endif // CONFIGVIEW_H
