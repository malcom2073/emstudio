/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QWidget>
#include <QMdiSubWindow>
#include "ui_comsettings.h"

class ComSettings : public QWidget
{
	Q_OBJECT

public:
	Q_PROPERTY(QString comport READ getComPort WRITE setComPort)
	explicit ComSettings(QWidget *parent = 0);
	~ComSettings();
	QString getComPort();
	void setComPort(QString port);
	int getBaud();
	QString getDataLogDir();
	bool getSaveDataLogs();
	bool getClearDataLogs();
	int getNumLogsToSave();
	void setDataLogDir(QString dir);
	void setSaveDataLogs(bool save);
	void setClearDataLogs(bool clear);
	void setNumLogsToSave(int num);
	void setBaud(int baud);
	int getInterByteDelay();
	void setInterByteDelay(int delay);
protected:
	void closeEvent(QCloseEvent *event);
private slots:
	void browseLogFolderClicked();
	void chooseColorClicked();
private:
	Ui::ComSettings ui;
signals:
	void saveClicked();
	void cancelClicked();
	void windowHiding(QMdiSubWindow *parent);
};

#endif // COMSETTINGS_H
