/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLE2DMETADATA_H
#define TABLE2DMETADATA_H

#include <QString>
#include <QList>
#include <QPair>

class Table2DMetaData
{
public:
    Table2DMetaData() { xDp=2; yDp=2; valid=false;}
    QString tableTitle;
    bool valid;
    unsigned int size;
    unsigned short locationId;
    QString xAxisTitle;
    QString yAxisTitle;
    QString xHighlight;
    int xDp;
    int yDp;
    QList<QPair<QString,double> > xAxisCalc;
    QList<QPair<QString,double> > yAxisCalc;
};

#endif // TABLE2DMETADATA_H
