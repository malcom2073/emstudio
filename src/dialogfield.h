/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef DIALOGFIELD_H
#define DIALOGFIELD_H
#include <QString>

class DialogField
{
public:
    QString title; //Display name
    QString variable; //Ram variable it edits
    QString condition; //Condition on it being editable/displayed
};

#endif // DIALOGFIELD_H
