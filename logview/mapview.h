/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QVariantMap>
namespace Ui {
class MapView;
}

class MapView : public QWidget
{
	Q_OBJECT
	
public:
	explicit MapView(QWidget *parent = 0);
	~MapView();
	void addItem(QString item);
	void setData(QList<QVariantMap> *data,QList<QPair<QString, QString> > logtypes);
private:
	Ui::MapView *ui;
	QList<QVariantMap> *m_data;
private slots:
    void rangesSet(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax);
    void goButtonClicked();
    void updateMinMaxX();
    void updateMinMaxY();
    void updateMinMaxZ();
    void currentMapXChanged(QString name);
    void currentMapYChanged(QString name);
    void currentMapZChanged(QString name);
};

#endif // MAPVIEW_H
