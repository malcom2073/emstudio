/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MEMORYLOCATIONINFO_H
#define MEMORYLOCATIONINFO_H

#include <QString>
#include <QList>
#include <QPair>
#include "datatype.h"
#include "formattype.h"
#include "tabledata.h"


class MemoryLocationInfo
{
public:
    unsigned short locationid;
    unsigned short rawflags;
    QList<unsigned short> flags;
    QList<QPair<QString,QString> > propertymap;
    bool isRam;
    bool isFlash;
    bool hasParent;
    bool isReadOnly;
    DataType type;
    FormatType formatType;
    unsigned short parent;
    unsigned char rampage;
    unsigned char flashpage;
    unsigned short ramaddress;
    unsigned short flashaddress;
    unsigned short size;
    unsigned short descid;
    TableMeta metaData;
    FieldMeta xAxis;
    FieldMeta yAxis;
    FieldMeta zAxis;
};

#endif // MEMORYLOCATIONINFO_H
