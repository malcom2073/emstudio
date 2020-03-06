/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include <QWidget>
#include <QGLWidget>
#include <QList>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
class EGraph : public QGLWidget
{
	Q_OBJECT
public:
	EGraph(QWidget *parent=0);
	void addValue(int graph,float val);
	void setValue(int graph,int pos, float val);
	void setMax(int graph,float val);
	void setMin(int graph, float val);
	float start;
	float stop;
	float valueAt(int graph,int i);
	int count();
	void select(int i);
	int addGraph(QString label,QString suffix);
	void clearGraphs();
	void clearValues();
	void setScrollWithData(bool scroll);
	void setShowAllData(bool show);
	void setGraphTimer(bool on);
protected:
	bool m_useTimer;
	QTimer *setTimer;
	bool scrolling;
	bool m_setUpdate;
	bool matchedScrolling;
	int selectedLine;
	int graphType;
	QList<QList<float> > m_values;
	QList<QString> m_labels;
	QList<QString> m_suffix;
	unsigned long m_setNum;
	//QList<int> selectedLine;
	QList<float> m_max;
	QList<float> m_min;
	bool mouseMoved;
	void paintEvent(QPaintEvent *e);
	void mouseMoveEvent(QMouseEvent *evt);
	void mousePressEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	bool mouseDown;
	int mouseX;
	int mouseY;
	void wheelEvent(QWheelEvent *evt);
signals:
	void GraphMoved(float x);
	void GraphResized(float start, float stop);
	void MousePress(QMouseEvent*);
public slots:
	void MoveGraph(float x);
	void ResizeGraph(float start, float stop);
private slots:
	void setTimerTimeout();
};
