/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef LOOKUPMETADATA_H
#define LOOKUPMETADATA_H

#include <QString>

class LookupMetaData
{
public:
    QString title;
    unsigned short locationid;
    bool editable;
};

#endif // LOOKUPMETADATA_H
