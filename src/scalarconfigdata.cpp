#include "scalarconfigdata.h"

void ScalarConfigData::setData(QByteArray data)
{
    if (data.size() < m_offset + m_elementSize)
    {
        //Error, data is not value for this config.
        return;
    }
    QByteArray newdata = data.mid(m_offset,m_elementSize);
    m_origBytes = newdata;
    if (m_elementType == ConfigData::FLOAT_ELEMENT)
    {
        float outval = *reinterpret_cast<float*>(newdata.data());
        m_value = QVariant(calcAxis(outval,m_calcList));
    }
    else
    {
        if (m_elementType == ConfigData::SIGNED_ELEMENT)
        {
            m_value = calcAxis(ConfigData::convert(newdata,m_elementSize,true).toFloat(),m_calcList);
        }
        else
        {
            m_value = calcAxis(ConfigData::convert(newdata,m_elementSize,false).toFloat(),m_calcList);
        }
        qDebug() << "Value for" << m_name << "offset" << m_offset << "size" << m_elementSize << m_value.toFloat();
    }
}

void ScalarConfigData::saveToFlash()
{

}

void ScalarConfigData::setValue(QVariant value)
{

}

QByteArray ScalarConfigData::getBytes()
{
    return QByteArray();
}

QVariant ScalarConfigData::getValue()
{
    return m_value;
    return QVariant();
}

void ScalarConfigData::setElementSize(unsigned short size)
{
    m_elementSize = size;
}

void ScalarConfigData::setSize(unsigned short size)
{
    m_size = size;
}
