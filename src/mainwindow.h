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
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class InterrogateProgressView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void connectionSelection(bool isserial,QString comorhost,int portorbaud,QString inifile);
private:
    Ui::MainWindow *ui;
    MSPComms *m_comms;
    InterrogateProgressView *progress;
private slots:
    void interrogateTaskStart(QString name,int seq);
    void interrogationCompleted();
};
#endif // MAINWINDOW_H
