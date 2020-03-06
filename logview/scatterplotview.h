/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef SCATTERPLOTVIEW_H
#define SCATTERPLOTVIEW_H

#include <QWidget>

namespace Ui {
class ScatterPlotView;
}

class ScatterPlotView : public QWidget
{
	Q_OBJECT

public:
	explicit ScatterPlotView(QWidget *parent = 0);
	~ScatterPlotView();
	void setData(QList<QVariantMap> *data,QList<QPair<QString, QString> > logtypes);
private:
	Ui::ScatterPlotView *ui;
};

#endif // SCATTERPLOTVIEW_H
