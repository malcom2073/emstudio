/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "mapview.h"
#include "ui_mapview.h"
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

MapView::MapView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MapView)
{
	ui->setupUi(this);
	//connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(goButtonClicked()));
	connect(ui->widget,SIGNAL(rangesSet(double,double,double,double,double,double)),this,SLOT(rangesSet(double,double,double,double,double,double)));
	connect(ui->xAxisMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxX()));
	connect(ui->xAxisMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxX()));
	connect(ui->yAxisMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxY()));
	connect(ui->yAxisMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxY()));
	connect(ui->zAxisMinSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxZ()));
	connect(ui->zAxisMaxSpinBox,SIGNAL(editingFinished()),this,SLOT(updateMinMaxZ()));
	connect(ui->xAxisResetButton,SIGNAL(clicked()),ui->widget,SLOT(autoScaleX()));
	connect(ui->yAxisResetButton,SIGNAL(clicked()),ui->widget,SLOT(autoScaleY()));
	connect(ui->zAxisResetButton,SIGNAL(clicked()),ui->widget,SLOT(autoScaleZ()));
	connect(ui->xAxisComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(currentMapXChanged(QString)));
	connect(ui->yAxisComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(currentMapYChanged(QString)));
	connect(ui->zAxisComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(currentMapZChanged(QString)));

}

MapView::~MapView()
{
	delete ui;
}
void MapView::addItem(QString item)
{
	ui->xAxisComboBox->addItem(item);
	ui->yAxisComboBox->addItem(item);
	ui->zAxisComboBox->addItem(item);
}
void MapView::goButtonClicked()
{
	if (m_data->size() > 0)
	{
		if (m_data->at(0).contains(ui->xAxisComboBox->currentText()) && m_data->at(0).contains(ui->yAxisComboBox->currentText()) && m_data->at(0).contains(ui->zAxisComboBox->currentText()))
		{
			QList<double> x;
			QList<double> y;
			QList<double> z;
			for (int i=0;i<m_data->size();i++)
			{
                x.append(m_data->at(i).value(ui->xAxisComboBox->currentText()).toDouble());
                y.append(m_data->at(i).value(ui->yAxisComboBox->currentText()).toDouble());
                z.append(m_data->at(i).value(ui->zAxisComboBox->currentText()).toDouble());
			}
			ui->widget->passData(x,y,z);
		}
		else
		{
			qDebug() << "ERROR!";
		}
	}

}
void MapView::setData(QList<QVariantMap> *data,QList<QPair<QString, QString> > logtypes)
{
	QSqlDatabase db = QSqlDatabase::database("QSQLITE");
	QSqlQuery colnamequery(db);
	if (!colnamequery.exec("PRAGMA table_info(log)"))
	{
		qDebug() << "Unable to get table info:" << colnamequery.lastError().text();
	}
	while (colnamequery.next())
	{
		QString colname = colnamequery.value(1).toString();
		addItem(colname);
	}
	int rpmindex = ui->xAxisComboBox->findText("RPM");
	ui->xAxisComboBox->setCurrentIndex(rpmindex);
	int mapindex= ui->yAxisComboBox->findText("MAP");
	ui->yAxisComboBox->setCurrentIndex(mapindex);
	int egoindex = ui->zAxisComboBox->findText("O2");
	ui->zAxisComboBox->setCurrentIndex(egoindex);

}
void MapView::rangesSet(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax)
{
	ui->xAxisMinSpinBox->setValue(xmin);
	ui->xAxisMaxSpinBox->setValue(xmax);
	ui->yAxisMinSpinBox->setValue(ymin);
	ui->yAxisMaxSpinBox->setValue(ymax);
	ui->zAxisMinSpinBox->setValue(zmin);
	ui->zAxisMaxSpinBox->setValue(zmax);
}
void MapView::updateMinMaxX()
{
	ui->widget->setXMinMax(ui->xAxisMinSpinBox->value(),ui->xAxisMaxSpinBox->value());
}

void MapView::updateMinMaxY()
{
	ui->widget->setYMinMax(ui->yAxisMinSpinBox->value(),ui->yAxisMaxSpinBox->value());
}

void MapView::updateMinMaxZ()
{
	ui->widget->setZMinMax(ui->zAxisMinSpinBox->value(),ui->zAxisMaxSpinBox->value());
}
void MapView::currentMapXChanged(QString name)
{
	QSqlDatabase db = QSqlDatabase::database("QSQLITE");
	QSqlQuery selectlogquery(db);
	if (!selectlogquery.exec("select * from log"))
	{
		qDebug() << "Unable to get table info:" << selectlogquery.lastError().text();
	}
	QList<double> x;
	while (selectlogquery.next())
	{
		double val = selectlogquery.value(name).toDouble();
		x.append(val);
	}
	ui->widget->passXData(x);
	return;
}

void MapView::currentMapYChanged(QString name)
{
	QSqlDatabase db = QSqlDatabase::database("QSQLITE");
	QSqlQuery selectlogquery(db);
	if (!selectlogquery.exec("select * from log"))
	{
		qDebug() << "Unable to get table info:" << selectlogquery.lastError().text();
	}
	QList<double> y;
	while (selectlogquery.next())
	{
		double val = selectlogquery.value(name).toDouble();
		y.append(val);
	}
	ui->widget->passYData(y);
	return;
}

void MapView::currentMapZChanged(QString name)
{
	QSqlDatabase db = QSqlDatabase::database("QSQLITE");
	QSqlQuery selectlogquery(db);
	if (!selectlogquery.exec("select * from log"))
	{
		qDebug() << "Unable to get table info:" << selectlogquery.lastError().text();
	}
	QList<double> z;
	while (selectlogquery.next())
	{
		double val = selectlogquery.value(name).toDouble();
		z.append(val);
	}
	ui->widget->passZData(z);
	return;
}
