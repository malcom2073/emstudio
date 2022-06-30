/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef BITCONFIGDATA_H
#define BITCONFIGDATA_H


#include "configdata.h"
class BitConfigData : public ConfigData
{
public:
    class BitFieldItem
    {
    public:
        QString name;
        unsigned char startbit;
        unsigned char stopbit;
        QList<QString> itemlist;
    };
//    BitConfigData();
    void setData(QByteArray data);
    QString name() { return m_name; }
    void setName(QString name) { m_name = name; }
    Type type() { return m_type; }
    void setType(Type t) { m_type = t; }
    void setElementType(ElementType t) { m_elementType = t; }
    ElementType elementType() { return m_elementType; }
    void saveToDevice();
    void setValue(QString name,QVariant value);
    QByteArray getBytes();
    QVariant getValue(QString name);
    void setElementSize(unsigned short size);
    void setCalc(QList<QPair<QString,double> >list) {m_calcList = list;}
    unsigned int offset() { return m_offset; }
    void addBitField(QString name,unsigned char startbit,unsigned char stopbit,QList<QString> items);
    QList<QString> getBitFields(QString name) { return m_bitFieldMap[name].itemlist; }
private:
    QMap<QString,BitFieldItem> m_bitFieldMap;
    QByteArray m_origBytes;
    QList<QPair<QString,double> > m_calcList;
    QString m_name;
    unsigned short m_elementSize;
    ElementType m_elementType;
    Type m_type;
    QMap<QString,QVariant> m_valueMap;


};

#endif // BITCONFIGDATA_H
