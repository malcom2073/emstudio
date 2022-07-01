/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QKeyEvent>
class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = 0);
    void addHotkey(int key,Qt::KeyboardModifier modifier);
private:
    QList<QPair<int,Qt::KeyboardModifiers> > m_hotkeyList;
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void hotKeyPressed(int key,Qt::KeyboardModifiers modifier);
public slots:

};

#endif // TABLEWIDGET_H
