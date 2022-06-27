/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef SCALARCONFIGDATA_H
#define SCALARCONFIGDATA_H

#include "configdata.h"
class ScalarConfigData : public ConfigData
{
public:
//    ScalarConfigData();
    void setData(QByteArray data);
    QString name() { return m_name; }
    int size() { return m_size; }
    void setName(QString name) { m_name = name; }
    Type type() { return m_type; }
    void setType(Type t) { m_type = t; }
    void setElementType(ElementType t) { m_elementType = t; }
    void setOffset(unsigned int offset) { m_offset = offset; }
    ElementType elementType() { return m_elementType; }
    void saveToFlash();
    void setValue(QVariant value);
    QByteArray getBytes();
    QVariant getValue();
    void setElementSize(unsigned short size);
    void setSize(unsigned short size);
    void setCalc(QList<QPair<QString,double> >list) {m_calcList = list;}
    unsigned int offset() { return m_offset; }
private:
    QByteArray m_origBytes;
    QList<QPair<QString,double> > m_calcList;
    QString m_name;
    unsigned int m_offset;
    unsigned short m_elementSize;
    unsigned int m_size;
    ElementType m_elementType;
    Type m_type;
    QVariant m_value;

};

#endif // SCALARCONFIGDATA_H
