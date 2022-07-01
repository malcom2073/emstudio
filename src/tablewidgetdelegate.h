/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLEWIDGETDELEGATE_H
#define TABLEWIDGETDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class TableWidgetDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TableWidgetDelegate(QObject *parent = 0);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // TABLEWIDGETDELEGATE_H
