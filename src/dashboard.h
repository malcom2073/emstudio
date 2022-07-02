/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef GAUGEWIDGET_H
#define GAUGEWIDGET_H
#include <QtQuick/QQuickView>
#include <QQuickWidget>
#include <QtQml/QQmlPropertyMap>

#include "gaugeitem.h"
class Dashboard : public QQuickWidget
{
public:
    Dashboard(QQuickWidget *parent=0);
    QQmlPropertyMap propertyMap;
    QString setFile(QString file);
    const QList<QString> getPropertiesInUse() { return propertylist; }
private:
    QList<QString> propertylist;
};

#endif // GAUGEWIDGET_H
