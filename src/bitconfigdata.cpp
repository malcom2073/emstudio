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
    for (QMap<QString,BitFieldItem>::const_iterator i=m_bitFieldMap.constBegin(); i!=m_bitFieldMap.constEnd();i++)
    {
        unsigned int buf = 0;
        for (int b=i.value().startbit;b<=i.value().stopbit;b++)
        {
            buf += (1 << b);
        }
        unsigned int mydataint = ConfigData::convert(mydata,m_elementSize,false).toUInt();
        mydataint &= buf;
        mydataint = mydataint >> i.value().startbit;
        m_valueMap[i.key()] = mydataint;
    }
}
void BitConfigData::saveToFlash()
{

}
void BitConfigData::setValue(QVariant value)
{

}
QByteArray BitConfigData::getBytes()
{
    return QByteArray();
}
QVariant BitConfigData::getValue(QString name)
{
    return m_bitFieldMap[name].itemlist[m_valueMap[name].toUInt()];
}
void BitConfigData::setElementSize(unsigned short size)
{
    m_elementSize = size;
}
void BitConfigData::setSize(unsigned short size)
{

}
