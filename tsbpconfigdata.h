/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TSBPCONFIGDATA_H
#define TSBPCONFIGDATA_H

#include "configdata.h"


class TSBPConfigData : public ConfigData
{
    Q_OBJECT
public:
    TSBPConfigData() { m_valid = false; }
    Type type() { return m_type; }
    ElementType elementType() { return m_elementType; }
    //Parent functions
    void setData(QByteArray data);
    QString name() { return m_name; }
    Q_INVOKABLE QVariant value();
    Q_INVOKABLE void setValue(QVariant value);
    QStringList getEnumValues();
    void setEnumValues(QStringList values);
    void setEnumBits(int min,int max);
    int getMinEnumBit();
    int getMaxEnumBit();
    double getValue(); // For scalars
    double getValue(unsigned short index); // For 2d arrays
    double getValue(unsigned short x,unsigned short y); // For 3d arrays
    //End Parent Functions
    QByteArray getBytes();


    void setMinMax(double min, double max) { m_min = min; m_max = max; }
    void setName(QString name) { m_name = name; }
    void setTypeString(QString type) { m_typeString = type; }
    void setType(Type t) { m_type = t; }
    void setElementType(ElementType t) { m_elementType = t; }
    void setSizeOverride(QString override) { m_sizeOverride = override; }
    void setSizeOverrideMult(double mult) { m_sizeOverrideMult = mult; }
    void setLocationId(unsigned short locationid) { m_locationId = locationid; }
    void setSize(unsigned short size) { m_size = size; m_xsize = 0; m_ysize = 0;}
    void setSize(unsigned short xsize,unsigned short ysize) { m_xsize = xsize; m_ysize = ysize; m_size = 0;}
    void setElementSize(unsigned short size) { m_elementSize = size; }
    void setOffset(unsigned short offset) { m_offset = offset; }
    void setCalc(QList<QPair<QString,double> > calc) { m_calc = calc; }

    QString typeString() { return m_typeString; }
    QString sizeOverride() { return m_sizeOverride; }
    double sizeOverrideMult() { return m_sizeOverrideMult; }
    unsigned short locationId() { return m_locationId; }
    unsigned short offset() { return m_offset; }
    unsigned short size() { return m_size; }
    unsigned short xsize() { return m_xsize; }
    unsigned short ysize() { return m_ysize; }
    unsigned short elementSize() { return m_elementSize; }
    QList<QPair<QString,double> > calc() { return m_calc; }
    double calcAxis(float val,QList<QPair<QString,double> > metadata);
    unsigned short reverseCalcAxis(double val,QList<QPair<QString,double> > metadata);
    Q_INVOKABLE int elements() { return 1; }
    void saveToFlash();
private:
    QVariant m_value;
    QStringList m_enumList;
    QByteArray m_data;
    int m_enumStart;
    int m_enumEnd;
    QString m_name;
    QString m_typeString;
    Type m_type;
    ElementType m_elementType;
    QString m_sizeOverride;
    double m_sizeOverrideMult;
    unsigned short m_xsize;
    double m_min;
    double m_max;
    unsigned short m_ysize;
    unsigned short m_locationId;
    unsigned short m_size;
    unsigned short m_elementSize;
    unsigned short m_offset;
    QList<QPair<QString,double> > m_calc;
    bool m_valid;


signals:
    void update();
    void saveSingleData(unsigned short page, QByteArray bytes,unsigned short offset,unsigned short size);
    void saveSingleDataToFlash(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
    void saveSingleDataToRam(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
    void pageBurnRequested(unsigned int page);

};

#endif // TSBPCONFIGDATA_H
