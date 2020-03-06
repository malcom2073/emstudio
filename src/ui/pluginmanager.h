/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QWidget>

#include "ui_pluginmanager.h"

class QRadioButton;

class PluginManager : public QWidget
{
	Q_OBJECT

public:
	explicit PluginManager(QWidget *parent = 0);
	~PluginManager();

private:
	Ui::PluginManager ui;
	QMap<QRadioButton*,QString> m_buttonToFilenameMap;
	QString m_currentFilename;
private slots:
	void radioButtonClicked();
	void selectClicked();
	void cancelClicked();
signals:
	void fileSelected(QString filename);

};

#endif // PLUGINMANAGER_H
