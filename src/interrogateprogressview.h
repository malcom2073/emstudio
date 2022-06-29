/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef INTERROGATEPROGRESSVIEW_H
#define INTERROGATEPROGRESSVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "ui_interrogateprogressview.h"
#include "overviewprogressitemdelegate.h"
class InterrogateProgressView : public QWidget
{
    Q_OBJECT

public:
    explicit InterrogateProgressView(QWidget *parent = 0);
    ~InterrogateProgressView();
    void setProgress(int progress);
    void setMaximum(int maximum);
    void addOutput(QString output);
    void reset();
    void done();

    int maximum() { return ui.progressBar->maximum(); }
    int progress() { return ui.progressBar->value(); }
protected:
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
private:
    Ui::InterrogateProgressView ui;
    QMap<int,int> m_typeToOverviewListMap;
public slots:
    void addTask(QString task, int sequencenumber,int type);
    void taskFail(int sequencenumber);
    void taskSucceed(int sequencenumber);
private slots:
    void cancelClickedSlot();
signals:
    void cancelClicked();
    void windowHiding(QMdiSubWindow *parent);
};

#endif // INTERROGATEPROGRESSVIEW_H
