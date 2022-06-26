/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "tsbptable3ddata.h"

TSBPTable3DData::TSBPTable3DData() :
    QObject()
{
    m_validXAxisMetadata = false;
    m_validYAxisMetadata = false;
    m_validZAxisMetadata = false;
    m_writesEnabled = true;
}
void TSBPTable3DData::setXAxis(QByteArray data)
{
//	axisdata = data;
    m_xAxis.clear();
    for (unsigned int i=0;i<(m_xAxisSize * m_xAxisElementSize);i+=m_xAxisElementSize)
    {
        double val = 0;
        if (!m_xAxisFloat)
        {
            for (int j=0;j<m_xAxisElementSize;j++)
            {
                val += (((unsigned char)data[(m_xAxisOffset + i + j)]) << (((m_xAxisElementSize-1) - j) * 8));
                //val += (((unsigned char)data[(m_xAxisOffset + i + j)]) << ((j) * 8));
            }
        }
        else
        {
            val = reinterpret_cast<float&>(*(data.data()+m_xAxisOffset + i));
        }
        val *= m_xAxisScale;
        val += m_xAxisTranslate;
        if (i == 0)
        {
            m_maxActualXAxis = val;
            m_minActualXAxis = val;
        }
        else
        {
            if (val > m_maxActualXAxis)
            {
                m_maxActualXAxis = val;
            }
            if (val < m_minActualXAxis)
            {
                m_minActualXAxis = val;
            }
        }
        m_xAxis.append(val);
    }
    emit update();
}

void TSBPTable3DData::setYAxis(QByteArray data)
{
    m_yAxis.clear();
    for (unsigned int i=0;i<(m_yAxisSize * m_yAxisElementSize);i+=m_yAxisElementSize)
    {
        double val = 0;
        if (!m_yAxisFloat)
        {
            for (int j=0;j<m_yAxisElementSize;j++)
            {
                val += (((unsigned char)data[(m_yAxisOffset + i + j)]) << (((m_yAxisElementSize-1) - j) * 8));
            }
        }
        else
        {
            val = reinterpret_cast<float&>(*(data.data()+m_yAxisOffset + i));
        }
        val *= m_yAxisScale;
        val += m_yAxisTranslate;
        if (i == 0)
        {
            m_maxActualYAxis = val;
            m_minActualYAxis = val;
        }
        else
        {
            if (val > m_maxActualYAxis)
            {
                m_maxActualYAxis = val;
            }
            if (val < m_minActualYAxis)
            {
                m_minActualYAxis = val;
            }
        }
        m_yAxis.append(val);
    }
    emit update();
}

void TSBPTable3DData::setValues(QByteArray data)
{
    m_values.clear();
    QList<double> newvals;
    for (unsigned int i=0;i<(m_valueSize * m_valueElementSize);i+=m_valueElementSize)
    {
        double val = 0;
        if (!m_valueFloat)
        {
            for (int j=0;j<m_valueElementSize;j++)
            {
                val += (((unsigned char)data[(m_valueOffset + i + j)]) << (((m_valueElementSize-1) - j) * 8));
            }
        }
        else
        {
            val = reinterpret_cast<float&>(*(data.data()+m_valueOffset + i));
        }
        val *= m_valueScale;
        val += m_valueTranslate;
        if (i == 0)
        {
            m_maxActualValue = val;
            m_minActualValue = val;
        }
        else
        {
            if (val > m_maxActualValue)
            {
                m_maxActualValue = val;
            }
            if (val < m_minActualValue)
            {
                m_minActualValue= val;
            }
        }
        newvals.append(val);
        //m_values.append(val);
        if (newvals.size() >= m_xDim)
        {
            m_values.append(newvals);
            newvals.clear();
        }
    }
    int stopper = 1;
    emit update();
}
void TSBPTable3DData::setXAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,float min, float max)
{
    m_validXAxisMetadata = true;
    m_xAxisPage = pagenum;
    m_xAxisOffset = offset;
    m_xAxisElementSize = elementsize;
    m_xAxisSize = totalsize;
    m_xAxisScale = scale;
    m_xAxisTranslate = translate;
    m_xAxisSigned = issigned;
    m_minCalcedXAxis = min;
    m_maxCalcedXAxis = max;
    m_xAxisFloat = isfloat;
}

void TSBPTable3DData::setYAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,float min, float max)
{
    m_validYAxisMetadata = true;
    m_yAxisPage = pagenum;
    m_yAxisOffset = offset;
    m_yAxisElementSize = elementsize;
    m_yAxisSize = totalsize;
    m_yAxisScale = scale;
    m_yAxisTranslate = translate;
    m_yAxisSigned = issigned;
    m_minCalcedYAxis = min;
    m_maxCalcedYAxis = max;
    m_yAxisFloat = isfloat;
}

