/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "configview.h"
#include <QTreeWidget>
#include "QsLog.h"

ConfigView::ConfigView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	//itemChanged ( QTreeWidgetItem * item, int column )
	ui.treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
	//ui.treeWidget->setColumnCount(2);
	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
	connect(ui.treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(treeItemDoubleClicked(QTreeWidgetItem*,int)));
	connect(ui.treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(treeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}
void ConfigView::treeCurrentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *previous)
{
	Q_UNUSED(current)
	ui.treeWidget->closePersistentEditor(previous,0);
}

void ConfigView::treeItemDoubleClicked(QTreeWidgetItem* item,int column)
{
	if (item->parent() == 0)
	{
		//Item is a top level item, uneditable.
		QLOG_DEBUG() << "Top level item";
		return;
	}
	ui.treeWidget->openPersistentEditor(item,column);
}

void ConfigView::treeItemChanged(QTreeWidgetItem* item,int column)
{
	//Q_UNUSED(column)
	ui.treeWidget->closePersistentEditor(item,column);
	QLOG_DEBUG() << "Item changed:" << item->text(0);
	for (int i=0;i<ui.treeWidget->topLevelItemCount();i++)
	{
		if (ui.treeWidget->topLevelItem(i) == item->parent())
		{
			int index = ui.treeWidget->topLevelItem(i)->indexOfChild(item);
			int totaloffset = m_configList[i].offset() + index * m_configList[i].elementSize();
			unsigned short newval = backConvertAxis(item->text(0).toDouble(),m_configList[i].calc());
			QByteArray retval;
			if (m_configList[i].elementSize() == 2)
			{
				retval.append((char)((newval >> 8) & 0xFF));
				retval.append(newval & 0xFF);
			}
			else if (m_configList[i].elementSize() == 1)
			{
				retval.append(newval & 0xFF);
			}
			QLOG_DEBUG() << newval << totaloffset << m_configList[i].elementSize();
			m_rawData.replace(totaloffset,m_configList[i].elementSize(),retval);
			emit saveData(m_configList[i].locationId(),m_rawData);
		}
		QLOG_DEBUG() << ui.treeWidget->topLevelItem(i)->text(0) << item->text(0);
	}
	QLOG_DEBUG() << "No item found";
}
void ConfigView::passConfig(QList<ConfigBlock> config,QByteArray data)
{
	ui.treeWidget->clear();
	m_configList = config;
	m_rawData = data;
	for (int i=0;i<config.size();i++)
	{
		//ui.tableWidget->setRowCount(ui.tableWidget->rowCount()+1);
		//ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,0,new QTableWidgetItem(config[i].name()));
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << config[i].name());
		ui.treeWidget->addTopLevelItem(item);
		ui.treeWidget->setItemExpanded(item,true);

		if (config[i].type() == "value")
		{
			unsigned short val = 0;
			for (int j=0;j<config[i].size();j++)
			{
				val += ((unsigned char)data[config[i].offset() + j]) << (8 * (config[i].size() - (j+1)));
				QLOG_DEBUG() << j << (8 * (config[i].size() - (j+1))) << QString::number(data[config[i].offset() + j]);
			}
			QLOG_DEBUG() << "Item:" << config[i].name() << val;
			ui.treeWidget->topLevelItem(ui.treeWidget->topLevelItemCount()-1)->addChild(new QTreeWidgetItem(QStringList() << QString::number(calcAxis(val,config[i].calc()))));
			//int count = ui.treeWidget->topLevelItemCount()-1;
			//ui.treeWidget->topLevelItem(count)->child(ui.treeWidget->topLevelItem(count)->childCount()-1)->setFlags(ui.treeWidget->topLevelItem(count)->child(ui.treeWidget->topLevelItem(count)->childCount()-1)->flags() & Qt::ItemIsEditable);
			//ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,new QTableWidgetItem(QString::number(calcAxis(val,config[i].calc()))));

		}
		else if (config[i].type() == "array")
		{
			//QString text = "";
			int max = 0;
			if (config[i].sizeOverride() != "")
			{
				QLOG_DEBUG() << "We have a size override for" << config[i].name();
				for (int j=0;j<config.size();j++)
				{
					if (config[j].name() == config[i].sizeOverride())
					{
						QLOG_DEBUG() << "Found config override";
						unsigned short val = 0;
						for (int k=0;k<config[j].size();k++)
						{
							val += ((unsigned char)data[config[j].offset() + k]) << (8 * (config[j].size() - (k+1)));
							//val += data[config[i].offset() + j+1] << 8;
						}
						if (config[i].sizeOverrideMult() != 0)
						{
							val *= config[i].sizeOverrideMult();
						}
						max = val;

					}
				}
			}
			else
			{
				max = config[i].size();
			}
			QLOG_DEBUG() << "Max array size:" << max;
			for (int j=0;j<max;j++)
			{
				unsigned short val = 0;
				for (int k=0;k<config[i].elementSize();k++)
				{
					val += ((unsigned char)data[config[i].offset() + (j * config[i].elementSize()) + k]) << (8 * (config[i].elementSize() - (k+1)));
					//val += data[config[i].offset() + j+1] << 8;
				}
				ui.treeWidget->topLevelItem(ui.treeWidget->topLevelItemCount()-1)->addChild(new QTreeWidgetItem(QStringList() << QString::number(calcAxis(val,config[i].calc()))));
				//text += "{" + QString::number(calcAxis(val,config[i].calc())) + "} ";
				//j+=config[i].elementSize()-1;
			}
			//ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,new QTableWidgetItem(text));
			//ui.treeWidget->topLevelItem(ui.treeWidget->topLevelItemCount()-1)->addChild(new QTreeWidgetItem(QStringList() << text));
		}
	}
}
double ConfigView::calcAxis(unsigned short val,QList<QPair<QString,double> > metadata)
{
	if (metadata.size() == 0)
	{
		return val;
	}
	double newval = val;
	for (int j=0;j<metadata.size();j++)
	{
		if (metadata[j].first == "add")
		{
			newval += metadata[j].second;
		}
		else if (metadata[j].first == "sub")
		{
			newval -= metadata[j].second;
		}
		else if (metadata[j].first == "mult")
		{
			newval *= metadata[j].second;
		}
		else if (metadata[j].first == "div")
		{
			newval /= metadata[j].second;
		}
	}
	return newval;
}
unsigned short ConfigView::backConvertAxis(double val,QList<QPair<QString,double> > metadata)
{
	if (metadata.size() == 0)
	{
		return val;
	}
	double newval = val;
	for (int j=metadata.size()-1;j>=0;j--)
	{
		if (metadata[j].first == "add")
		{
			newval -= metadata[j].second;
		}
		else if (metadata[j].first == "sub")
		{
			newval += metadata[j].second;
		}
		else if (metadata[j].first == "mult")
		{
			newval /= metadata[j].second;
		}
		else if (metadata[j].first == "div")
		{
			newval *= metadata[j].second;
		}
	}
	return (unsigned short)newval;
}

ConfigView::~ConfigView()
{

}
