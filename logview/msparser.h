/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef MSPARSER_H
#define MSPARSER_H

#include <QVariantMap>
#include <QThread>
class MSParser : public QThread
{
	Q_OBJECT
public:
	MSParser(QObject *parent=0);
	void loadLog(QString log);
private:
	QString m_fileName;
	QByteArray mybytes;
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
	void logData(QString key,QString val);
protected:
	void run();
};

#endif // MSPARSER_H
