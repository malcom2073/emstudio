/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef EMSTEST_H
#define EMSTEST_H

#include <QObject>
#include "mspcomms.h"
class EMSTest : public QObject
{
    Q_OBJECT
public:
    explicit EMSTest(QObject *parent = nullptr);
    void startTest();
    bool testSimple2DArray();
    bool testSimple2DWrite();
private:
    MSPComms *m_comms;
    bool m_failure;
signals:
public slots:
    void interrogationCompleted();
};

#endif // EMSTEST_H
