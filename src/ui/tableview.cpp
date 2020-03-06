/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tableview.h"
#include <QMdiSubWindow>

TableView::TableView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setColumnWidth(0,200);
	ui.tableWidget->setColumnWidth(1,100);
	ui.tableWidget->setColumnWidth(2,300);
	guiUpdateTimer = new QTimer(this);
	connect(guiUpdateTimer,SIGNAL(timeout()),this,SLOT(guiUpdateTimerTick()));
	guiUpdateTimer->start(250);

}
void TableView::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
	emit windowHiding((QMdiSubWindow*)this->parent());
}
void TableView::passDecoder(DataPacketDecoder *decoder)
{
	dataPacketDecoder = decoder;
	dataFormatChanged();
	connect(dataPacketDecoder,SIGNAL(dataFormatChanged()),this,SLOT(dataFormatChanged()));
}
void TableView::dataFormatChanged()
{
	m_nameToIndexMap.clear();
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(dataPacketDecoder->fieldSize());
	for (int i=0;i<dataPacketDecoder->fieldSize();i++)
	{
		m_nameToIndexMap[dataPacketDecoder->getFieldName(i)] = i;
		ui.tableWidget->setItem(i,0,new QTableWidgetItem(dataPacketDecoder->getFieldName(i)));
		ui.tableWidget->setItem(i,1,new QTableWidgetItem("0"));
		ui.tableWidget->setItem(i,2,new QTableWidgetItem(dataPacketDecoder->getFieldDescription(i)));
	}
}

TableView::~TableView()
{

}
void TableView::passData(QVariantMap data)
{
	m_valueMap = data;
}

void TableView::guiUpdateTimerTick()
{
	QVariantMap::const_iterator i = m_valueMap.constBegin();
	while (i != m_valueMap.constEnd())
	{
		if (i.value().type() == QVariant::Double)
		{
			//widget->propertyMap.setProperty(i.key().toAscii(),QVariant::fromValue(i.value()));
			double val = i.value().toDouble();
			QString valstr = QString::number(val,'f',2);
			if (m_nameToIndexMap.contains(i.key()))
			{
				ui.tableWidget->item(m_nameToIndexMap[i.key()],1)->setText(valstr);
			}
		}
		//QLOG_DEBUG() << i.key() << m_nameToIndexMap[i.key()] << i.value();
		i++;
	}

}