void TSBPTable3DData::setValueMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,unsigned short xdim,unsigned short ydim,float min, float max)
{
    m_validZAxisMetadata = true;
    //xdim is your x axis size
    //ydim is your w axis size
    m_valuePage = pagenum;
    m_valueOffset = offset;
    m_valueElementSize = elementsize;
    m_valueSize = totalsize;
    m_valueScale = scale;
    m_valueTranslate = translate;
    m_valueSigned = issigned;
    m_xDim = xdim;
    m_yDim = ydim;
    m_minCalcedValue = min;
    m_maxCalcedValue = max;
    m_valueFloat = isfloat;
}

void TSBPTable3DData::setData(unsigned short locationid,bool isflashonly,QByteArray payload,Table3DMetaData metadata)
{

}

void TSBPTable3DData::setData(unsigned short locationid,bool isflashonly,QByteArray payload)
{
    if (m_validXAxisMetadata)
    {
        setXAxis(payload);
    }
    if (m_validYAxisMetadata)
    {
        setYAxis(payload);
    }
    if (m_validZAxisMetadata)
    {
        setValues(payload);
    }
}

QByteArray TSBPTable3DData::data()
{
    return QByteArray();
}

QList<double> TSBPTable3DData::xAxis()
{
    return m_xAxis;
}

QList<double> TSBPTable3DData::yAxis()
{
    return m_yAxis;
}

QList<QList<double> > TSBPTable3DData::values()
{
    return m_values;
}

int TSBPTable3DData::columns()
{
    return m_xAxis.size();
}

int TSBPTable3DData::rows()
{
    return m_yAxis.size();
}

void TSBPTable3DData::setCell(int row, int column,double val)
{
/*
    m_values.clear();
    QList<double> newvals;
    for (unsigned int i=0;i<(m_valueSize * m_valueElementSize);i+=m_valueElementSize)
    {
        double val = 0;
        for (int j=0;j<m_valueElementSize;j++)
        {
            val += (((unsigned char)data[(m_valueOffset + i + j)]) << (((m_valueElementSize-1) - j) * 8));
        }
        val *= m_valueScale;
        val += m_valueTranslate;
        newvals.append(val);
        //m_values.append(val);
        if (newvals.size() >= m_xDim)
        {
            m_values.append(newvals);
            newvals.clear();
        }
    }
    int stopper = 1;
*/
    m_values[row][column] = val;
    QByteArray result;
    for (int i=0;i<m_values.size();i++)
    {
        for (int j=0;j<m_values[i].size();j++)
        {
            double newval = m_values[i][j];
            newval -= m_valueTranslate;
            newval /= m_valueScale;
            for (int k=0;k<m_valueElementSize;k++)
            {
                result.append(((qRound(newval)) >> (((m_valueElementSize-1) - k) * 8)) & 0xFF);
            }

        }
    }
    //setValues(result);
    if (m_writesEnabled)
    {
        emit saveSingleData(m_valuePage,result,m_valueOffset,m_xDim * m_yDim * m_valueElementSize);
    }
}

void TSBPTable3DData::setXAxis(int column,double val)
{
    m_xAxis[column] = val;
    int axisindex = 0;
    QByteArray valbytes;
    for (int i=0;i<(m_xAxisSize * m_xAxisElementSize);i+=m_xAxisElementSize)
    {
        double newval = m_xAxis[axisindex++];
        newval -= m_xAxisTranslate;
        newval /= m_xAxisScale;
        for (int j=0;j<m_xAxisElementSize;j++)
        {
            valbytes.append(((qRound(newval)) >> (((m_xAxisElementSize-1) - j) * 8)) & 0xFF);
        }
    }
    if (m_writesEnabled)
    {
        emit saveSingleData(m_xAxisPage,valbytes,m_xAxisOffset,m_xAxisSize * m_xAxisElementSize);
    }
}

void TSBPTable3DData::setYAxis(int row,double val)
{
    m_yAxis[row] = val;
    int axisindex = 0;
    QByteArray valbytes;
    for (int i=0;i<(m_yAxisSize * m_yAxisElementSize);i+=m_yAxisElementSize)
    {
        double newval = m_yAxis[axisindex++];
        newval -= m_yAxisTranslate;
        newval /= m_yAxisScale;
        for (int j=0;j<m_yAxisElementSize;j++)
        {
            valbytes.append(((qRound(newval)) >> (((m_yAxisElementSize-1) - j) * 8)) & 0xFF);
        }
    }
    if (m_writesEnabled)
    {
        emit saveSingleData(m_yAxisPage,valbytes,m_yAxisOffset,m_yAxisSize * m_yAxisElementSize);
    }
}

