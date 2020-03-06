/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef DATAVIEW_H
#define DATAVIEW_H
#include <QWidget>
#include <QVariantMap>
#include "tabledata.h"
class DataView : public QWidget
{
public:
	virtual bool setData(unsigned short locationid,DataBlock *tdata)=0;
	//virtual bool setData(unsigned short locationid,QByteArray data)=0;
	virtual void passDatalog(QVariantMap data)=0;
};

#endif // DATAVIEW_H
