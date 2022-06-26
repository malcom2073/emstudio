/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLEDATA_H
#define TABLEDATA_H
#include <QObject>
#include <QList>
#include <QPair>
class DataBlock : public QObject
{
public:
    //virtual double calcAxis(int val,QList<QPair<QString,double> > metadata)=0;
    //virtual int backConvertAxis(double val,QList<QPair<QString,double> > metadata)=0;
};



class TableMeta
{
public:
    int id;
    int locationId;
    QString name;
    QString desc;
    int formatId;
    int xAxisId;
    int yAxisId;
    int zAxisId;
    int size;
    bool valid;
};

class FieldMeta
{
public:
    int id;
    int size;
    bool isSigned;
    QString name;
    QString desc;
    double multiplier;
    double adder;
    QString flags;
    QString suffix;
    bool valid;
};

#endif // TABLEDATA_H
