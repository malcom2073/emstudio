#ifndef PATHS_H
#define PATHS_H
#include <QtWidgets/QApplication>
#include <QString>
#include <QDir>
#define define2string_p(x) #x
#define define2string(x) define2string_p(x)


class Paths
{
	public:
	static QString getDefaultsDir()
	{
	#ifdef Q_OS_WIN
		QString defaultsDir = QApplication::instance()->applicationDirPath();
		return defaultsDir;
	#else //if Q_OS_LINUX
		QString defaultsDir = QString(define2string(INSTALL_PREFIX)) + "/share/emstudio";
		return defaultsDir;
	#endif
	}

	static QString getAppDataDir()
	{
	#ifdef Q_OS_WIN
		QString appDataDir = getenv("%AppData%");
		if (appDataDir == "")
		{
			appDataDir = getenv("AppData");
			if (appDataDir == "")
			{
				appDataDir = getenv("%UserProfile%");
				if (appDataDir == "")
				{
					appDataDir = getenv("UserProfile");
				}
			}
		}
		appDataDir = appDataDir.replace("\\","/");
		if (!QDir(appDataDir).exists("EMStudio"))
		{
			QDir(appDataDir).mkpath("EMStudio");
		}
		QString settingsDir = appDataDir + "/" + "EMStudio";
		return settingsDir;
	#else //if Q_OS_LINUX
		QString appDataDir = getenv("HOME");
		if (!QDir(appDataDir).exists(".EMStudio"))
		{
			QDir(appDataDir).mkpath(".EMStudio");
		}

		QString settingsDir = QString(define2string(INSTALL_PREFIX)) + "/share/emstudio";
		return settingsDir;
	#endif
	}

	static QString getLogsDir()
	{
	#ifdef Q_OS_WIN
		QString localHomeDir = QString(getenv("%USERPROFILE%")).replace("\\","/");
		if (localHomeDir == "")
		{
			localHomeDir = QString(getenv("USERPROFILE")).replace("\\","/");
		}
		localHomeDir += "/EMStudio";
		return localHomeDir;
		#else //if Q_OS_LINUX
		QString appDataDir = getenv("HOME");
		if (!QDir(appDataDir).exists("EMStudio"))
		{
			QDir(appDataDir).mkpath("EMStudio");
		}
		QString localHomeDir = appDataDir + "/" + "EMStudio";
		return localHomeDir;
	#endif
	}


};




#endif // PATHS_H

