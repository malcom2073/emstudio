/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef SUBMENUITEM_H
#define SUBMENUITEM_H

#include <QString>

class SubMenuItem
{
public:
    QString variable;
    QString title;
    QString parent;
    QString var;
    QString param;
    QString condition;
    bool is_seperator;
};


#endif // SUBMENUITEM_H
