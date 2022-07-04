/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "baseconfigdata.h"
#include <QDebug>
void TSBPConfigData::setData(QByteArray data)
{
    m_valid = true;
    if (m_type == FLOAT)
    {
        if (data.size() < m_offset + m_elementSize)
        {
            //Error, data is not value for this config.
            return;
        }
        QByteArray newdata = data.mid(m_offset,m_elementSize);
        quint32 temp = ((char)newdata[3] << 24)|((char)newdata[2] << 16)|((char)newdata[1] << 8)|(char)newdata[0]; // Big endian
        //float* out = reinterpret_cast<float*>(&temp);
        float* out = reinterpret_cast<float*>(newdata.data());
        //qDebug() << "Value for" << m_name << *out << "offset" << m_offset << "size" << m_elementSize;
        //m_value = QVariant(calcAxis(out,m_calc));
        m_value = QVariant(calcAxis(*out,m_calc));
    }
    else if (m_type == VALUE)
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
        //qDebug() << "Value for" << m_name << val << "offset" << m_offset << "size" << m_elementSize;
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
    else if (m_type == ARRAY)
    {
        if (m_size == 0)
        {
            //It's a 3d array
            QList<double> newvals;
            QVariantList m_axis;
            for (unsigned int i=m_offset;i<(m_offset + (m_elementSize * m_xsize * m_ysize));i+=m_elementSize)
            {
                double val = 0;
                if (m_elementType == ConfigData::FLOAT_ELEMENT)
                {
                    QByteArray valarray = data.mid(i,m_elementSize);
                    float outval = *reinterpret_cast<float*>(valarray.data());
                        m_axis.append(outval);
                }
                else
                {
                    // Not currently handled!
                }
            }
            m_value = m_axis;
        }
        else
        {
            if (data.size() < m_offset + (m_elementSize * m_size))
            {
                qDebug() << data.size() << m_offset + (m_elementSize * m_size);
                //Error, data is not value for this config.
                return;
            }
            QVariantList m_axis;
            for (int i=m_offset;i<(m_offset + (m_elementSize * m_size));i+=m_elementSize)
            {
                double val = 0;
                if (m_elementType == ConfigData::FLOAT_ELEMENT)
                {
                    QByteArray valarray = data.mid(i,m_elementSize);
                    float outval = *reinterpret_cast<float*>(valarray.data());
                        m_axis.append(outval);
                }
            }
            m_value = m_axis;

        }
    }
    emit update();
}
QByteArray TSBPConfigData::getBytes()
{
    if (m_type == ARRAY)
    {
        if (m_size == 0)
        {
            //It's a 3d array
            return QByteArray();
        }
        else
        {
            QByteArray retval;
            QVariantList varlist = m_value.toList();
            for (int i=0;i<varlist.size();i++)
            {
                float floatval = varlist.at(i).toFloat();
                retval.append(QByteArray::fromRawData(reinterpret_cast<char *>(&floatval), sizeof(float)));
            }
            return retval;
        }
    }
    return QByteArray();
}
double TSBPConfigData::getValue()
{
    return m_value.toDouble();
    return 0.0;
}
double TSBPConfigData::getValue(unsigned short index)
{
    QList<QVariant> vallist = m_value.toList();
    return vallist.at(index).toFloat();
    return 0.0;
}
double TSBPConfigData::getValue(unsigned short x,unsigned short y)
{
    QList<QVariant> vallist = m_value.toList();
    return vallist.at((y*m_xsize) + x).toFloat();
    return 0.0;
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

bool TSBPConfigData::setValue(int index,QVariant value)
{
    if (m_type != ARRAY)
    {
        return false;
    }
    QVariantList varlist = m_value.toList();
    varlist[index] = value;
    m_value = varlist;
    return true;
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
double TSBPConfigData::calcAxis(float val,QList<QPair<QString,double> > metadata)
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
