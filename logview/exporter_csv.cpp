/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "exporter_csv.h"
#include <QFile>
Exporter_Csv::Exporter_Csv(QObject *parent) : QThread(parent)
{

}
void Exporter_Csv::startExport(QString filename,QList<QPair<QString, QString> > &typelist,QList<QVariantMap> &valuelist)
{
	m_filename = filename;
	m_typelist = &typelist;
	m_valuelist = &valuelist;
	start();
}

void Exporter_Csv::run()
{
	QFile output(m_filename);
	output.open(QIODevice::ReadWrite | QIODevice::Truncate);
	emit progress(0,m_typelist->size() + m_valuelist->size());
	for (int i=0;i<m_typelist->size();i++)
	{
		output.write(QString(m_typelist->at(i).first + ((i < m_typelist->size()-1) ? "," : "")).toLatin1());
		emit progress(i,m_typelist->size() + m_valuelist->size());

	}
	output.write("\r\n");
	for (int i=0;i<m_valuelist->size();i++)
	{
		//output.write(m_typelist->at(i) + ((i < m_typelist->size()-1) ? "," : ""));
		for (int j=0;j<m_typelist->size();j++)
		{
			output.write(QString(m_valuelist->at(i).value(m_typelist->at(j).first).toString() + ((j < m_typelist->size()-1) ? "," : "")).toLatin1());
		}
		output.write("\r\n");
		emit progress(m_typelist->size() + i,m_typelist->size() + m_valuelist->size());
	}
	output.close();
	emit done();
}
