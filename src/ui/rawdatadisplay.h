/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef RAWDATADISPLAY_H
#define RAWDATADISPLAY_H

#include <QWidget>
#include "ui_rawdatadisplay.h"
class RawDataDisplay : public QWidget
{
	Q_OBJECT

public:
	explicit RawDataDisplay(QWidget *parent = 0);
	~RawDataDisplay();

private:
	Ui::RawDataDisplay ui;
};

#endif // RAWDATADISPLAY_H
