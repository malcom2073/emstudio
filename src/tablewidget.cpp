/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "tablewidget.h"
//#include "QsLog.h"
//
TableWidget::TableWidget(QWidget *parent) : QTableWidget(parent)
{

}
void TableWidget::addHotkey(int key,Qt::KeyboardModifier modifier)
{
    m_hotkeyList.append(QPair<int,Qt::KeyboardModifiers>(key,modifier));
}

void TableWidget::keyPressEvent(QKeyEvent *event)
{
    if (this->selectedItems().size() == 0)
    {
        QTableWidget::keyPressEvent(event);
        return;
    }
    for (int i=0;i<m_hotkeyList.size();i++)
    {
        if (event->modifiers() != Qt::NoModifier)
        {
            if (m_hotkeyList[i].first == event->key() && m_hotkeyList[i].second & event->modifiers())
            {
//                QLOG_DEBUG() << "found event";
                emit hotKeyPressed(event->key(),event->modifiers());
                return;
            }
        }
        else
        {
            if (m_hotkeyList[i].first == event->key())
            {
//                QLOG_DEBUG() << "found event2";
                emit hotKeyPressed(event->key(),event->modifiers());
                return;
            }
        }
    }
    //QLOG_DEBUG() << "Unhandled event:" << event->key() << event->modifiers();
    event->ignore();
    QTableWidget::keyPressEvent(event);
}
