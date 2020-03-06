/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "rawdatadisplay.h"


RawDataDisplay::RawDataDisplay(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.sendCommandTableWidget->setColumnCount(4);
	ui.sendCommandTableWidget->setColumnWidth(0,50);
	ui.sendCommandTableWidget->setColumnWidth(1,100);
	ui.sendCommandTableWidget->setColumnWidth(2,100);
	ui.sendCommandTableWidget->setColumnWidth(3,500);

}

RawDataDisplay::~RawDataDisplay()
{
}
