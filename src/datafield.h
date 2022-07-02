/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef DATAFIELD_H
#define DATAFIELD_H
#include <QString>
class DataField
{
public:
    DataField();
    DataField(QString shortname,QString description,int offset,int size,double div,double addoffset=0,double min=0,double max=0,bool issigned=false,bool isfloat=false,bool isFlags=false,int bit=0);
    float getValue(QByteArray *payload,bool translatebeforescale = false);
    QString description() { return m_description; }
    QString name() { return m_name; }
    bool isFlag() { return m_isFlags; }
    bool flagValue(QByteArray *payload);
    int offset() { return m_offset; }
    int size() { return m_size; }
private:
    QString m_name;
    QString m_description;
    bool m_isFlags;
    bool m_isSigned;
    bool m_isFloat;
    int m_bit;
    int m_offset;
    int m_size;
    float m_div;
    float m_addoffset;
    float m_min;
    float m_max;
};

#endif // DATAFIELD_H
