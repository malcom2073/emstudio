#include "megasquirtplugin.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

MegasquirtPlugin::MegasquirtPlugin(QObject *parent) : EmsLogViewPlugin(parent)
{
}
void MegasquirtPlugin::loadLog(QString log)
{
	m_fileName = log;
	start();
}

void MegasquirtPlugin::run()
{
	//Megasquirt log file, CSV
	QFile logfile(m_fileName);
	logfile.open(QIODevice::ReadOnly);
	bool foundfirst = false;
	bool foundsecond = false;
	QList<QString> fieldlist;
	QList<QString> unitlist;
	while (!logfile.atEnd())
	{
		QString line = logfile.readLine().replace("\r","").replace("\n","");
		if (line.startsWith("\"") || line.startsWith("MARK"))
		{
			//Ignore the line
		}
		else
		{
			if (!foundfirst && !foundsecond)
			{
				//This line is the header info
				QStringList linesplit = line.split("\t");
				foreach (QString item,linesplit)
				{
					fieldlist.append(item);
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
				QVariantMap result;
				QStringList linesplit = line.split("\t");
				if (linesplit.size() != fieldlist.size())
				{
					qDebug() << "Bad log line!:" << line;
					return;
				}
				for (int i=0;i<fieldlist.size();i++)
				{
					result[fieldlist[i]] = linesplit[i].toFloat();
				}
				emit payloadDecoded(result);
			}

		}
	}
	emit done();
}
