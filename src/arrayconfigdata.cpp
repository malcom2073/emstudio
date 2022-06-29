/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "arrayconfigdata.h"

/*ArrayConfigData::ArrayConfigData()
{

}*/
void ArrayConfigData::setData(QByteArray data)
{
    if (data.size() < m_offset + (m_elementSize * m_size))
    {
        qDebug() << data.size() << m_offset + (m_elementSize * m_size);
        //Error, data is not value for this config.
        return;
    }
    m_values.clear();
    for (int i=m_offset;i<(m_offset + (m_elementSize * m_size));i+=m_elementSize)
    {
        if (m_elementType == ConfigData::FLOAT_ELEMENT)
        {
            QByteArray valarray = data.mid(i,m_elementSize);
            float val = *reinterpret_cast<float*>(valarray.data());
            for (int c = 0; c< m_calcList.size();c++)
            {
                if (m_calcList.at(c).first == "add")
                {
                    val = val + m_calcList.at(c).second;
                }
                if (m_calcList.at(c).first == "mult")
                {
                    val = val * m_calcList.at(c).second;
                }
            }
                m_values.append(val);
        }
        else
        {
            double val = 0;
            for (int j=0;j<m_elementSize;j++)
            {
                //val += (((unsigned char)data[(i + j)]) << (((m_elementSize-1) - j) * 8));
                val += (((unsigned char)data[(i + j)]) << ((j) * 8));
            }
            for (int c = 0; c< m_calcList.size();c++)
            {
                if (m_calcList.at(c).first == "add")
                {
                    val = val + m_calcList.at(c).second;
                }
                if (m_calcList.at(c).first == "mult")
                {
                    val = val * m_calcList.at(c).second;
                }
            }
            m_values.append(val);
        }
    }
}
void ArrayConfigData::setElementSize(unsigned short size)
{
    m_elementSize = size;
}
QByteArray ArrayConfigData::getBytes()
{
    if (m_size == 0)
    {
        //It's a 3d array
        return QByteArray();
    }
    else
    {
        QByteArray retval;
        QVariantList varlist = m_values.toList();
        for (int i=0;i<varlist.size();i++)
        {
            if (m_elementType == ConfigData::FLOAT_ELEMENT)
            {
                float val = varlist.at(i).toFloat();
                for (int c = 0; c< m_calcList.size();c++)
                {
                    if (m_calcList.at(c).first == "add")
                    {
                        val = val - m_calcList.at(c).second;
                    }
                    if (m_calcList.at(c).first == "mult")
                    {
                        val = val / m_calcList.at(c).second;
                    }
                }
                retval.append(QByteArray::fromRawData(reinterpret_cast<char *>(&val), sizeof(float)));
            }
            else
            {
                float val = varlist.at(i).toFloat();
                for (int c = 0; c< m_calcList.size();c++)
                {
                    if (m_calcList.at(c).first == "add")
                    {
                        val = val - m_calcList.at(c).second;
                    }
                    if (m_calcList.at(c).first == "mult")
                    {
                        val = val / m_calcList.at(c).second;
                    }
                }
                for (int k=0;k<m_elementSize;k++)
                {
                    retval.append(((qRound(val)) >> (((m_elementSize-1) - k) * 8)) & 0xFF);
                }
            }
        }
        return retval;
    }
}
void ArrayConfigData::saveToDevice()
{
    emit saveSignal();
}

void ArrayConfigData::setValue(int index, QVariant value)
{
    QVariantList varlist = m_values.toList();
    varlist[index] = value;
    m_values = varlist;
}
QVariant ArrayConfigData::getValue(int index)
{
    return m_values.at(index);
}
