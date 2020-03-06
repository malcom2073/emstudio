/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef COMBOPARAM_H
#define COMBOPARAM_H

#include <QWidget>
#include "configdata.h"

namespace Ui {
class ComboParam;
}

class ComboParam : public QWidget
{
	Q_OBJECT
	
public:
	explicit ComboParam(QWidget *parent = 0);
	~ComboParam();
	void setName(QString name);
	void setConfig(ConfigData *data);
	void saveValue();
private slots:
	void dataUpdate();
	void currentIndexChanged(int index);
private:
	Ui::ComboParam *ui;
	ConfigData *m_data;
};

#endif // COMBOPARAM_H
