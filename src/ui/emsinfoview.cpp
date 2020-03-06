/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "emsinfoview.h"
#include <QMdiSubWindow>
#include <QMessageBox>
#include "QsLog.h"

EmsInfoView::EmsInfoView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.locationIdInfoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	connect(ui.locationIdInfoTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(locationInfoWidgetDoubleClicked(int,int)));
}
void EmsInfoView::normalViewClicked()
{
	if (ui.locationIdInfoTableWidget->selectedItems().size() == 0)
	{
		return;
	}
	int row = ui.locationIdInfoTableWidget->selectedItems()[0]->row();
	QString num = ui.locationIdInfoTableWidget->item(row,0)->text().split("x")[1];
	bool ok = false;
	//int locid = num.toInt(&ok,16);
	if (!ok)
	{
		QLOG_ERROR() << "Numeric conversion failed for:" << num;
		return;
	}
	//displayLocationId(locid,true,0); //0 for normal view, 1 for hex view
}

void EmsInfoView::hexViewClicked()
{
	if (ui.locationIdInfoTableWidget->selectedItems().size() == 0)
	{
		return;
	}
	int row = ui.locationIdInfoTableWidget->selectedItems()[0]->row();
	QString num = ui.locationIdInfoTableWidget->item(row,0)->text().split("x")[1];
	bool ok = false;
	//int locid = num.toInt(&ok,16);
	if (!ok)
	{
		QLOG_ERROR() << "Numeric conversion failed for:" << num;
		return;
	}
	//displayLocationId(locid,true,1); //0 for normal view, 1 for hex view
}

void EmsInfoView::clear()
{
	ui.locationIdInfoTableWidget->clearContents();
//	ui.firmwareVersionLineEdit->clear();
//	ui.interfaceVersionLineEdit->clear();
	ui.locationIdInfoTableWidget->setRowCount(0);
}


void EmsInfoView::locationInfoWidgetDoubleClicked(int row, int column)
{
	QLOG_DEBUG() << "Double click";
	Q_UNUSED(column)
	if (ui.locationIdInfoTableWidget->rowCount() <= row)
	{
		return;
	}
	QString num = ui.locationIdInfoTableWidget->item(row,0)->text().split("x")[1];
	bool ok = false;
	int locid = num.toInt(&ok,16);
	if (!ok)
	{
		QLOG_ERROR() << "Numeric conversion failed for:" << num;
		return;
	}
	FormatType type = TABLE_1D;
	if (ui.locationIdInfoTableWidget->item(row,2)->text().contains("2D Table"))
	{
		type = TABLE_2D_STRUCTURED;
	}
	else if (ui.locationIdInfoTableWidget->item(row,2)->text().contains("2D Signed Table"))
	{
		type = TABLE_2D_STRUCTURED;
	}
	else if (ui.locationIdInfoTableWidget->item(row,2)->text() == "3D Table")
	{
		type = TABLE_3D;
	}
	else if (ui.locationIdInfoTableWidget->item(row,2)->text() == "Lookup Table")
	{
		type = TABLE_1D;
	}
	else if (ui.locationIdInfoTableWidget->item(row,2)->text() == "Configuration")
	{
		type = TABLE_1D;
	}
	else if (ui.locationIdInfoTableWidget->item(row,2)->text().contains("2D 32bit Table"))
	{
		type = TABLE_2D_STRUCTURED;
	}


	/*if (ui.locationIdInfoTableWidget->item(row,9)->text().toLower() == "true" && ui.locationIdInfoTableWidget->item(row,10)->text().toLower() == "true")
	{
		if (QMessageBox::question(0,"Ram or flash?","Do you wish to open the RAM? (If not, flash will be opened)",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
		{
			//Is Ram
			emit displayLocationId(locid,true,type);
		}
		else
		{
			//is flash
			emit displayLocationId(locid,false,type);
		}
	}
	else if (ui.locationIdInfoTableWidget->item(row,9)->text().toLower() == "true")
	{
		//Is Ram
		emit displayLocationId(locid,true,type);
	}
	else if (ui.locationIdInfoTableWidget->item(row,10)->text().toLower() == "true")
	{
		//is flash
		emit displayLocationId(locid,false,type);
	}*/
	displayLocationId(locid,true,type);
}

