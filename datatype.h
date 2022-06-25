/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef DATATYPE_H
#define DATATYPE_H

enum DataType
{
    DATA_UNDEFINED=1,
    DATA_TABLE=2,
    DATA_TABLE_LOOKUP=4,
    DATA_CONFIG=5,
};

#endif // DATATYPE_H
