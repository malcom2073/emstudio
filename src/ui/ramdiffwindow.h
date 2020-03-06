/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef RAMDIFFWINDOW_H
#define RAMDIFFWINDOW_H

#include <QWidget>

namespace Ui {
class RamDiffWindow;
}

class RamDiffWindow : public QWidget
{
	Q_OBJECT
	
public:
	explicit RamDiffWindow(QWidget *parent = 0);
	~RamDiffWindow();
	void setDirtyLocation(unsigned short locationid);
	void setAcceptText(QString text);
	void setRejectText(QString text);
private:
	Ui::RamDiffWindow *ui;
signals:
	void acceptLocalChanges();
	void rejectLocalChanges();
};

#endif // RAMDIFFWINDOW_H
