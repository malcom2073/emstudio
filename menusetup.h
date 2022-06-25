/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MENUSETUP_H
#define MENUSETUP_H

#include <QList>
#include "menuitem.h"
#include "dialogitem.h"

class MenuSetup
{
public:
    QList<MenuItem> menulist;
    //QList<QPair<QString,QList<QString> > > menulist;
    //QMap<QString,QString> menunamemap;
    //QMap<QString,QList<QPair<QString,QString> > > dialogmap;
    QList<DialogItem> dialoglist;
};


#endif // MENUSETUP_H
