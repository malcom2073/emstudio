/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLineEdit>
#include "configblock.h"
#include "configdata.h"
#include "dialogfield.h"
#include "scalarparam.h"
#include "comboparam.h"
#include "ui_parameterwidget.h"

class ParameterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ParameterWidget(QWidget *parent = 0);
	//void addParam(QString title,DialogField field,ConfigBlock block);
	QList<unsigned short> getLocationIdList();
	void updateValue(unsigned short locationid,QByteArray block);
	void setValueRamFlash(unsigned short locationid,bool isRam,bool isFlash);
	void addParam(QString title,DialogField field,ConfigData* data);
private:
	double calcAxis(int val,QList<QPair<QString,double> > metadata);
	int backConvertAxis(double val,QList<QPair<QString,double> > metadata);
	QList<QPair<DialogField,ConfigBlock> > m_fieldConfigList;
	QMap<QString,QLineEdit*> m_nameToLineEditMap;
	//QScrollArea *scrollArea;
	//QVBoxLayout *mainLayout;
	//QWidget *scrollWidget;
	QList<ScalarParam*> m_scalarParamList;
	QList<ComboParam*> m_comboParamList;
	Ui::ParameterWidget ui;
private slots:
	void saveButtonClicked();
signals:
	void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
public slots:

};

#endif // PARAMETERWIDGET_H
