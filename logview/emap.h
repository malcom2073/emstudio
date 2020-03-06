/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EMAP_H
#define EMAP_H

#include <QWidget>
#include <QPaintEvent>
class EMap : public QWidget
{
	Q_OBJECT
public:
	explicit EMap(QWidget *parent = 0);
	void passData(QList<double> x,QList<double> y,QList<double> z);
	void setXMinMax(double min,double max);
	void setYMinMax(double min,double max);
	void setZMinMax(double min,double max);
	void passXData(QList<double> x);
	void passYData(QList<double> y);
	void passZData(QList<double> z);
public slots:
	void autoScaleX();
	void autoScaleY();
	void autoScaleZ();
protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *evt);
private:
	bool m_redrawBackground;
	QImage *m_mapImage;
	void reDrawMap(QPainter *painter);
	QList<double> m_x;
	QList<double> m_y;
	QList<double> m_z;
	double m_minX;
	double m_minY;
	double m_minZ;
	double m_maxX;
	double m_maxY;
	double m_maxZ;

	double m_autoMinX;
	double m_autoMinY;
	double m_autoMinZ;
	double m_autoMaxX;
	double m_autoMaxY;
	double m_autoMaxZ;
signals:
	void rangesSet(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax);
	
};

#endif // EMAP_H
