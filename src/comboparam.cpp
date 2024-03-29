/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "comboparam.h"
#include "ui_comboparam.h"

ComboParam::ComboParam(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ComboParam)
{
	ui->setupUi(this);
	connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
}

ComboParam::~ComboParam()
{
	delete ui;
}
void ComboParam::setName(QString name)
{
	ui->label->setText(name);
    m_name = name;
}

void ComboParam::setConfig(QString variable,BitConfigData *data)
{
//	disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
//	ui->comboBox->addItems(data->getEnumValues());
//	ui->comboBox->setCurrentIndex(data->value().toInt());
	m_data = data;
    ui->comboBox->clear();
    QList<QString> bitfields = data->getBitFields(variable);
    for (int i=0;i<bitfields.length();i++)
    {
        ui->comboBox->addItem(bitfields.at(i));
    }
    ui->comboBox->setCurrentText(m_data->getValue(variable).toString());
    m_variable = variable;
    //connect(data,SIGNAL(update()),this,SLOT(dataUpdate()));
    //connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
}

void ComboParam::saveValue()
{
	//m_data->setValue(ui->comboBox->currentIndex());
    m_data->saveToDevice();
}
void ComboParam::dataUpdate()
{
	disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
//	ui->comboBox->setCurrentIndex(m_data->value().toInt());
	connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
}
void ComboParam::currentIndexChanged(int index)
{
    m_data->setValue(m_variable,index);
//	m_data->setValue(index);
}
