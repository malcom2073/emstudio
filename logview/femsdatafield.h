/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef FEMSDATAFIELD_H
#define FEMSDATAFIELD_H
#include <QString>

class FEMSDataField
{
public:
	//FEMSDataField();
	FEMSDataField(QString shortname,QString description,QString suffix,int offset,int size,double div,double addoffset=0,double min=0,double max=0,bool isFlags=false,int bit=0);
	float getValue(QByteArray *payload,bool translatebeforescale = false);
	QString description() { return m_description; }
	QString name() const { return m_name; }
	bool isFlag() { return m_isFlags; }
	bool flagValue(QByteArray *payload);
	int offset() { return m_offset; }
	int size() { return m_size; }
private:
	QString m_name;
	QString m_description;
	QString m_suffix;
	bool m_isFlags;
	int m_bit;
	int m_offset;
	int m_size;
	float m_div;
	float m_addoffset;
	float m_min;
	float m_max;

};

#endif // FEMSDATAFIELD_H
