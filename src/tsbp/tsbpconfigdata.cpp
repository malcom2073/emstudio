/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tsbpconfigdata.h"
#include <QDebug>
void TSBPConfigData::setData(QByteArray data)
{
	m_valid = true;
	if (m_type == VALUE)
	{
		if (data.size() < m_offset + m_elementSize)
		{
			//Error, data is not value for this config.
			return;
		}
		QByteArray newdata = data.mid(m_offset,m_elementSize);
		unsigned long val = 0;
		for (int i=0;i<m_elementSize;i++)
		{
		    val += ((unsigned char)newdata[i]) << (((m_elementSize-1) - i) * 8);
		}
		m_value = QVariant(calcAxis(val,m_calc));
	}
	else if (m_type == ENUM)
	{
		if (data.size() < m_offset + m_elementSize)
		{
			//Error, data is not value for this config.
			return;
		}
		QByteArray newdata = data.mid(m_offset,m_elementSize);
		m_data = newdata;
		unsigned int newint = (unsigned char)newdata.at(0);
		unsigned int finalint = 0;
		for (int i=m_enumStart;i<=m_enumEnd;i++)
		{
			if (newint & (1 << i))
			{
				finalint += (1 << i);
			}
		}
		m_value = QVariant(finalint);
	}
	emit update();
}
QStringList TSBPConfigData::getEnumValues()
{
	return m_enumList;
}

void TSBPConfigData::setEnumValues(QStringList values)
{
	m_enumList = values;
}

void TSBPConfigData::setEnumBits(int min,int max)
{
	m_enumStart = min;
	m_enumEnd = max;
}

int TSBPConfigData::getMinEnumBit()
{
	return m_enumStart;
}

int TSBPConfigData::getMaxEnumBit()
{
	return m_enumEnd;
}

QVariant TSBPConfigData::value()
{
	if (!m_valid)
	{
		return QVariant();
	}
	return m_value;
}
void TSBPConfigData::saveToFlash()
{
	emit pageBurnRequested(m_locationId);
}

void TSBPConfigData::setValue(QVariant value)
{
	m_valid =  true;
	if (m_type == VALUE)
	{
		double dval = value.toDouble();
		unsigned short usval = reverseCalcAxis(dval,m_calc);
		QByteArray data;
		for (int i=0;i<m_elementSize;i++)
		{
			data.append(usval >> (((m_elementSize-1)-i)*8));
		}
		emit saveSingleData(m_locationId,data,m_offset,m_elementSize);
	}
	else if (m_type == ENUM)
	{
		unsigned int ival = value.toInt();
		unsigned int prev = ((unsigned char)m_data.at(0));
		qDebug() << "Old val:" << prev << "New Val:" << ival;
		//unsigned int newval = (ival << m_enumStart) & prev;
		for (int i=m_enumStart;i<=m_enumEnd;i++)
		{
			if (ival & (1 << i))
			{
				prev |= (1 << i);
			}
			else
			{
				prev &= ~(1 << i);
			}
		}
		qDebug() << "NewNew:" << prev;
		m_value = prev;
		emit saveSingleData(m_locationId,QByteArray().append(m_value.toInt()),m_offset,1);

	}
}
double TSBPConfigData::calcAxis(unsigned short val,QList<QPair<QString,double> > metadata)
{
	if (metadata.size() == 0)
	{
		return val;
	}
	double newval = val;
	for (int j=0;j<metadata.size();j++)
	{
		if (metadata[j].first == "add")
		{
			newval += metadata[j].second;
		}
		else if (metadata[j].first == "sub")
		{
			newval -= metadata[j].second;
		}
		else if (metadata[j].first == "mult")
		{
			newval *= metadata[j].second;
		}
		else if (metadata[j].first == "div")
		{
			newval /= metadata[j].second;
		}
	}
	return newval;
}

unsigned short TSBPConfigData::reverseCalcAxis(double val,QList<QPair<QString,double> > metadata)
{
	if (metadata.size() == 0)
	{
	    return val;
	}
	double newval = val;
	for (int j=metadata.size()-1;j>=0;j--)
	{
	    if (metadata[j].first == "add")
	    {
		newval -= metadata[j].second;
	    }
	    else if (metadata[j].first == "sub")
	    {
		newval += metadata[j].second;
	    }
	    else if (metadata[j].first == "mult")
	    {
		newval /= metadata[j].second;
	    }
	    else if (metadata[j].first == "div")
	    {
		newval *= metadata[j].second;
	    }
	}
	return qRound(newval);
}
