/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef GAUGEUTIL_H
#define GAUGEUTIL_H

class GaugeUtil
{
public:
    static int * getPointInArc(int a, int b, int arcDegrees, int arcStart, int arcRadius, float amount);
    static int * getAmountInArc(float from, float to);
    static double toRadians(double degrees);
};

#endif // GAUGEUTIL_H
