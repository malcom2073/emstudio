/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLECONFIGDATA_H
#define TABLECONFIGDATA_H

#include "configdata.h"

class TableConfigData : public ConfigData
{
    Q_OBJECT
public:
    void setData(QByteArray data);
    QString name() { return m_name; }
    int ySize() { return m_ySize; }
    int xSize() { return m_xSize; }
    void setName(QString name) { m_name = name; }
    Type type() { return m_type; }
    void setType(Type t) { m_type = t; }
    void setElementType(ElementType t) { m_elementType = t; }
    void setOffset(unsigned int offset) { m_offset = offset; }
    ElementType elementType() { return m_elementType; }
    void saveToDevice();
    void setValue(unsigned int x,unsigned int y, QVariant value);
    QByteArray getBytes();
    QVariant getValue(unsigned int x, unsigned int y);
    void setElementSize(unsigned short size);
    void setXSize(unsigned short size) { m_xSize = size; m_size = m_xSize*m_ySize; }
    void setYSize(unsigned short size) { m_ySize = size; m_size = m_xSize*m_ySize; }
    void setCalc(QList<QPair<QString,double> >list) {m_calcList = list;}
    QByteArray getOrig() { return m_origBytes; }

private:
    QList<QPair<QString,double> > m_calcList;
    QByteArray m_origBytes;
    QString m_name;
    unsigned short m_elementSize;
    unsigned int m_xSize;
    unsigned int m_ySize;
    ElementType m_elementType;
    Type m_type;
    QList<QVariantList> m_values;
signals:
     void saveSignal();
};

#endif // TABLECONFIGDATA_H
