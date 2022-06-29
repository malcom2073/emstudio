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
#include "interrogateprogressview.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}
void MainWindow::connectionSelection(bool isserial,QString comorhost,int portorbaud)
{
    progress = new InterrogateProgressView();
    ui->mdiArea->addSubWindow(progress);
    progress->show();
    m_comms = new MSPComms();
    connect(m_comms,&MSPComms::interrogationComplete,this,&MainWindow::interrogationCompleted);
    m_comms->setPort("\\\\.\\COM100");
    m_comms->setBaud(115200);
    m_comms->connectSerial(isserial,comorhost,portorbaud);
    m_comms->startInterrogation();
    connect(m_comms,&MSPComms::interrogateTaskStart,this,&MainWindow::interrogateTaskStart);
    connect(m_comms,&MSPComms::interrogateTaskSucceed,progress,&InterrogateProgressView::taskSucceed);
    connect(m_comms,&MSPComms::interrogateTaskFail,progress,&InterrogateProgressView::taskFail);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::interrogationCompleted()
{
    ParameterView *param = new ParameterView();
    ui->mdiArea->addSubWindow(param);
    param->passConfigBlockList(m_comms->getMetaParser()->configMetaData());
    param->passMenuList(m_comms->getMetaParser()->menuMetaData());
    ui->buttonBar->passMenuList(m_comms->getMetaParser()->menuMetaData());
    param->setActiveComms(m_comms);
    ui->buttonBar->setActiveComms(m_comms);
    ui->buttonBar->passMdiArea(ui->mdiArea);
    //param->show();

}
void MainWindow::interrogateTaskStart(QString name,int seq)
{
    if (name == "Ecu Info")
    {
        progress->addTask(name,seq,0);
    }
    else
    {
        progress->addTask(name,seq,1);
    }

}
