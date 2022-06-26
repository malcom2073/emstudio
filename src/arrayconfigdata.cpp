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
        double val = 0;
        if (m_elementType == ConfigData::FLOAT_ELEMENT)
        {
            QByteArray valarray = data.mid(i,m_elementSize);
            float outval = *reinterpret_cast<float*>(valarray.data());
                m_values.append(outval);
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
            float floatval = varlist.at(i).toFloat();
            retval.append(QByteArray::fromRawData(reinterpret_cast<char *>(&floatval), sizeof(float)));
        }
        return retval;
    }
}
void ArrayConfigData::setSize(unsigned short size)
{
    m_size = size;
}
void ArrayConfigData::setValue(int index, QVariant value)
{
    QVariantList varlist = m_values.toList();
    varlist[index] = value;
    m_values = varlist;
}
void ArrayConfigData::saveToFlash()
{

}
QVariant ArrayConfigData::getValue(int index)
{
    return m_values.at(index);
}
