/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>

namespace Ui {
class GraphView;
}
#include <QVariant>
#include <QMap>
class GraphView : public QWidget
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = 0);
    ~GraphView();
    void passData(QList<QVariantMap> *data,QList<QPair<QString, QString> > logtypes);
private:
    QList<QPair<QString,QString> > m_logTypes;
    Ui::GraphView *ui;
    QList<QVariantMap> *m_dataPtr;
private slots:
    void itemEnabled(QString item);
    void itemDisabled(QString item);
};

#endif // GRAPHVIEW_H
