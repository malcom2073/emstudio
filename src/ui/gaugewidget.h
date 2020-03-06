/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef GAUGEWIDGET_H
#define GAUGEWIDGET_H
#include <QtQuick/QQuickView>
#include <QtQml/QQmlPropertyMap>

#include "gaugeitem.h"
class GaugeWidget : public QQuickView
{
public:
	GaugeWidget(QQuickView *parent=0);
	QQmlPropertyMap propertyMap;
	QString setFile(QString file);
	const QList<QString> getPropertiesInUse() { return propertylist; }
private:
	QList<QString> propertylist;
};

#endif // GAUGEWIDGET_H
