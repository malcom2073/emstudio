/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "scatterplotview.h"
#include "ui_scatterplotview.h"

ScatterPlotView::ScatterPlotView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ScatterPlotView)
{
	ui->setupUi(this);
}

ScatterPlotView::~ScatterPlotView()
{
	delete ui;
}
void ScatterPlotView::setData(QList<QVariantMap> *data,QList<QPair<QString, QString> > logtypes)
{
	ui->leftMapView->setData(data,logtypes);
	ui->rightMapView->setData(data,logtypes);
	//ui->bottomGraph->passData(data,logtypes);
}
