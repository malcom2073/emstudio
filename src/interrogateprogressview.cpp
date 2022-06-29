/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "interrogateprogressview.h"
#include <QMdiSubWindow>

InterrogateProgressView::InterrogateProgressView(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelClickedSlot()));
    reset();
    ui.outputTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.overviewTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void InterrogateProgressView::closeEvent(QCloseEvent *event)
{
    event->ignore();
    ((QMdiSubWindow*)this->parent())->hide();
    emit windowHiding((QMdiSubWindow*)this->parent());
}
void InterrogateProgressView::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    emit windowHiding((QMdiSubWindow*)this->parent());
}
void InterrogateProgressView::reset()
{
    ui.cancelPushButton->setEnabled(true);
    ui.outputTableWidget->clearContents();
    ui.outputTableWidget->setRowCount(0);

    ui.outputTableWidget->setColumnCount(3);
    ui.outputTableWidget->setColumnWidth(0,50);
    ui.outputTableWidget->setColumnWidth(1,300);
    ui.outputTableWidget->setColumnWidth(2,100);


    ui.overviewTableWidget->clear();

    ui.overviewTableWidget->setColumnCount(2);
    ui.overviewTableWidget->setColumnWidth(0,300);
    ui.overviewTableWidget->setColumnWidth(1,100);
    ui.overviewTableWidget->setRowCount(3);
    ui.overviewTableWidget->setItem(0,0,new QTableWidgetItem("ECU Build Information"));
    ui.overviewTableWidget->setItem(0,1,new QTableWidgetItem("0"));
    ui.overviewTableWidget->item(0,1)->setData(Qt::DisplayRole,QStringList() << "0" << "0");

    ui.overviewTableWidget->setItem(1,0,new QTableWidgetItem("Location ID Info"));
    ui.overviewTableWidget->setItem(1,1,new QTableWidgetItem("0"));
    ui.overviewTableWidget->item(1,1)->setData(Qt::DisplayRole,QStringList() << "0" << "0");

    ui.overviewTableWidget->setItem(2,0,new QTableWidgetItem("Location ID Contents"));
    ui.overviewTableWidget->setItem(2,1,new QTableWidgetItem("0"));
    ui.overviewTableWidget->item(2,1)->setData(Qt::DisplayRole,QStringList() << "0" << "0");
    ui.overviewTableWidget->setItemDelegate(new OverviewProgressItemDelegate());
}

InterrogateProgressView::~InterrogateProgressView()
{
}
void InterrogateProgressView::addOutput(QString output)
{
    ui.outputTableWidget->setRowCount(ui.outputTableWidget->rowCount()+1);
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,0,new QTableWidgetItem());
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,1,new QTableWidgetItem(output));
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,2,new QTableWidgetItem());
    ui.outputTableWidget->scrollToBottom();
}
void InterrogateProgressView::addTask(QString task, int sequencenumber,int type)
{
    if (type == 0)
    {
        //ECU Build Info
        //ui.overviewTableWidget->item(0,1)->setText(QString::number(ui.overviewTableWidget->item(0,1)->text().toInt()+1));
        int first = ui.overviewTableWidget->item(0,1)->data(Qt::DisplayRole).toStringList()[0].toInt();
        int second = ui.overviewTableWidget->item(0,1)->data(Qt::DisplayRole).toStringList()[1].toInt();
        ui.overviewTableWidget->item(0,1)->setData(Qt::DisplayRole,QStringList() << QString::number(first) << QString::number(second+1));
        m_typeToOverviewListMap[sequencenumber] = 0;
    }
    else if (type == 1)
    {
        //Location ID Info
        //ui.overviewTableWidget->item(1,1)->setText(QString::number(ui.overviewTableWidget->item(1,1)->text().toInt()+1));
        int first = ui.overviewTableWidget->item(1,1)->data(Qt::DisplayRole).toStringList()[0].toInt();
        int second = ui.overviewTableWidget->item(1,1)->data(Qt::DisplayRole).toStringList()[1].toInt();
        ui.overviewTableWidget->item(1,1)->setData(Qt::DisplayRole,QStringList() << QString::number(first) << QString::number(second+1));
        m_typeToOverviewListMap[sequencenumber] = 1;
    }
    else if (type == 2)
    {
        //Location ID contents
        //ui.overviewTableWidget->item(2,1)->setText(QString::number(ui.overviewTableWidget->item(2,1)->text().toInt()+1));
        int first = ui.overviewTableWidget->item(2,1)->data(Qt::DisplayRole).toStringList()[0].toInt();
        int second = ui.overviewTableWidget->item(2,1)->data(Qt::DisplayRole).toStringList()[1].toInt();
        ui.overviewTableWidget->item(2,1)->setData(Qt::DisplayRole,QStringList() << QString::number(first) << QString::number(second+1));
        m_typeToOverviewListMap[sequencenumber] = 2;
    }
    //ui.outputTextBrowser->append(QString::number(sequencenumber) + ": " + task + "...");
    ui.outputTableWidget->setRowCount(ui.outputTableWidget->rowCount()+1);
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(sequencenumber)));
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,1,new QTableWidgetItem(task));
    ui.outputTableWidget->setItem(ui.outputTableWidget->rowCount()-1,2,new QTableWidgetItem("In progress"));
}