double TSBPTable3DData::maxCalcedXAxis() { return m_maxCalcedXAxis; }
double TSBPTable3DData::maxCalcedYAxis() { return m_maxCalcedYAxis; }
double TSBPTable3DData::maxCalcedValue() { return m_maxCalcedValue; }
double TSBPTable3DData::minCalcedXAxis() { return m_minCalcedXAxis; }
double TSBPTable3DData::minCalcedYAxis() { return m_minCalcedYAxis; }
double TSBPTable3DData::minCalcedValue() { return m_minCalcedValue; }

double TSBPTable3DData::maxActualXAxis() { return m_maxActualXAxis; }
double TSBPTable3DData::maxActualYAxis() { return m_maxActualYAxis; }
double TSBPTable3DData::maxActualValue() { return m_maxActualValue; }
double TSBPTable3DData::minActualXAxis() { return m_minActualXAxis; }
double TSBPTable3DData::minActualYAxis() { return m_minActualYAxis; }
double TSBPTable3DData::minActualValue() { return m_minActualValue; }

void TSBPTable3DData::setWritesEnabled(bool enabled)
{
    m_writesEnabled = enabled;
}

void TSBPTable3DData::writeWholeLocation(bool ram)
{
    int axisindex = 0;
    QByteArray xAxisBytes;
    for (int i=0;i<(m_xAxisSize * m_xAxisElementSize);i+=m_xAxisElementSize)
    {
        float newval = m_xAxis[axisindex++];
        newval -= m_xAxisTranslate;
        newval /= m_xAxisScale;
        if (!m_xAxisFloat)
        {
            for (int j=0;j<m_xAxisElementSize;j++)
            {
                xAxisBytes.append(((qRound(newval)) >> (((m_xAxisElementSize-1) - j) * 8)) & 0xFF);
            }
        }
        else
        {
            char *valbytes = reinterpret_cast<char*>(&newval);
            for (int i=0;i<4;i++)
            {
                xAxisBytes.append(valbytes[i]);
            }
        }
    }
    emit saveSingleData(m_xAxisPage,xAxisBytes,m_xAxisOffset,m_xAxisSize * m_xAxisElementSize);
    axisindex = 0;
    QByteArray yAxisBytes;
    for (int i=0;i<(m_yAxisSize * m_yAxisElementSize);i+=m_yAxisElementSize)
    {
        float newval = m_yAxis[axisindex++];
        newval -= m_yAxisTranslate;
        newval /= m_yAxisScale;
        if (!m_yAxisFloat)
        {
            for (int j=0;j<m_yAxisElementSize;j++)
            {
                yAxisBytes.append(((qRound(newval)) >> (((m_yAxisElementSize-1) - j) * 8)) & 0xFF);
            }
        }
        else
        {
            char *valbytes = reinterpret_cast<char*>(&newval);
            for (int i=0;i<4;i++)
            {
                yAxisBytes.append(valbytes[i]);
            }
        }
    }
    emit saveSingleData(m_yAxisPage,yAxisBytes,m_yAxisOffset,m_yAxisSize * m_yAxisElementSize);
    QByteArray valueBytes;
    for (int i=0;i<m_values.size();i++)
    {
        for (int j=0;j<m_values[i].size();j++)
        {
            float newval = static_cast<float>(m_values[i][j]);
            newval -= m_valueTranslate;
            newval /= m_valueScale;
            if (!m_valueFloat)
            {
                for (int k=0;k<m_valueElementSize;k++)
                {
                    valueBytes.append(((qRound(newval)) >> (((m_valueElementSize-1) - k) * 8)) & 0xFF);
                }
            }
            else
            {
                char *valbytes = reinterpret_cast<char*>(&newval);
                for (int i=0;i<4;i++)
                {
                    valueBytes.append(valbytes[i]);
                }
            }
        }
    }
    //setValues(result);
    emit saveSingleData(m_valuePage,valueBytes,m_valueOffset,m_xDim * m_yDim * m_valueElementSize);
}

bool TSBPTable3DData::isRam()
{
    return false;
}

void TSBPTable3DData::saveRamToFlash()
{
    emit burnPage(m_xAxisPage);
    if (m_yAxisPage != m_xAxisPage)
    {
        emit burnPage(m_yAxisPage);
    }
    if (m_valuePage != m_yAxisPage)
    {
        emit burnPage(m_valuePage);
    }

}

void TSBPTable3DData::updateFromFlash()
{

}

void TSBPTable3DData::updateFromRam()
{

}
