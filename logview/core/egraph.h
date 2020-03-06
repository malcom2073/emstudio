/**************************************************************************
*   Copyright (C) 2010 by Michael Carpenter (malcom2073)                  *
*   malcom2073@gmail.com                                                  *
*                                                                         *
*   This file is a part of OBDToolbox                                     *
*                                                                         *
*   OBDToolbox is free software: you can redistribute it and/or modify    *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   OBDToolbox is distributed in the hope that it will be useful,         *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with OBDToolbox.  If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/
#include <QWidget>
#include <QList>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
class EGraph : public QWidget
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
	int addGraph(QString label);
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
