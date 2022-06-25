/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef DIALOGITEM_H
#define DIALOGITEM_H

#include "dialogfield.h"
#include <QString>
#include <QList>
#include <QMap>
class DialogItem
{
public:
    QString title; //Display name
    QString variable; //What a menu item points to
    QList<DialogField> fieldList;
    QList<QPair<QString,QString> > panelList;
    QMap<QString,DialogItem> panelMap;
};


#endif // DIALOGITEM_H
