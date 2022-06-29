/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef OVERVIEWPROGRESSITEMDELEGATE_H
#define OVERVIEWPROGRESSITEMDELEGATE_H

#include <QItemDelegate>

class OverviewProgressItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit OverviewProgressItemDelegate(QObject *parent = 0);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // OVERVIEWPROGRESSITEMDELEGATE_H
