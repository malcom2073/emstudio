/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef ARRAYCONFIGDATA_H
#define ARRAYCONFIGDATA_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QStringList>
#include "configdata.h"
class ArrayConfigData: public ConfigData
{
    Q_OBJECT
public:
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
    void setValue(int index, QVariant value);
    QByteArray getBytes();
    QVariant getValue(int index);
    void setElementSize(unsigned short size);
    void setSize(unsigned short size);
    void setCalc(QList<QPair<QString,double> >list) {}
    unsigned int offset() { return m_offset; }
private:
    QString m_name;
    unsigned int m_offset;
    unsigned short m_elementSize;
    unsigned int m_size;
    ElementType m_elementType;
    Type m_type;
    QVariantList m_values;
};

#endif // ARRAYCONFIGDATA_H
