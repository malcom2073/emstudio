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
        ENUM,
        FLOAT
    };
    enum ElementType
    {
        UNSIGNED_ELEMENT,
        SIGNED_ELEMENT,
        FLOAT_ELEMENT
    };

    static inline QVariant convert(QByteArray data,int size,bool issigned)
    {
        if (issigned)
        {
            if (size == 1)
            {
                int8_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
            else if (size == 2)
            {
                int16_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
            else if (size == 4)
            {
                int32_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
        }
        else
        {
            if (size == 1)
            {
                uint8_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
            else if (size == 2)
            {
                uint16_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
            else if (size == 4)
            {
                uint32_t val = 0;
                for (int i=0;i<size;i++)
                {
                    val += ((unsigned char)data[i]) << ((i) * 8);
                }
                return val;
            }
        }

    }
    virtual void setData(QByteArray data)=0;
    virtual QString name()=0;
    virtual Type type()=0;
    virtual ElementType elementType()=0;
    virtual void saveToFlash()=0;
    inline double calcAxis(float val,QList<QPair<QString,double> > & metadata)
    {
        if (metadata.size() == 0)
        {
            return val;
        }
        double newval = val;
        for (int j=0;j<metadata.size();j++)
        {
            if (metadata[j].first == "add")
            {
                newval += metadata[j].second;
            }
            else if (metadata[j].first == "sub")
            {
                newval -= metadata[j].second;
            }
            else if (metadata[j].first == "mult")
            {
                newval *= metadata[j].second;
            }
            else if (metadata[j].first == "div")
            {
                newval /= metadata[j].second;
            }
        }
        return newval;
    }

    inline unsigned short reverseCalcAxis(double val,QList<QPair<QString,double> > & metadata)
    {
        if (metadata.size() == 0)
        {
            return val;
        }
        double newval = val;
        for (int j=metadata.size()-1;j>=0;j--)
        {
            if (metadata[j].first == "add")
            {
            newval -= metadata[j].second;
            }
            else if (metadata[j].first == "sub")
            {
            newval += metadata[j].second;
            }
            else if (metadata[j].first == "mult")
            {
            newval /= metadata[j].second;
            }
            else if (metadata[j].first == "div")
            {
            newval *= metadata[j].second;
            }
        }
        return qRound(newval);
    }

signals:
    void update();
};

#endif // CONFIGDATA_H
