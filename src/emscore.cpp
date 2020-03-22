/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "emscore.h"
#include "QsLog.h"
#include <QDir>
#include <QString>
#include <QCommandLineParser>
#include <QProcessEnvironment>

#define define2string_p(x) #x
#define define2string(x) define2string_p(x)
EMSCore *EMSCore::instancevar = 0;

EMSCore::EMSCore(int &argc, char *argv[]) : QApplication(argc,argv)
{
	QCommandLineParser parser;
	const QProcessEnvironment env(QProcessEnvironment::systemEnvironment());

	//Init the logger
	QsLogging::Logger& logger = QsLogging::Logger::instance();
	logger.setLoggingLevel(QsLogging::TraceLevel);

#ifdef Q_OS_WIN
	QString AppData(env.value("AppData").replace("\\","/"));
	QString UserProfile(env.value("UserProfile").replace("\\","/"));
	if (AppData.isEmpty())
	{
		AppData = UserProfile;
	}

	if (!QDir(AppData).exists("EMStudio"))
	{
		QDir(AppData).mkpath("EMStudio");
	}
	m_defaultsDir = QApplication::instance()->applicationDirPath();

	m_settingsDir = AppData + "/" + "EMStudio";
	m_localHomeDir = UserProfile + "/EMStudio";
#else
	//*nix is so much simpler.
	QString appDataDir = env.value("HOME");
	if (!QDir(appDataDir).exists(".EMStudio"))
	{
		QDir(appDataDir).mkpath(".EMStudio");
	}

	m_defaultsDir = QString(define2string(INSTALL_PREFIX)) + "/share/emstudio";
	m_settingsDir = appDataDir + "/" + ".EMStudio";
	m_localHomeDir = appDataDir + "/" + "EMStudio";
	//m_settingsFile = appDataDir + "/" + ".EMStudio/EMStudio-config.ini";
#endif

	QDir appDir(AppData);
	if (appDir.exists())
	{
		if (!appDir.cd("EMStudio"))
		{
			appDir.mkdir("EMStudio");
			appDir.cd("EMStudio");
		}
		if (!appDir.cd("applogs"))
		{
			appDir.mkdir("applogs");
		}
		if (!appDir.cd("profiles"))
		{
			appDir.mkdir("profiles");
		}
	}

	m_settingsFile = "settings.ini";
	//TODO: Figure out proper directory names

	if (!QFile::exists(m_localHomeDir + "/logs"))
	{
		QDir(m_localHomeDir).mkpath(m_localHomeDir + "/logs");
	}
	//Settings file should ALWAYS be the one in the settings dir. No reason to have it anywhere else.
	m_settingsFile = m_settingsDir + "/EMStudio-config.ini";
	const QString sLogPath(QDir(AppData + "/EMStudio/applogs").filePath("log.txt"));

	QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath, true, 0, 100));
	QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
	logger.addDestination(debugDestination);
	logger.addDestination(fileDestination);

	bool autoconnect = true;

	QCommandLineOption devOption(QStringList() << "d" << "dev", "Serial device", "file");
	QCommandLineOption pluginOption(QStringList() << "p" << "plugin", "Extra plugin", "file");
	QCommandLineOption autoconnectOption(QStringList() << "a" << "autoconnect", "Autoconnect", "file");

	parser.addOptions({ devOption, pluginOption, autoconnectOption });
	parser.addHelpOption();

	parser.process(*this);
	autoconnect = parser.isSet(autoconnectOption);

	if (parser.isSet(devOption)) {
		m_port = parser.value(devOption);
	}

	if (parser.isSet(pluginOption)) {
		m_plugin = parser.value(pluginOption);
	}

}
void EMSCore::run()
{
	mainWindow = new MainWindow();
	if (!m_port.isEmpty())
	{
		mainWindow->setDevice(m_port);
	}
	if (!m_plugin.isEmpty())
	{
	//A specific plugin is specified, override the plugin manager's choice.
	}
	//w->setPlugin(plugin);
	//if (autoconnect)
	//{
	//	w->connectToEms();
	//}


	QSettings settings(getSettingsFile(),QSettings::IniFormat);
	settings.beginGroup("plugin");
	QString savedPluginPath = settings.value("filename","").toString();
	settings.endGroup();
	mainWindow->show();
	/*if (savedPluginPath.isEmpty())
	{
		PluginManager *manager = new PluginManager();
		manager->show();
		mainWindow->connect(manager,SIGNAL(fileSelected(QString)),mainWindow,SLOT(setPlugin(QString)));
	}
	else
	{
		mainWindow->setPlugin(savedPluginPath);
	}*/
	mainWindow->setPlugin(savedPluginPath);
}

