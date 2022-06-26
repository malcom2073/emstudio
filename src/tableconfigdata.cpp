#include "tableconfigdata.h"

//TableConfigData::TableConfigData()
//{

//}
void TableConfigData::setData(QByteArray data)
{
    //It's a 3d array
    m_origBytes = data.mid(m_offset,(m_elementSize * m_xSize * m_ySize));
    QVariantList newvals;
    QVariantList m_axis;
    for (unsigned int i=m_offset;i<(m_offset + (m_elementSize * m_xSize * m_ySize));i+=m_elementSize)
    {
        double val = 0;
        if (m_elementType == ConfigData::FLOAT_ELEMENT)
        {
            QByteArray valarray = data.mid(i,m_elementSize);
            float outval = *reinterpret_cast<float*>(valarray.data());
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
            newvals.append(outval);
            if (newvals.length() >= m_xSize)
            {
                m_values.append(newvals);
                newvals.clear();
            }
        }
        else
        {
            // Not currently handled!
            double val = 0;
            for (int j=0;j<m_elementSize;j++)
            {
                val += (((unsigned char)data[(i + j)]) << (((m_elementSize-1) - j) * 8));
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
            newvals.append(val);
            if (newvals.length() >= m_xSize)
            {
                m_values.append(newvals);
                newvals.clear();
            }
        }
    }
    m_values.append(newvals);
}
void TableConfigData::saveToFlash()
{

}
void TableConfigData::setValue(unsigned int x,unsigned int y, QVariant value)
{

}
QByteArray TableConfigData::getBytes()
{
    QByteArray retval;
    for (int x=0;x<m_values.size();x++)
    {
        for (int y=0;y<m_values.at(x).size();y++)
        {
            if (m_elementType == ConfigData::FLOAT_ELEMENT)
            {
                float floatval = m_values.at(x).at(y).toFloat();
                retval.append(QByteArray::fromRawData(reinterpret_cast<char *>(&floatval), sizeof(float)));
            }
            else
            {
                float floatval = m_values.at(x).at(y).toFloat();
                //newval -= m_valueTranslate;
                //newval /= m_valueScale;
                for (int k=0;k<m_elementSize;k++)
                {
                    retval.append(((qRound(floatval)) >> (((m_elementSize-1) - k) * 8)) & 0xFF);
                }
            }
        }
    }
    return retval;
}
QVariant TableConfigData::getValue(unsigned int x, unsigned int y)
{
    return m_values.at(x).at(y);
}
void TableConfigData::setElementSize(unsigned short size)
{
    m_elementSize = size;
}
/*void TableConfigData::setSize(unsigned short size)
{

}*/
