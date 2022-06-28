/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "parameterview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QCloseEvent>
#include "parameterwidget.h"
//#include "QsLog.h"

ParameterView::ParameterView(QWidget *parent) : QWidget(parent)
{
    m_metaData=0;
    //m_emsData = 0;
    ui.setupUi(this);
    connect(ui.parameterTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui.parameterTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(itemSelectionChanged()));
    ui.parameterTreeWidget->setColumnCount(2);
}
void ParameterView::itemSelectionChanged()
{
    return;
}
void ParameterView::addConfig(QString name,ConfigData *data)
{
    Q_UNUSED(name)
    Q_UNUSED(data)

}
void ParameterView::setActiveComms(MSPComms* comms)
{
    m_emsComms = comms;
}

void ParameterView::closeEvent(QCloseEvent *event)
{
    event->ignore();
    ((QMdiSubWindow*)this->parent())->hide();
    emit windowHiding((QMdiSubWindow*)this->parent());
}

void ParameterView::currentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *prev)
{
    Q_UNUSED(prev);
    if (!current)
    {
        return;
    }
    if (current->parent() == 0)
    {
        //It is a top level item. don't do anything
        return;
    }
    if (!m_treeWidgetToVariableNameMap.contains(current))
    {
        qDebug() << "No child widget found for parameterview";
        return;
    }
    QString variablename = m_treeWidgetToVariableNameMap[current];
    //QLOG_DEBUG() << "Variable:" << m_metaMenu.menulist[i].subMenuList[j].variable;
    emit showTable(variablename);
    bool found = false;
    for (int k=0;k<m_metaMenu.dialoglist.size();k++)
    {
        if (m_metaMenu.dialoglist[k].variable == variablename)
        {
            //QLOG_DEBUG() << "Found Dialog:" << m_metaMenu.dialoglist[k].title << m_metaMenu.dialoglist[k].fieldList.size();
            if (m_metaMenu.dialoglist[k].panelList.size() > 0)
            {
                //Has sub-panels, find and populate them.
            }
            found = true;
            generateDialog(m_metaMenu.dialoglist[k]);
            //Generate a dialog here.
        }
    }
    if (!found)
    {
        Table* table = m_emsComms->getTableFromName(variablename);
        if (table)
        {
            ParameterWidget *widget = new ParameterWidget();
            connect(widget,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
            widget->show();
            paramWidgetList.append(widget);
            ArrayConfigData *xdata = m_emsComms->getArrayConfigData(table->xbin);
            ArrayConfigData *ydata = m_emsComms->getArrayConfigData(table->ybin);
            TableConfigData *zdata = m_emsComms->getTableConfigData(table->zbin);
            if (xdata && ydata && zdata)
            {
                widget->addTable(xdata,ydata,zdata);
            }
        }
        else
        {

        }
        //No dialog found, show the raw data type.
    }
    /*for (int i=0;i<m_metaMenu.menulist.size();i++)
    {
        if (m_metaMenu.menulist[i].title == current->parent()->text(0))
        {
            //This is our menu!
            for (int j=0;j<m_metaMenu.menulist[i].subMenuList.size();j++)
            {
                if (m_metaMenu.menulist[i].subMenuList[j].variable == current->text(1))
                {
                    //This is our current item!
                    if (!m_metaMenu.menulist[i].subMenuList[j].is_seperator)
                    {
                        //QLOG_DEBUG() << "Variable:" << m_metaMenu.menulist[i].subMenuList[j].variable;
                        emit showTable(m_metaMenu.menulist[i].subMenuList[j].variable);
                        bool found = false;
                        for (int k=0;k<m_metaMenu.dialoglist.size();k++)
                        {
                            if (m_metaMenu.dialoglist[k].variable == m_metaMenu.menulist[i].subMenuList[j].variable)
                            {
                                //QLOG_DEBUG() << "Found Dialog:" << m_metaMenu.dialoglist[k].title << m_metaMenu.dialoglist[k].fieldList.size();
                                if (m_metaMenu.dialoglist[k].panelList.size() > 0)
                                {
                                    //Has sub-panels, find and populate them.
                                }
                                found = true;
                                generateDialog(m_metaMenu.dialoglist[k]);
                                //Generate a dialog here.
                            }

                        }
                        if (!found)
                        {
                            QString variablename = m_metaMenu.menulist[i].subMenuList[j].variable;
                            Table* table = m_emsComms->getTableFromName(variablename);
                            if (table)
                            {
                                ParameterWidget *widget = new ParameterWidget();
                                connect(widget,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
                                widget->show();
                                paramWidgetList.append(widget);
                                ArrayConfigData *xdata = m_emsComms->getArrayConfigData(table->xbin);
                                ArrayConfigData *ydata = m_emsComms->getArrayConfigData(table->ybin);
                                TableConfigData *zdata = m_emsComms->getTableConfigData(table->zbin);
                                if (xdata && ydata && zdata)
                                {
                                    widget->addTable(xdata,ydata,zdata);
                                }
                            }
                            else
                            {

                            }
                            //No dialog found, show the raw data type.
                        }
                    }
                }
            }
        }
    }*/
}

void ParameterView::generateDialog(DialogItem item,QString dir,ParameterWidget *parent)
{
    //QString title,QList<DialogField> fieldlist
    if (item.panelList.size() > 0)
    {
        int stopper = 1;
    }
    QString title = item.title;
    QList<DialogField> fieldlist = item.fieldList;
    ParameterWidget *widget;
    if (parent)
    {
        widget = new ParameterWidget(parent);
        widget->disableSaveButton();
        parent->addPanel(widget,dir);
    }
    else
    {
        widget = new ParameterWidget();
    }
    widget->setWindowTitle(item.title);
    connect(widget,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
    widget->show();
    for (QMap<QString,DialogItem>::const_iterator i=item.panelMap.constBegin();i!=item.panelMap.constEnd();i++)
    {
        //generateDialog(i.value(),widget);
    }
    if (item.panelList.size() > 0)
    {
        //We have subpanels! Create them
        for (int i=0;i<item.panelList.size();i++)
        {
            for (int j=0;j<this->m_metaMenu.dialoglist.size();j++)
            {
                if (this->m_metaMenu.dialoglist.at(j).variable == item.panelList.at(i).second)
                {
                    if (item.panelList.at(i).first == "East")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"East",widget);
                    }
                    else if (item.panelList.at(i).first == "Center")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"Center",widget);
                    }
                    else if (item.panelList.at(i).first == "West")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"West",widget);
                    }
                    else
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"",widget);
                    }
                }
            }
        }
        //generateDialog()
    }
    paramWidgetList.append(widget);
    for (int i=0;i<item.panelList.size();i++)
    {
        Table* table = m_emsComms->getTableFromName(item.panelList[i].second);
        if (table)
        {
            widget->setWindowTitle(table->title);
            ArrayConfigData *xdata = m_emsComms->getArrayConfigData(table->xbin);
            ArrayConfigData *ydata = m_emsComms->getArrayConfigData(table->ybin);
            TableConfigData *zdata = m_emsComms->getTableConfigData(table->zbin);
            if (xdata && ydata && zdata)
            {
                widget->addTable(xdata,ydata,zdata);
            }
        }
    }
    for (int i=0;i<fieldlist.size();i++)
    {
        //QLOG_DEBUG() << "Field:" << fieldlist[i].title << fieldlist[i].variable;
        if (m_emsComms->getConfigList().contains(fieldlist[i].variable))
        {
            ScalarConfigData *data = m_emsComms->getScalarConfigData(fieldlist[i].variable);
            //widget->addParam(title,fieldlist[i],m_emsComms->getConfigData(fieldlist[i].variable));
            if (data)
            {
                widget->addParam(title,fieldlist[i],data);
            }
            else
            {
                BitConfigData *bdata = m_emsComms->getBitConfigData(fieldlist[i].variable);
                if (bdata)
                {
                    widget->addParam(title,fieldlist[i],bdata);
                    //widget->
                }
            }
        }
        else
        {
            qDebug() << "Bad Variable:" << fieldlist[i].variable;
        }
        /*for (int j=0;j<m_memoryConfigBlockList.size();j++)
        {
            //QLOG_DEBUG() << "Config block:" << m_memoryConfigBlockList[j].type();
            if (m_memoryConfigBlockList[j].name() == fieldlist[i].variable)
            {
                widget->addParam(title,fieldlist[i],m_memoryConfigBlockList[j]);
                QLOG_DEBUG() << "Found config block:" << m_memoryConfigBlockList[j].type();
                QLOG_DEBUG() << m_memoryConfigBlockList[j].name() << j;
                //This is the config block.
                //int sizeoverride = -1;
                if (m_memoryConfigBlockList[j].sizeOverride() != "")
                {
                    //There is a size override, find it.
                    for (int k=0;k<m_memoryConfigBlockList.size();k++)
                    {
                        if (m_memoryConfigBlockList[k].name() == m_memoryConfigBlockList[j].sizeOverride())
                        {
                            //sizeoverride = data[m_memoryConfigBlockList[k].offset()];
                        }
                    }
                }
            }
        }*/
        //fieldlist[i].condition
    }
}
void ParameterView::updateValues()
{
    /*if (!m_emsData)
    {
        return;
    }
    for (int i=0;i<paramWidgetList.size();i++)
    {
        QList<unsigned short> loclist = paramWidgetList[i]->getLocationIdList();
        for (int k=0;k<loclist.size();k++)
        {
            if (m_emsData->hasLocalFlashBlock(loclist[k]))
            {
                paramWidgetList[i]->updateValue(loclist[k],m_emsData->getLocalFlashBlock(loclist[k]));
            }
        }
    }*/
    return;
}

