/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef SCALARPARAM_H
#define SCALARPARAM_H

#include <QWidget>
#include "configdata.h"
#include "scalarconfigdata.h"
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
    void setConfig(ScalarConfigData *data);
	void saveValue();
private slots:
	void dataUpdate();
	void lineEditFinished();
private:
	Ui::ScalarParam *ui;
    ScalarConfigData *m_data;
};

#endif // SCALARPARAM_H
