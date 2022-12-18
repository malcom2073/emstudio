/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mspcomms.h"
#include "dashboard.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class InterrogateProgressView;
class QMdiSubWindow;
class ConnectionDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Dashboard *m_dashboard;
public slots:
    void connectionSelection(bool isserial,QString comorhost,int portorbaud,QString inifile);
private:
    Ui::MainWindow *ui;
    MSPComms *m_comms;
    InterrogateProgressView *progress;
    QMdiSubWindow *m_dashboardWindow;
    QMdiSubWindow *m_serialConnectWindow;
private slots:
    void interrogateTaskStart(QString name,int seq);
    void interrogationCompleted();
    void dataLogPayloadDecoded(QVariantMap data);
    void logFilePosChangeReq(int pos);
    void loadLogActionClicked();
    void connectSerialActionClicked();
};
#endif // MAINWINDOW_H