void EmsInfoView::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
	emit windowHiding((QMdiSubWindow*)this->parent());
}
void EmsInfoView::locationIdInfo(unsigned short locationid,QString title,MemoryLocationInfo info)
{
	/*ui.locationIdInfoTableWidget->setColumnCount(17);

	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("LocID"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Table Name"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Flags"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Parent"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("RamPage"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("FlashPage"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("RamAddress"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(7,new QTableWidgetItem("FlashAddress"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(8,new QTableWidgetItem("Size"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(9,new QTableWidgetItem("Has Parent"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(10,new QTableWidgetItem("Is Ram"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(11,new QTableWidgetItem("Is Flash"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(12,new QTableWidgetItem("Is Index"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(13,new QTableWidgetItem("Is Read Only"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(14,new QTableWidgetItem("Is Verified"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(15,new QTableWidgetItem("For Backup"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(16,new QTableWidgetItem("Table Type"));
*/
	ui.locationIdInfoTableWidget->setColumnCount(3+info.propertymap.size());
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("LocID"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Table Name"));
	ui.locationIdInfoTableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Type"));
	for (int i=0;i<info.propertymap.size();i++)
	{
		ui.locationIdInfoTableWidget->setHorizontalHeaderItem(3+i,new QTableWidgetItem(info.propertymap[i].first));
	}
	Q_UNUSED(info)
	QLOG_TRACE() << "Location ID Info for:" << "0x" + QString::number(locationid,16).toUpper();
	int foundi = -1;
	bool ok = false;
	bool found = false;
	for (int i=0;i<ui.locationIdInfoTableWidget->rowCount();i++)
	{
		unsigned short test = ui.locationIdInfoTableWidget->item(i,0)->text().split("x")[1].toInt(&ok,16);
		if (test > locationid)
		{
			foundi = i;
			found = true;
			ui.locationIdInfoTableWidget->insertRow(i);
			break;
		}
	}
	if (!found)
	{
		ui.locationIdInfoTableWidget->setRowCount(ui.locationIdInfoTableWidget->rowCount()+1);
		foundi = ui.locationIdInfoTableWidget->rowCount()-1;
	}
	ui.locationIdInfoTableWidget->setItem(foundi,0,new QTableWidgetItem("0x" + QString::number(locationid,16).toUpper()));
	if (info.metaData.valid)
	{
		if ((info.metaData.formatId == TABLE_2D_STRUCTURED))
		{
			if (info.xAxis.size == 16)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("2D Table"));
			}
			else if (info.xAxis.size == 32)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("2D 32bit Table"));
			}
			else if (info.xAxis.isSigned)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("2D Signed Table"));
			}
		}
		else  if (info.metaData.formatId == TABLE_2D_LEGACY)
		{
			if (info.xAxis.size == 16)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Legacy 2D Table"));
			}
			else if (info.xAxis.size == 32)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Legacy 2D 32bit Table"));
			}
			else if (info.xAxis.isSigned)
			{
				ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Legacy 2D Signed Table"));
			}
		}
		else if (info.metaData.formatId == TABLE_3D)
		{
			ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("3D Table"));
		}
		else if (info.metaData.formatId == TABLE_1D)
		{
			ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Lookup Table"));
		}
		else
		{
			ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Undefined"));
		}
	}
	else
	{
		ui.locationIdInfoTableWidget->setItem(foundi,2,new QTableWidgetItem("Undefined"));
	}

	/*for (int i=1;i<17;i++)
	{
		ui.locationIdInfoTableWidget->setItem(foundi,i,new QTableWidgetItem(""));
	}*/
	//}
	ui.locationIdInfoTableWidget->setItem(foundi,1,new QTableWidgetItem(title));
	//ui.locationIdInfoTableWidget->item(foundi,2)->setText("0x" + QString::number(rawFlags,16).toUpper());
	for (int i=0;i<info.propertymap.size();i++)
	{
		ui.locationIdInfoTableWidget->setItem(foundi,3+i,new QTableWidgetItem(info.propertymap[i].second));
	}

	ui.locationIdInfoTableWidget->resizeColumnsToContents();
}
EmsInfoView::~EmsInfoView()
{
    //delete ui;
}
