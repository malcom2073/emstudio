/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QStringList>
class ConfigData : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        ARRAY,
        VALUE,
        ENUM
    };

    virtual void setData(QByteArray data)=0;
    virtual QString name()=0;
    virtual Q_INVOKABLE QVariant value()=0;
    virtual Q_INVOKABLE void setValue(QVariant value)=0;
    virtual Q_INVOKABLE int elements()=0;
    virtual Type type()=0;
    virtual QStringList getEnumValues()=0;
    virtual void setEnumValues(QStringList values)=0;
    virtual void setEnumBits(int min,int max)=0;
    virtual int getMinEnumBit()=0;
    virtual int getMaxEnumBit()=0;
    virtual void saveToFlash()=0;
signals:
    void update();
};

#endif // CONFIGDATA_H
