/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "firmwaremetadata.h"


FirmwareMetaData::FirmwareMetaData(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setColumnWidth(0,200);
	ui.tableWidget->setColumnWidth(1,400);
	ui.tableWidget->horizontalHeader()->hide();
	ui.tableWidget->verticalHeader()->hide();

}

FirmwareMetaData::~FirmwareMetaData()
{
}
void FirmwareMetaData::setInterrogationData(QMap<QString,QString> datamap)
{
	for (QMap<QString,QString>::const_iterator i=datamap.constBegin();i!=datamap.constEnd();i++)
	{
		for (int j=0;j<ui.tableWidget->rowCount();j++)
		{
			if (ui.tableWidget->item(j,0)->text() == i.key())
			{
				ui.tableWidget->item(j,1)->setText(i.value());
				return;
			}
		}
		ui.tableWidget->setRowCount(ui.tableWidget->rowCount()+1);
		ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,0,new QTableWidgetItem(i.key()));
		ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,new QTableWidgetItem(i.value()));
	}
}

void FirmwareMetaData::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
	emit windowHiding((QMdiSubWindow*)this->parent());
}
