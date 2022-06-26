/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef READONLYRAMDATA_H
#define READONLYRAMDATA_H

#include <QString>
#include <QList>
#include <QPair>

class ReadOnlyRamData
{
public:
    QString dataTitle;
    QString dataDescription;
    unsigned short locationId;
    unsigned char offset;
    unsigned char size;
    QList<QPair<QString,double> > calc;
};

#endif // READONLYRAMDATA_H
