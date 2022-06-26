/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MENUITEM_H
#define MENUITEM_H
#include <QString>
#include <QList>
#include "submenuitem.h"

class MenuItem
{
public:
    QString title;
    QString condition;
    QList<SubMenuItem> subMenuList;
};


#endif // MENUITEM_H
