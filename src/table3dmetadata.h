/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLE3DMETADATA_H
#define TABLE3DMETADATA_H

#include <QString>
#include <QList>
#include <QPair>

class Table3DMetaData
{
public:
    Table3DMetaData() { xDp=2; yDp=2; zDp=2; valid=false;}
    QString tableTitle;
    bool valid;
    unsigned int size;
    unsigned short locationId;
    QString xAxisTitle;
    QString yAxisTitle;
    QString zAxisTitle;
    QString xHighlight;
    QString yHighlight;
    int xDp;
    int yDp;
    int zDp;
    QList<QPair<QString,double> > xAxisCalc;
    QList<QPair<QString,double> > yAxisCalc;
    QList<QPair<QString,double> > zAxisCalc;
};

#endif // TABLE3DMETADATA_H
