/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef SCALARPARAM_H
#define SCALARPARAM_H

#include <QWidget>
#include "configdata.h"

namespace Ui {
class ScalarParam;
}

class ScalarParam : public QWidget
{
	Q_OBJECT
	
public:
	explicit ScalarParam(QWidget *parent = 0);
	~ScalarParam();
	void setName(QString name);
	void setConfig(ConfigData *data);
	void saveValue();
private slots:
	void dataUpdate();
	void lineEditFinished();
private:
	Ui::ScalarParam *ui;
	ConfigData *m_data;
};

#endif // SCALARPARAM_H
