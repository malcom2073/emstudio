/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TABLEMAP3D_H
#define TABLEMAP3D_H

#include <QWidget>
#include <QPaintEvent>
#include <table3ddata.h>
#include <QtOpenGL>
#include <QPoint>
class TableMap3D : public QGLWidget
{
	Q_OBJECT
public:
	explicit TableMap3D(QWidget *parent = 0);
	void passData(Table3DData *tableData);
protected:
	float m_xrot;
	float m_yrot;
	float m_zrot;
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *evt);
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	bool m_mouseDown;
	QPoint lastPos;
private:
	Table3DData *m_tableData;
	QColor getColor(float value,float max);
signals:

public slots:
	
};

#endif // TABLEMAP3D_H