void InterrogateProgressView::taskFail(int sequencenumber)
{
    if (m_typeToOverviewListMap.contains(sequencenumber))
    {
        int first = ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->data(Qt::DisplayRole).toStringList()[0].toInt();
        int second = ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->data(Qt::DisplayRole).toStringList()[1].toInt();
        ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->setData(Qt::DisplayRole,QStringList() << QString::number(first+1) << QString::number(second));
        //ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->setText(QString::number(ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->text().toInt()-1));
    }
    for (int i=0;i<ui.outputTableWidget->rowCount();i++)
    {
        if (ui.outputTableWidget->item(i,0)->text() == QString::number(sequencenumber))
        {
            ui.outputTableWidget->item(i,2)->setText("Failed");
            ui.outputTableWidget->item(i,2)->setBackground(QColor::fromRgb(255,0,0));
            ui.outputTableWidget->scrollToItem(ui.outputTableWidget->item(i,0));
        }
    }
}

void InterrogateProgressView::taskSucceed(int sequencenumber)
{
    if (m_typeToOverviewListMap.contains(sequencenumber))
    {
        int first = ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->data(Qt::DisplayRole).toStringList()[0].toInt();
        int second = ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->data(Qt::DisplayRole).toStringList()[1].toInt();
        ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->setData(Qt::DisplayRole,QStringList() << QString::number(first+1) << QString::number(second));
        //ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->setText(QString::number(ui.overviewTableWidget->item(m_typeToOverviewListMap[sequencenumber],1)->text().toInt()-1));
    }
    for (int i=0;i<ui.outputTableWidget->rowCount();i++)
    {
        if (ui.outputTableWidget->item(i,0)->text() == QString::number(sequencenumber))
        {
            ui.outputTableWidget->item(i,2)->setText("Ok");
            ui.outputTableWidget->item(i,2)->setBackground(QColor::fromRgb(0,255,0));
            ui.outputTableWidget->scrollToItem(ui.outputTableWidget->item(i,0));
        }
    }
}
void InterrogateProgressView::done()
{
    ui.progressBar->setValue(ui.progressBar->maximum());
    ui.cancelPushButton->setEnabled(false);
}

void InterrogateProgressView::setProgress(int progress)
{
    ui.progressBar->setValue(progress);
}

void InterrogateProgressView::setMaximum(int maximum)
{
    ui.progressBar->setMaximum(maximum);
}

void InterrogateProgressView::cancelClickedSlot()
{
    emit cancelClicked();
}
