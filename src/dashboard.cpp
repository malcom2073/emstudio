/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "dashboard.h"
//#include "QsLog.h"
#include <QMetaType>
#include <QFile>
#include <QDir>
#include <QtQml/QQmlContext>
Dashboard::Dashboard(QQuickWidget *parent) : QQuickWidget(parent)
{
    qmlRegisterType<GaugeItem>("GaugeImage",0,1,"GaugeImage");
    this->rootContext()->setContextProperty("propertyMap",&propertyMap);
    /*if (QFile::exists("gauges.qml"))
    {
        setSource(QUrl("gauges.qml"));
    }
    else if (QFile::exists("src/gauges.qml"))
    {
        setSource(QUrl("src/gauges.qml"));
    }
    else if (QFile::exists("/etc/emstudio/gauges.qml"))
    {
        setSource(QUrl("/etc/emstudio/gauges.qml"));
    }
    else
    {
    }*/
}

QString Dashboard::setFile(QString file)
{
    //QLOG_DEBUG() << "GaugeWidget loading " << file;
    setSource(QUrl::fromLocalFile(file));
    qDebug() << this->errors();
    if (rootObject())
    {
        for (int i=0;i<rootObject()->childItems().size();i++)
        {
            QQuickItem *obj = qobject_cast<QQuickItem*>(rootObject()->childItems()[i]);
            if (obj)
            {
                propertylist.append(obj->property("propertyMapProperty").toString());
            }
        }
        if (rootObject()->property("plugincompat").isValid())
        {
            QString plugincompat = rootObject()->property("plugincompat").toString();
            //QLOG_DEBUG() << "Plugin compatability:" << plugincompat;
            return plugincompat;
        }
    }
    return "";
}
