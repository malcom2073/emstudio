/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "graphview.h"
#include "ui_graphview.h"

GraphView::GraphView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphView)
{
    ui->setupUi(this);
    connect(ui->dataSelectWidget,SIGNAL(itemEnabled(QString)),this,SLOT(itemEnabled(QString)));
    connect(ui->dataSelectWidget,SIGNAL(itemDisabled(QString)),this,SLOT(itemDisabled(QString)));
}

GraphView::~GraphView()
{
    delete ui;
}
void GraphView::passData(QList<QVariantMap> *data,QList<QPair<QString,QString> > logtypes)
{
	m_dataPtr = data;

	m_logTypes = logtypes;
	if (logtypes.size() > 0)
	{
		for (int i=0;i<logtypes.size();i++)
		{
			ui->dataSelectWidget->addItem(logtypes.at(i).first);
		}
	}
	else
	{
		for (QMap<QString,QVariant>::const_iterator i=data->at(0).begin();i!=data->at(0).end();i++)
		{
			ui->dataSelectWidget->addItem(i.key());
		}
	}
}
void GraphView::itemEnabled(QString item)
{
    QList<float> list;
    for (int i=0;i<m_dataPtr->size();i++)
    {
        list.append(m_dataPtr->at(i)[item].toFloat());
    }
    QString suffix = "";
    for (int i=0;i<m_logTypes.size();i++)
    {
	    if (m_logTypes.at(i).first == item)
	    {
		    suffix = m_logTypes.at(i).second;
	    }
    }
    ui->graphWidget->addData(list,item,suffix);
}

void GraphView::itemDisabled(QString item)
{
    ui->graphWidget->removeData(item);
}
