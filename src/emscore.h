/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EMSCORE_H
#define EMSCORE_H

#include <QApplication>
#include <QSettings>
#include "mainwindow.h"
#include "pluginmanager.h"

class EMSCore : public QApplication
{
public:
    EMSCore(int &argc, char **argv);
	static EMSCore& instance(int &argc, char **argv)  // return a reference
	{
		if (instancevar == 0)
		{
			instancevar = new EMSCore(argc,argv);
		}
		return *instancevar;
	}
	static EMSCore& instance() //Default return an instance
	{
		return *instancevar;
	}
	static EMSCore *instancevar;
	void run();
	QString &getSettingsDirectory() { return m_settingsDir; }
	QString &getDefaultsDirectory() { return m_defaultsDir; }
	QString &getHomeDirectory() { return m_localHomeDir; }
	QString &getSettingsFile() { return m_settingsFile; }
private:
	QList<QPair<QString,QString> > getArgs(int argc, char **argv);
	void printHelp();

	MainWindow *mainWindow;
	PluginManager *pluginManager;

	QString m_settingsDir;
	QString m_defaultsDir;
	QString m_localHomeDir;
	QString m_settingsFile;
	QString m_port;
	QString m_plugin;

};

#endif // EMSCORE_H
