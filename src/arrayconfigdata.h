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
    void setName(QString name) { m_name = name; }
    Type type() { return m_type; }
    void setType(Type t) { m_type = t; }
    void setElementType(ElementType t) { m_elementType = t; }
    ElementType elementType() { return m_elementType; }
    void saveToDevice();
    void setValue(int index, QVariant value);
    QByteArray getBytes();
    QVariant getValue(int index);
    void setElementSize(unsigned short size);
    void setCalc(QList<QPair<QString,double> >list) {m_calcList = list;}
    unsigned int offset() { return m_offset; }
private:
    QList<QPair<QString,double> > m_calcList;
    QString m_name;
    unsigned short m_elementSize;
    ElementType m_elementType;
    Type m_type;
    QVariantList m_values;
};

#endif // ARRAYCONFIGDATA_H
