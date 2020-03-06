/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef DATASELECTIONSCREEN_H
#define DATASELECTIONSCREEN_H

#include <QWidget>
#include "ui_dataselectionscreen.h"

class DataSelectionScreen : public QWidget
{
	Q_OBJECT
	
public:
	explicit DataSelectionScreen(QWidget *parent = 0);
	~DataSelectionScreen();
	void addItem(QString name);
	void clear();
signals:
	void itemEnabled(QString name);
	void itemDisabled(QString name);
private slots:
	void checkBoxClicked(bool checked);
private:
	Ui::DataSelectionScreen ui;
};

#endif // DATASELECTIONSCREEN_H
