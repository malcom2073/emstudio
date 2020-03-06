/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "pluginmanager.h"
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QRadioButton>
#include <QDebug>
#include "paths.h"

PluginManager::PluginManager(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	/*
	pluginLoader = new QPluginLoader(this);
	pluginLoader->setFileName(m_pluginFileName);
	qDebug() << pluginLoader->metaData();
	for (QJsonObject::const_iterator i = pluginLoader->metaData().constBegin();i!=pluginLoader->metaData().constEnd();i++)
	{
		qDebug() << i.key() << i.value();
	}

	QLOG_INFO() << "Attempting to load plugin:" << m_pluginFileName;
*/
	m_currentFilename = "";
	QStringList dirlist;
	dirlist.append(QApplication::applicationDirPath() + "/plugins"); //Installed folder
	dirlist.append("plugins"); //Default local
	dirlist.append(QString(define2string(INSTALL_PREFIX)) + "/share/emstudio/plugins"); //make installed on linux
	dirlist.append("C:\\Program Files (x86)\\EMStudio\\plugins"); //Default on Windows
	foreach (QString dirstr,dirlist)
	{
		QDir dir(dirstr);
		foreach (QString file,dir.entryList())
		{
			QString absolutepath = dir.absoluteFilePath(file);
			QPluginLoader loader(absolutepath);
			QJsonObject meta = loader.metaData();
			qDebug() << meta;
			if (meta.contains("MetaData"))
			{
				QJsonObject metaDataObject = meta.value("MetaData").toObject();
				QString name = metaDataObject.value("Name").toString();
				QString version = metaDataObject.value("Version").toString();
				QString desc = metaDataObject.value("Description").toString();
				QRadioButton *button = new QRadioButton(this);
				connect(button,SIGNAL(clicked()),this,SLOT(radioButtonClicked()));
				button->setText("Name: " + name + "\nVersion: " + version + "\nDescription: " + desc + "\nMetaID: " + meta.value("IID").toString()+ "\nFile Path: " + absolutepath);
				QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui.groupBox->layout());
				layout->insertWidget(1,button);
				m_buttonToFilenameMap.insert(button,absolutepath);
			}
			else
			{
				qDebug() << "Plugin contains no meta";
			}
		}
	}
	connect(ui.savePushButton,SIGNAL(clicked()),this,SLOT(selectClicked()));
	connect(ui.cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));

	ui.savePushButton->setEnabled(false);
}

PluginManager::~PluginManager()
{
}
void PluginManager::radioButtonClicked()
{
	QRadioButton *send = qobject_cast<QRadioButton*>(sender());
	if (m_buttonToFilenameMap.contains(send))
	{
		m_currentFilename = m_buttonToFilenameMap.value(send);
		ui.savePushButton->setEnabled(true);
	}
}
void PluginManager::selectClicked()
{
	emit fileSelected(m_currentFilename);
	this->close();
}

void PluginManager::cancelClicked()
{
	this->close();
}