/*void ParameterView::passEmsData(EmsData *data)
{
    m_emsData = data;
    updateValues();
}*/

void ParameterView::passMenuList(MenuSetup menu)
{
    m_metaMenu = menu;
    for (int i=0;i<menu.menulist.size();i++)
    {
        //menu.menulist.
        ui.parameterTreeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << menu.menulist[i].title));
        QTreeWidgetItem *item = ui.parameterTreeWidget->topLevelItem(ui.parameterTreeWidget->topLevelItemCount()-1);
        for (int j=0;j<menu.menulist[i].subMenuList.size();j++)
        {
            //if (menu.menunamemap.contains(menu.menulist[i].second[j]))
            //{
            //	item->addChild(new QTreeWidgetItem(QStringList() << menu.menunamemap[menu.menulist[i].second[j]]));
            //}
            //else
            //{
            //	item->addChild(new QTreeWidgetItem(QStringList() << menu.menulist[i].second[j]));
            //}
            QTreeWidgetItem *childitem = new QTreeWidgetItem(QStringList() << menu.menulist[i].subMenuList[j].title << menu.menulist[i].subMenuList[j].variable);
            m_treeWidgetToVariableNameMap[childitem] = menu.menulist[i].subMenuList[j].variable;
            item->addChild(childitem);
        }
    }
}
void ParameterView::passMetaData(TSBPMemoryMetaData *metadata)
{
    m_metaData = metadata;
}

