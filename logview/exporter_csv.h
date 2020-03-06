/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EXPORTER_CSV_H
#define EXPORTER_CSV_H
#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QThread>

class Exporter_Csv : public QThread
{
	Q_OBJECT
public:
	Exporter_Csv(QObject *parent=0);
	void startExport(QString filename,QList<QPair<QString,QString> > &typelist,QList<QVariantMap> &valuelist);
private:
	QString m_filename;
	QList<QPair<QString, QString> > *m_typelist;
	QList<QVariantMap> *m_valuelist;
	void run();
signals:
	void progress(int pos,int tot);
	void done();
};

#endif // EXPORTER_CSV_H
