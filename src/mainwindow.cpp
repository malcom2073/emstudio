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
#include "consoletextview.h"
#include "logfilemanager.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dashboard = new Dashboard();
    m_dashboard->setFile("default_dashboard.qml");
    ui->mdiArea->addSubWindow(m_dashboard);
    m_dashboard->show();
    LogFileManager *manager = new LogFileManager(this);
    connect(manager,&LogFileManager::dataLogPayloadDecoded,this,&MainWindow::dataLogPayloadDecoded);
    connect(manager,&LogFileManager::logFileProgress,ui->widget,&LogControlBar::setTime);

}
void MainWindow::connectionSelection(bool isserial,QString comorhost,int portorbaud,QString inifile)
{
    progress = new InterrogateProgressView();
    ui->mdiArea->addSubWindow(progress);
    progress->show();
    m_comms = new MSPComms();
    connect(m_comms,&MSPComms::interrogationComplete,this,&MainWindow::interrogationCompleted);
    m_comms->setPort("\\\\.\\COM100");
    m_comms->setBaud(115200);
    m_comms->connectSerial(isserial,comorhost,portorbaud,inifile);
    m_comms->startInterrogation();
    connect(m_comms,&MSPComms::interrogateTaskStart,this,&MainWindow::interrogateTaskStart);
    connect(m_comms,&MSPComms::interrogateTaskSucceed,progress,&InterrogateProgressView::taskSucceed);
    connect(m_comms,&MSPComms::interrogateTaskFail,progress,&InterrogateProgressView::taskFail);
    connect(m_comms,&MSPComms::dataLogPayloadDecoded,this,&MainWindow::dataLogPayloadDecoded);

}
void MainWindow::logFilePosChangeReq(int pos)
{

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

    ConsoleTextView *console = new ConsoleTextView();
    ui->mdiArea->addSubWindow(console);
    connect(m_comms,&MSPComms::consoleText,console,&ConsoleTextView::consoleText);
    console->show();



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
void MainWindow::dataLogPayloadDecoded(QVariantMap data)
{
    QVariantMap::const_iterator i = data.constBegin();
    while (i != data.constEnd())
    {
            m_dashboard->propertyMap.setProperty(i.key().toLatin1(),i.value());
        //ui.tableWidget->item(m_nameToIndexMap[i.key()],1)->setText(QString::number(i.value()));
        i++;
    }
}
