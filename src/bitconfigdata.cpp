/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "bitconfigdata.h"

/*BitConfigData::BitConfigData()
{

}*/
void BitConfigData::addBitField(QString name,unsigned char startbit, unsigned char stopbit,QList<QString> items)
{
    BitFieldItem item;
    item.name = name;
    item.startbit = startbit;
    item.stopbit = stopbit;
    item.itemlist = items;
    m_bitFieldMap.insert(name,item);
}
void BitConfigData::setData(QByteArray data)
{

    QByteArray mydata = data.mid(m_offset,m_elementSize);
    m_origBytes = mydata;
    for (QMap<QString,BitFieldItem>::const_iterator i=m_bitFieldMap.constBegin(); i!=m_bitFieldMap.constEnd();i++)
    {
        unsigned int buf = 0;
        for (int b=i.value().startbit;b<=i.value().stopbit;b++)
        {
            buf += (1 << b);
        }
        unsigned int mydataint = ConfigData::BytesToValue(mydata,m_elementSize,false).toUInt();
        mydataint &= buf;
        mydataint = mydataint >> i.value().startbit;
        m_valueMap[i.key()] = mydataint;
    }
}
void BitConfigData::saveToDevice()
{
    emit saveSignal();
}
void BitConfigData::setValue(QString name,QVariant value)
{
    m_valueMap[name] = value.toUInt();
}
QByteArray BitConfigData::getBytes()
{
    QByteArray retval;
    unsigned int totalval = 0;
    for (QMap<QString,BitFieldItem>::const_iterator i=m_bitFieldMap.constBegin(); i!=m_bitFieldMap.constEnd();i++)
    {
        totalval += (m_valueMap[i.key()].toUInt() << i.value().startbit);
    }
    retval = ValueToBytes(totalval,m_size,false);
    qDebug() << "BitconfigData::getBytes Orig:" << m_origBytes.toHex() << "New:" << retval.toHex();
    return retval;
}
QVariant BitConfigData::getValue(QString name)
{
    return m_bitFieldMap[name].itemlist[m_valueMap[name].toUInt()];
}
void BitConfigData::setElementSize(unsigned short size)
{
    m_elementSize = size;
}
