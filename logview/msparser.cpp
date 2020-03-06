/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "msparser.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>

MSParser::MSParser(QObject *parent) : QThread(parent)
{
}
void MSParser::loadLog(QString log)
{
	m_fileName = log;
	start();
}

void MSParser::run()
{
	//Megasquirt log file, CSV
	QFile logfile(m_fileName);
	logfile.open(QIODevice::ReadOnly);
	bool foundfirst = false;
	bool foundsecond = false;
	QList<QString> fieldlist;
	QList<QString> unitlist;
	int count = 0;
	quint64 totallines=0;
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(":memory:");
	if (!db.open())
	{
		qDebug() << "Unable to open DB" << db.databaseName();
		return;
	}
	db.exec("PRAGMA JOURNAL_MODE=MEMORY");
	db.exec("PRAGMA SYNCHRONOUS=OFF");
	db.exec("PRAGMA TEMP_STORE=MEMORY");
	db.exec("PRAGMA LOCKING_MODE=EXCLUSIVE");
	db.exec("PRAGMA PAGE_SIZE=32768");
	QSqlQuery *insertquery = new QSqlQuery(db);
	//insertquery->prepare("INSERT into log (col1,col2) values (:col1,:col2);");
	//insertquery->exec();
	qint64 msecs = QDateTime::currentMSecsSinceEpoch();
	qint64 startmsec = msecs;
	QMap<QString,quint64> m_timerMap;
	m_timerMap["SPLIT"] = 0;
	m_timerMap["FIELDS"] = 0;
	m_timerMap["EXEC"] = 0;


	if (!db.transaction())
	{
		qDebug() << "Unable to start transaction" << db.lastError().text();
	}
	while (!logfile.atEnd())
	{
		QString line = logfile.readLine().replace("\r","").replace("\n","");
		if (line.startsWith("\"") || line.startsWith("MARK"))
		{
			//Ignore the line
		}
		else
		{


			totallines++;
			if (!foundfirst && !foundsecond)
			{
				//This line is the header info
				QStringList linesplit = line.split("\t");
				QString createquerytext;
				QString insertnamesquerytext;
				QString insertvaluesquerytext;
				createquerytext = "CREATE TABLE log (";
				insertnamesquerytext = "INSERT INTO log (";
				insertvaluesquerytext = "values (";

				foreach (QString item,linesplit)
				{
					fieldlist.append(":" + item);
					QString newitem = item.replace(" ","").replace("%","pct").replace("#","num").replace("/","p");
					createquerytext += newitem.replace(" ","").replace(":","").replace("-","").replace("*","") + " REAL,";
					insertnamesquerytext += newitem.replace(" ","").replace(":","").replace("-","").replace("*","") + ",";
					insertvaluesquerytext += ":" + newitem.replace(" ","").replace(":","").replace("-","").replace("*","") + ",";
				}
				QSqlQuery createquery(db);
				createquerytext = createquerytext.mid(0,createquerytext.length()-1);
				insertnamesquerytext = insertnamesquerytext.mid(0,insertnamesquerytext.length()-1);
				insertvaluesquerytext = insertvaluesquerytext.mid(0,insertvaluesquerytext.length()-1);
				createquerytext = createquerytext + ")";
				QString insertquerytext = insertnamesquerytext + ") " + insertvaluesquerytext + ")";


				if (!createquery.exec(createquerytext))
				{
					qDebug() << "Error execing statment:" << createquery.lastError().text();
					int stopper = 1;
				}
				if (!insertquery->prepare(insertquerytext))
				{
					qDebug() << "Error preparing insert:" << insertquery->lastError().text();
					int stopper = 1;
				}
				foundfirst = true;

			}
			else if (!foundsecond)
			{
				//Ignore units for now.
				foundsecond = true;
			}
			else
			{
				//Normal log
				msecs = QDateTime::currentMSecsSinceEpoch();
				QVariantMap result;
				QStringList linesplit = line.split("\t");
				if (linesplit.size() != fieldlist.size())
				{
					qDebug() << "Bad log line!:" << line;
					db.commit();
					return;
				}
				m_timerMap["SPLIT"] = m_timerMap["SPLIT"] + (QDateTime::currentMSecsSinceEpoch() - msecs);
				msecs = QDateTime::currentMSecsSinceEpoch();
				for (int i=0;i<fieldlist.size();i++)
				{
				//	result[fieldlist[i]] = linesplit[i].toFloat();
					insertquery->bindValue(fieldlist[i],linesplit[i]);
				}
				m_timerMap["FIELDS"] = m_timerMap["FIELDS"] + (QDateTime::currentMSecsSinceEpoch() - msecs);
				msecs = QDateTime::currentMSecsSinceEpoch();
				if (!insertquery->exec())
				{
					qDebug() << "Error execing insert:" << insertquery->lastError().text();
					int stopper = 1;
				}
				m_timerMap["EXEC"] = m_timerMap["EXEC"] + (QDateTime::currentMSecsSinceEpoch() - msecs);
				//emit payloadDecoded(result);
			}
			count++;
			if (count >= 500)
			{
				count = 0;
				emit loadProgress(logfile.pos(),logfile.size());
			}
		}
	}
	db.commit();
	qDebug() << "Time Taken:" << m_timerMap["SPLIT"] << m_timerMap["FIELDS"] << m_timerMap["EXEC"] << QDateTime::currentMSecsSinceEpoch() - startmsec;
	qDebug() << "Total:" << totallines;
	emit logData("Lines Loaded",QString::number(totallines));
	emit done();
}
