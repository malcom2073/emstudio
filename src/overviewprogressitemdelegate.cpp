/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "overviewprogressitemdelegate.h"
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QPainterPath>
OverviewProgressItemDelegate::OverviewProgressItemDelegate(QObject *parent) : QItemDelegate(parent)
{
}
void OverviewProgressItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        QStringList valuelist = index.data().toStringList();
        int ivalue = valuelist[0].toInt();
        int imax = valuelist[1].toInt();
        QPen pen = painter->pen();
        QBrush brush =  painter->brush();
        painter->setPen(QColor::fromRgb(100,100,255));
        //painter->fillRect();
        //painter->setBrush(Qt::SolidPattern);
        //QLOG_DEBUG() << "IValue:" << ivalue << "IMax:" << imax;
        if (imax ==0 && ivalue == 0)
        {
        }
        else
        {
            QPainterPath rounded_rect;
            rounded_rect.addRoundedRect(option.rect.x(),option.rect.y() + ((option.rect.height() * 0.4) / 2.0),(((double)ivalue / (double)imax) * option.rect.width())-1,(option.rect.height() * 0.6), 20,20);
            //painter->drawPath(rounded_rect);

            if (imax == ivalue)
            {

                painter->fillPath(rounded_rect,QColor::fromRgb(100,255,100));
                painter->setPen(QColor::fromRgb(0,0,0));
                painter->drawText(option.rect.x() + (option.rect.width() / 2.0) - (painter->fontMetrics().horizontalAdvance("Done")/2.0),option.rect.y()+ (option.rect.height()/2.0) - (painter->fontMetrics().height()/2.0),option.rect.width(),option.rect.height(),0,QString("Done"));
                painter->setPen(QColor::fromRgb(100,255,100));
            }
            else
            {
                painter->fillPath(rounded_rect,QColor::fromRgb(100,100,200));
                painter->setPen(QColor::fromRgb(0,0,0));
                painter->drawText(option.rect.x() + (option.rect.width() / 2.0) - (painter->fontMetrics().horizontalAdvance(QString::number((int)(((double)ivalue / (double)imax) * 100.0)) + "%")/2.0),option.rect.y()+ (option.rect.height()/2.0) - (painter->fontMetrics().height()/2.0),option.rect.width(),option.rect.height(),0,QString::number((int)(((double)ivalue / (double)imax) * 100.0)) + "%");
                painter->setPen(QColor::fromRgb(0,0,255));
            }

        }
        painter->setPen(pen);
        painter->setBrush(brush);
        //painter->restore();
    }
    else
    {
        QItemDelegate::paint(painter,option,index);
    }
}