void ParameterView::passConfigBlockList(QMap<QString,QList<ConfigBlock> > blocklist)
{
    for (QMap<QString,QList<ConfigBlock> >::const_iterator i=blocklist.constBegin();i!=blocklist.constEnd();i++)
    {
        //ui.parameterTreeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << i.key()));
        //QTreeWidgetItem *item = ui.parameterTreeWidget->topLevelItem(ui.parameterTreeWidget->topLevelItemCount()-1);
        for (int j=0;j<i.value().size();j++)
        {
        //	item->addChild(new QTreeWidgetItem(QStringList() << i.value().at(j).name()));
            m_memoryConfigBlockList.append(i.value().at(j));
        }
    }
}

double ParameterView::calcAxis(unsigned short val,QList<QPair<QString,double> > metadata)
{
    if (metadata.size() == 0)
    {
        return val;
    }
    double newval = val;
    for (int j=0;j<metadata.size();j++)
    {
        if (metadata[j].first == "add")
        {
            newval += metadata[j].second;
        }
        else if (metadata[j].first == "sub")
        {
            newval -= metadata[j].second;
        }
        else if (metadata[j].first == "mult")
        {
            newval *= metadata[j].second;
        }
        else if (metadata[j].first == "div")
        {
            newval /= metadata[j].second;
        }
    }
    return newval;
}
