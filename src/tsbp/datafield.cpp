/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "datafield.h"
#include <QDebug>
DataField::DataField()
{
}
DataField::DataField(QString shortname,QString description,int offset,int size,double div,double addoffset,double min,double max,bool isFlags,int bit)
{
	m_offset = offset;
	m_size = size;
	m_div = div;
	m_name = shortname;
	m_description = description;
	m_min = min;
	m_max = max;
	m_addoffset = addoffset;
	m_isFlags = isFlags;
	m_bit = bit;
}
bool DataField::flagValue(QByteArray *payload)
{
	if (!m_isFlags)
	{
		return false;
	}
	if (payload->size() > m_offset+m_size)
	{
		unsigned int val = 0;
		for (int i=0;i<m_size;i++)
		{
			val += ((unsigned char)payload->at(m_offset+i)) << (8*(m_size-(i+1)));
		}
		return (m_bit & val);
	}
	return false;
}

float DataField::getValue(QByteArray *payload,bool translatebeforescale)
{
	if (payload->size() > m_offset+m_size)
	{
		float val = 0;
		for (int i=0;i<m_size;i++)
		{
			val += ((unsigned char)payload->at(m_offset+i)) << (8*(m_size-(i+1)));
		}
		if (translatebeforescale)
		{
			return (val + m_addoffset) * m_div;
		}
		else
		{
			return (val / m_div) + m_addoffset;
		}
	}
	return 0;
}
