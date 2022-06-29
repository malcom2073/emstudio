/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameterview.h"
#include "connectiondialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}
void MainWindow::connectionSelection(bool isserial,QString comorhost,int portorbaud)
{
    m_comms = new MSPComms();
    connect(m_comms,&MSPComms::interrogationComplete,this,&MainWindow::interrogationCompleted);
    m_comms->setPort("\\\\.\\COM100");
    m_comms->setBaud(115200);
    m_comms->connectSerial(isserial,comorhost,portorbaud);
    m_comms->startInterrogation();

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::interrogationCompleted()
{
    ParameterView *param = new ParameterView();
    param->passConfigBlockList(m_comms->getMetaParser()->configMetaData());
    param->passMenuList(m_comms->getMetaParser()->menuMetaData());
    param->setActiveComms(m_comms);
    param->show();

}
