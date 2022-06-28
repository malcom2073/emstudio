/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef PARAMETERVIEW_H
#define PARAMETERVIEW_H

#include <QWidget>
#include <QMdiSubWindow>
#include "ui_parameterview.h"
#include "configblock.h"
#include "parameterwidget.h"
#include "configdata.h"
#include "tsbpmemorymetadata.h"
#include "menusetup.h"
#include "mspcomms.h"
class ParameterView : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterView(QWidget *parent = 0);
    void passMenuList(MenuSetup menu);
    void passConfigBlockList(QMap<QString,QList<ConfigBlock> > blocklist);
    void generateDialog(DialogItem item,QString dir="",ParameterWidget *parent=0);
    //void passEmsData(EmsData *data);
    void passMetaData(TSBPMemoryMetaData *metadata);
    void updateValues();
    double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
    void addConfig(QString name,ConfigData *data);
    void setActiveComms(MSPComms* comms);
private:
    MSPComms *m_emsComms;
    QMap<QTreeWidgetItem*,QString> m_treeWidgetToVariableNameMap;


    TSBPMemoryMetaData *m_metaData;
    QList<ParameterWidget*> paramWidgetList;
    QMap<QLineEdit*,ConfigBlock> lineEditToConfigBlockMap;
    QMap<QWidget*,QList<DialogField> > widgetToFieldMap;
    //EmsData *m_emsData;
    MenuSetup m_metaMenu;
    QList<ConfigBlock> m_memoryConfigBlockList;
    Ui::ParameterView ui;
    //Ui::Form ui;
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
    void windowHiding(QMdiSubWindow *parent);
    void showTable(QString table);
public slots:
    void currentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *prev);
    void itemSelectionChanged();
};

#endif // PARAMETERVIEW_H
/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef PARAMETERVIEW_H
#define PARAMETERVIEW_H

#include <QWidget>
#include <QMdiSubWindow>
#include "ui_parameterview.h"
#include "configblock.h"
#include "parameterwidget.h"
#include "configdata.h"
#include "memorymetadata.h"
#include "menusetup.h"
#include "emscomms.h"
class ParameterView : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterView(QWidget *parent = 0);
    void passMenuList(MenuSetup menu);
    void passConfigBlockList(QMap<QString,QList<ConfigBlock> > blocklist);
    void generateDialog(DialogItem item);
    //void passEmsData(EmsData *data);
    void passMetaData(MemoryMetaData *metadata);
    void updateValues();
    double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
    void addConfig(QString name,ConfigData *data);
    void setActiveComms(EmsComms* comms);
private:
    EmsComms *m_emsComms;
    MemoryMetaData *m_metaData;
    QList<ParameterWidget*> paramWidgetList;
    QMap<QLineEdit*,ConfigBlock> lineEditToConfigBlockMap;
    QMap<QWidget*,QList<DialogField> > widgetToFieldMap;
    //EmsData *m_emsData;
    MenuSetup m_metaMenu;
    QList<ConfigBlock> m_memoryConfigBlockList;
    Ui::Form ui;
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
    void windowHiding(QMdiSubWindow *parent);
    void showTable(QString table);
public slots:
    void currentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *prev);
    void itemSelectionChanged();
};

#endif // PARAMETERVIEW_H
