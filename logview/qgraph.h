/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef QGRAPH_H
#define QGRAPH_H

#include <QWidget>
#include <QGLWidget>
class QGraph : public QGLWidget
{
	Q_OBJECT
public:
	explicit QGraph(QWidget *parent = 0);
	void addData(QList<float> data,QString name,QString suffix);
	void removeData(QString name);
	void clear();
protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
private:
	QList<QList<float> > m_dataList;
	QList<QList<float> > m_parsedDataList;
	QList<QColor> m_colorList;
	QList<float> m_maxList;
	QList<QString> m_nameList;
	QList<QString> m_suffixList;
	bool m_mouseDown;
	int m_maxViewX;
	int m_minViewX;
	int m_mouseDownMinViewX;
	int m_mouseDownMaxViewX;
	int m_mouseX;
	int m_mouseY;
	int m_mouseDownX;
	int m_mouseDownY;
	QList<float> downsample(QList<float> data, int threshold);
	void recalculate();
signals:
	
public slots:
	
};

#endif // QGRAPH_H
