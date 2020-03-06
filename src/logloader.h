/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef LOGLOADER_H
#define LOGLOADER_H

#include <QThread>
//#include "datafield.h"
class LogLoader : public QThread
{
	Q_OBJECT
public:
	LogLoader(QObject *parent = 0);
	void loadFile(QString filename);
private:
	QString m_filename;
//	QList<DataField> *m_dataFieldList;
    void parseBuffer(QByteArray buffer);
protected:
	void run();
signals:

	void payloadReceived(QByteArray header,QByteArray payload);
	void logProgress(qlonglong current,qlonglong total);
	void endOfLog();
public slots:

};

#endif // LOGLOADER_H
