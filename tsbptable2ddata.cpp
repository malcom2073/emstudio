/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "tsbptable2ddata.h"

TSBPTable2DData::TSBPTable2DData() :
    QObject()
{
    m_validAxisMetaData = false;
    m_validValueMetaData = false;
}
void TSBPTable2DData::setAxis(QByteArray data)
{
    if (!m_validAxisMetaData)
    {
        return;
    }
    axisdata = data;
    m_axis.clear();
    for (int i=0;i<(m_axisSize * m_axisElementSize);i+=m_axisElementSize)
    {
        double val = 0;
        if (!m_axisIsFloat)
        {
            for (int j=0;j<m_axisElementSize;j++)
            {
                val += (((unsigned char)data[(m_axisOffset + i + j)]) << (((m_axisElementSize-1) - j) * 8));
            }
        }
        else
        {
            val = reinterpret_cast<float&>(*(data.data()+m_axisOffset + i));
        }
        val *= m_axisScale;
        val += m_axisTranslate;
        m_axis.append(val);
    }
}
void TSBPTable2DData::setValues(QByteArray data)
{
    if (!m_validValueMetaData)
    {
        return;
    }
    valuedata = data;
    m_values.clear();
    for (int i=0;i<(m_valueSize * m_valueElementSize);i+=m_valueElementSize)
    {
        double val = 0;
        if (!m_valueIsFloat)
        {
            for (int j=0;j<m_valueElementSize;j++)
            {
                val += (((unsigned char)data[m_valueOffset + i + j]) << (((m_axisElementSize-1)-j) * 8));
            }
        }
        else
        {
            val = reinterpret_cast<float&>(*(data.data()+m_valueOffset + i));
        }
        val *= m_valueScale;
        val += m_valueTranslate;
        m_values.append(val);
    }
}
void TSBPTable2DData::setAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat)
{
    m_axisOffset = offset;
    m_axisSize = totalsize;
    m_axisElementSize = elementsize;
    m_axisScale = scale;
    m_axisTranslate = translate;
    m_axisSigned = issigned;
    m_axisPage = pagenum;
    m_axisIsFloat = isfloat;
    m_validAxisMetaData = true;
}

void TSBPTable2DData::setValueMetaData(int pagenum,unsigned int offset, unsigned int totalsize, int elementsize,float scale,float translate,bool issigned,bool isfloat)
{
    m_valueOffset = offset;
    m_valueSize = totalsize;
    m_valueElementSize = elementsize;
    m_valueScale = scale;
    m_valueTranslate = translate;
    m_valueSigned = issigned;
    m_valuePage = pagenum;
    m_valueIsFloat = isfloat;
    m_validValueMetaData = true;
}

QList<double> TSBPTable2DData::axis()
{
    return m_axis;
}

QList<double> TSBPTable2DData::values()
{
    return m_values;
}
void TSBPTable2DData::saveRamToFlash()
{
    emit burnPage(m_axisPage);
    //If for some reason, the values aren't on the same page as the axis,
    //make sure to burn them too.
    if (m_axisPage != m_valuePage)
    {
        emit burnPage(m_valuePage);
    }
}

void TSBPTable2DData::updateFromFlash()
{

}

void TSBPTable2DData::updateFromRam()
{

}
void TSBPTable2DData::setData(unsigned short locationid,bool isflashonly,QByteArray payload,Table2DMetaData metadata,bool signedData)
{

}
void TSBPTable2DData::setData(unsigned short locationid,bool isflashonly,QByteArray payload)
{
    setAxis(payload);
    setValues(payload);
}
QByteArray TSBPTable2DData::data()
{
    return QByteArray();
}

double TSBPTable2DData::maxActualXAxis()
{
    return 65535;
}

double TSBPTable2DData::maxActualYAxis()
{
    return 65535;
}

double TSBPTable2DData::minActualXAxis()
{
    return 0;
}

double TSBPTable2DData::minActualYAxis()
{
    return 0;
}

double TSBPTable2DData::maxCalcedXAxis()
{
    return 65535;
}

double TSBPTable2DData::maxCalcedYAxis()
{
    return 65535;
}

double TSBPTable2DData::minCalcedXAxis()
{
    return 0;
}

double TSBPTable2DData::minCalcedYAxis()
{
    return 0;
}

void TSBPTable2DData::setCell(int row, int column,double newval)
{
    if (row == 0)
    {
        m_axis[column] = newval;
        int axisindex = 0;
        QByteArray valbytes;
        for (int i=0;i<(m_axisSize * m_axisElementSize);i+=m_axisElementSize)
        {
            double val = m_axis[axisindex++];
            val -= m_axisTranslate;
            val /= m_axisScale;
            for (int j=0;j<m_axisElementSize;j++)
            {
                valbytes.append(((qRound(val)) >> (((m_axisElementSize-1) - j) * 8)) & 0xFF);
                //val += (((unsigned char)data[(m_axisOffset + i + j)]) << (((m_axisElementSize-1) - j) * 8));
            }
        }
        emit saveSingleData(m_axisPage,valbytes,m_axisOffset,m_axisSize * m_axisElementSize);
    }
    else if (row == 1)
    {
        //It's a value update
        m_values[column] = newval;
        int valueindex = 0;
        QByteArray valbytes;
        for (int i=0;i<(m_valueSize * m_valueElementSize);i+=m_valueElementSize)
        {
            double val = m_values[valueindex++];
            val -= m_valueTranslate;
            val /= m_valueScale;
            for (int j=0;j<m_valueElementSize;j++)
            {
                valbytes.append(((qRound(val)) >> (((m_valueElementSize-1) - j) * 8)) & 0xFF);
                //val += (((unsigned char)data[(m_axisOffset + i + j)]) << (((m_axisElementSize-1) - j) * 8));
            }
        }
        emit saveSingleData(m_valuePage,valbytes,m_valueOffset,m_valueSize * m_valueElementSize);
    }

}

void TSBPTable2DData::setWritesEnabled(bool enabled)
{

}

void TSBPTable2DData::writeWholeLocation(bool ram)
{

}

bool TSBPTable2DData::isRam()
{
    return false;
}
