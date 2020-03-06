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

#include "egraph.h"
#include <QPainter>
#include <QDebug>
#include <QPainterPath>
EGraph::EGraph(QWidget *parent) : QWidget(parent)
{
	start = 0;
	stop = 0;
	selectedLine = 0;
	matchedScrolling = false;
	scrolling = true;
	graphType = 0;
	setTimer = new QTimer(this);
	connect(setTimer,SIGNAL(timeout()),this,SLOT(setTimerTimeout()));
	setTimer->start(250);
	m_setNum = 0;
	m_setUpdate = false;
	m_useTimer = false;
}
void EGraph::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
	if (graphType == 0)
	{
		QPainter painter(this);
		float labelSize = 0.08;
		painter.drawRect(0,0,width()-1,height()-1);
		if (m_values.size() > 0)
		{
			painter.drawRect(width() * 0.03,height() * 0.05,width() * (.94 - (labelSize * m_values.size())),height() * .9);
			for (int j=0;j<m_values.size();j++)
			{
				float x1 = (width() * (.94 - (labelSize * j))) - (width()*.03);
				if (m_values[j].size() > 0)
				{
					if (m_values[j].size() >= stop)
					{
					}
					//qDebug() << "Size: " << m_max[j] << "Val:" << m_values[j][0];
					QPainterPath p;
					//p.moveTo(width()*0.03,(height() * 0.9));
					p.moveTo((width()*0.03),(height()*.95) - (((m_values[j][start] - m_min[j]) / (m_max[j] - m_min[j])) * (height() * .9)));
					bool nextflat = false;
					for (int i=(int)start+1;i<=(int)stop-1;i++)
					{
						if (m_values[j].size() > i+2)
						{
							///TODO: Curve paths using quadTo or cubicTwo to smooth the graph
							//p.lineTo(((i - (int)start) * ((float)(width()*(.94-(labelSize * m_values.size()))) / (float)(stop-start)))+(width()*.03),(height()*.95) - (((m_values[j][i] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9)));

							//float mx = ((i - (int)start) * ((float)(width()*(.94-(labelSize * m_values.size()))) / (float)(stop-start)))+(width()*.03);
							//float my = (height()*.95) - (((m_values[j][i] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));
							float currentx = ((i - (int)start) * ((float)(width()*(.94-(labelSize * m_values.size()))) / (float)(stop-start)))+(width()*.03);
							float currenty = (height()*.95) - (((m_values[j][i] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));

							float prevx = (((i-1) - (int)start) * ((float)(width()*(.94-(labelSize * m_values.size()))) / (float)(stop-start)))+(width()*.03);
							float prevy = (height()*.95) - (((m_values[j][i-1] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));

							float nextx = (((i+1) - (int)start) * ((float)(width()*(.94-(labelSize * m_values.size()))) / (float)(stop-start)))+(width()*.03);
							float nexty = (height()*.95) - (((m_values[j][i+1] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));
							float mult = 3;
							float xdiff = currentx - prevx;
							float ydiff = currenty - prevy;
							float ratio = ydiff / xdiff;
							float middlex = prevx + ((currentx-prevx) / mult);
							float middley = prevy + (ratio * ((currentx-prevx)/mult));

							float nextxdiff = nextx-currentx;
							float nextydiff = nexty-currenty;
							float nextratio = nextydiff/nextxdiff;
							float nextmiddlex = currentx-((nextx-currentx) / mult);
							float nextmiddley = currenty-(nextratio*((nextx-currentx)/mult));
							//p.cubicTo(middlex,middley,nextx,nexty,currentx,currenty);
							painter.setPen(QPen(QColor::fromRgb(255,0,0)));
							painter.drawLine(middlex,middley,prevx,prevy);
							painter.setPen(QPen(QColor::fromRgb(0,0,255)));
							painter.drawLine(currentx,currenty,nextmiddlex,nextmiddley);
							painter.setPen(QPen(QColor::fromRgb(0,0,0)));
							if ((nextmiddley < currenty && middley < prevy) || (middley > prevy && nextmiddley > currenty))
							{
								p.cubicTo(middlex,currenty,nextmiddlex,currenty,currentx,currenty);
								nextflat = true;
							}
							else if (!nextflat)
							{
								p.cubicTo(middlex,middley,nextmiddlex,nextmiddley,currentx,currenty);
							}
							else
							{
								nextflat = false;
								p.cubicTo(middlex,currenty,nextmiddlex,currenty,currentx,currenty);
							}

							//}
							//p.lineTo(sx,sy);
							//p.quadTo(mx,my,sx,sy);
						}
					}
					painter.drawPath(p);
					painter.drawLine(selectedLine,0,selectedLine,height());
					float y1 = (height()*.95) - (((m_values[j][((m_values[j].size() > stop) ? stop : m_values[j].size()-1)] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));
					painter.drawLine(x1,y1,x1+5,y1);
					painter.drawText(x1+5,y1-(fontMetrics().height()/2.0),QString::number(m_values[j][((m_values[j].size() > stop) ? stop : m_values[j].size()-1)]));
				}
				else
				{
					float y1 = (height()*.95) - (((m_min[j] - m_min[j]) / (m_max[j] - m_min[j])) * (height()*.9));
					painter.drawLine(x1,y1,x1+5,y1);
					painter.drawText(x1+5,y1-(fontMetrics().height()/2.0),QString::number(m_min[j]));
				}
				painter.drawText(x1-5,(height() * .95)+fontMetrics().height(),m_labels[j]);
				painter.drawLine(x1,height() * 0.05,x1,height() * .95);
			}
		}
		painter.end();
	}
	else if (graphType == 1)
	{
		QPainter painter(this);
		//painter.drawRect(0,0,width()-1,height()-1);
		if (m_values.size() > 0)
		{
			for (int j=0;j<m_values.size();j++)
			{
				painter.drawRect(0,((height()) / m_values.size()) * j,width()-1,(height()) / m_values.size());
				painter.drawRect(width() * 0.03,(((height()) / m_values.size()) * j) + ((height() / m_values.size()) * 0.05),width() * .85,((height()) / m_values.size()) * 0.9);

				if (m_values[j].size() > 0)
				{
					//qDebug() << "Size: " << m_max[j] << "Val:" << m_values[j][0];
					QPainterPath p;
					p.moveTo(((width()) * 0.03) -1 ,(((height()-1) / m_values.size()) * (j+1)) - ((height() / m_values.size()) * 0.05));
					for (int i=(int)start;i<(int)stop;i++)
					{
						if (m_values[j].size() > i)
						{
						p.lineTo(((width() * 0.03)-1) + (i - (int)start) * ((float)(width() * 0.85) / (float)(stop-start)),(((((height()) / m_values.size()) * j) + ((height() / m_values.size()) * 0.05) + (((height()) / m_values.size()) * 0.9))) - (((m_values[j][i] - m_min[j]) / (m_max[j] - m_min[j])) * ((height() / m_values.size()) * 0.9)) - 1);
						}
						//p.lineTo((width() * 0.03) + (i - (int)start) * ((float)(width() * 0.85) / (float)(stop-start)),(((height() / m_values.size()) * (j+1)) - ((height() / m_values.size()) * 0.05)) - (((m_values[j][i] - m_min[j]) / (m_max[j] - m_min[j]))) * ((height() / m_values.size()) * 0.9));
					}
					float x = (width() * 0.85) + (width() * 0.03); //(width() * 0.03) + ((stop) - (int)start) * ((float)(width() * 0.85) / (float)(stop-start));
					//float y = ((((height()-1) / m_values.size()) * (j+1)) - ((height() / m_values.size()) * 0.05)) - (((m_values[j][stop-1] - m_min[j]) / (m_max[j] - m_min[j])) * (((height()) / m_values.size()) * 0.9));
					float y = (((((height()) / m_values.size()) * (j)) + ((height() / m_values.size()) * 0.05) + (((height()) / m_values.size()) * 0.9))) - (((m_values[j][m_values[j].size()-1] - m_min[j]) / (m_max[j] - m_min[j])) * ((height() / m_values.size()) * 0.9)) - 1;
					p.lineTo(x-1,y);
					painter.drawPath(p);
					painter.drawLine(selectedLine,0,selectedLine,height());
					painter.drawText(x+5,y + (fontMetrics().height()/2.5),QString::number(m_values[j][((m_values[j].size() > stop-1) ? stop-1 : m_values[j].size()-1)]));
					painter.drawText(x+5,(((height()-1) / m_values.size()) * (j+1)),QString::number(m_min[j]));
					painter.drawText(x+5,((((height()-1) / m_values.size()) * (j+1)) - (((height()-1) / m_values.size()))) + (fontMetrics().height()),QString::number(m_max[j]));
				}
			}
		}
		painter.end();

	}
}
int EGraph::addGraph(QString label)
{
	m_labels.append(label);
	m_values.append(QList<float>());
	m_max.append(0);
	m_min.append(0);
	return m_labels.count()-1;
	repaint();
}
void EGraph::setValue(int graph,int pos, float val)
{
	//qDebug() << m_values[0].size() << m_values[1].size() << m_values[2].size();
	//qDebug() << graph << val << pos;
	for (int i=0;i<m_values.size();i++)
	{
		int len = 0;
		if (m_useTimer)
		{
			len = m_setNum - m_values[i].size();
		}
		else
		{
			len = pos - m_values[i].size();
		}
		float last = 0;
		if (m_values[i].size() > 0)
		{
			last = m_values[i][m_values[i].size()-1];
		}
		for (int j=0;j<=len;j++)
		{
			//qDebug() << i;
			m_values[i].append(last);
		}
	}
	if (m_useTimer)
	{
		m_setUpdate = true;
		m_values[graph][m_setNum] = val;
	}
	else
	{
		m_values[graph][pos] = val;
	}
	if ((scrolling))
	{
		if (matchedScrolling)
		{
			start += ((m_values[0].count()-1) - stop);
		}
		stop = m_values[0].count()-1;
	}
	repaint();
}
void EGraph::addValue(int graph, float val)
{
	m_values[graph].append(val);
	//if (m_values.count() > 700)
	//{
	//	m_values.pop_front();
	//}
	if ((scrolling) && (stop < 300) && m_values[0].count() > 0)
	{
		stop = m_values[0].count()-1;
	}
	repaint();
}
void EGraph::setMax(int graph,float val)
{
	m_max[graph] = val;
	//m_max = val;
	repaint();
}
void EGraph::setMin(int graph, float val)
{
	m_min[graph] = val;
	repaint();
}
void EGraph::mouseMoveEvent(QMouseEvent *evt)
{
	//qDebug() << start;
	//qDebug() << stop;
	if (mouseDown)
	{
		mouseMoved = true;
		float temp = ((mouseX - evt->x()) * ((stop - start) / width()));
		if ((start + temp >= 0) && (stop + temp <= m_values[0].count()))
		{
			stop += temp;
			start += temp;
			GraphMoved(temp);
			mouseX = evt->x();
			mouseY = evt->y();
			repaint();
		}
	}
}
void EGraph::MoveGraph(float x)
{
	start += x;
	stop += x;
	repaint();
}
void EGraph::mousePressEvent(QMouseEvent *evt)
{
	mouseMoved = false;
	//scrolling = false;
	mouseX = evt->x();
	mouseY = evt->y();
	mouseDown = true;
	//int num = (int)(evt->x() * ((float)(stop-start) / (float)width()));

	//if (num >= 0 && num < m_values.count())
	//{
	//	qDebug() << m_values[num];
	//}
	MousePress(evt);
}
void EGraph::mouseReleaseEvent(QMouseEvent *evt)
{
	Q_UNUSED(evt);
	mouseDown = false;
	if (!mouseMoved)
	{
		//Click!
		if (graphType == 0)
		{
			graphType = 1;
			repaint();
		}
		else if (graphType == 1)
		{
			graphType = 0;
			repaint();
		}
	}
}
float EGraph::valueAt(int graph,int i)
{
	return m_values[graph][i];
}
int EGraph::count()
{
	return m_values.count();
}
void EGraph::select(int i)
{
	selectedLine = i;
	repaint();
}
void EGraph::wheelEvent(QWheelEvent *evt)
{
	//scrolling = false;
	if ((stop - start < 20) && evt->delta() > 0)
	{
		//qDebug() << "Invalid 1";
		return;
	}
	if ((stop - start > m_values[0].count()) && (evt->delta() < 0))
	{
		//qDebug() << "Invalid 2:" << stop-start;
		return;
	}
	//qDebug() << "Wheel" << evt->delta();
	int mag = width() - evt->x();
	float percent = (float)mag / (float)width();
	//qDebug() << mag << evt->delta() * percent;
	float tmpstart = (float)(evt->delta() / 10) * (1- percent);
	float tmpend = (evt->delta() / 10) * ( percent);
	if (start + tmpstart >= 0)
	{
		if (stop - tmpend <= m_values[0].count())
		{

		}
		else
		{
			percent = 0;
			tmpstart = (float)(evt->delta() / 10) * (1- percent);
			tmpend = (evt->delta() / 10) * ( percent);
		}
	}
	else
	{
		if (stop - tmpend <= m_values[0].count())
		{
			percent = 1;
			tmpstart = (float)(evt->delta() / 10) * (1- percent);
			tmpend = (evt->delta() / 10) * ( percent);
		}
		else
		{
			return;
		}
	}
	start += tmpstart;
	if (start < 0)
	{
		start = 0;
	}
	stop -= tmpend;
	GraphResized(start,stop);
	repaint();
}
void EGraph::ResizeGraph(float pstart, float pstop)
{
	if (pstart<0)
	{
		pstart = 0;
	}
	start = pstart;
	stop = pstop;
	repaint();
}
void EGraph::setScrollWithData(bool scroll)
{
	if (scroll)
	{
		scrolling = true;
		matchedScrolling = true;
	}
	else
	{
		scrolling = false;
		matchedScrolling = false;
	}
}
void EGraph::setShowAllData(bool show)
{
	if (show)
	{
		matchedScrolling = false;
		start = 0;
		if (m_values.count() == 0)
		{
			stop = 0;
		}
		else
		{
			stop = m_values[0].count()-1;
		}
		scrolling = true;
	}
	else
	{
		scrolling = false;
	}
}
void EGraph::clearGraphs()
{
	m_values.clear();
	m_labels.clear();
	m_max.clear();
	m_min.clear();
	start = 0;
	stop = 0;
	selectedLine = 0;
	repaint();
}
void EGraph::clearValues()
{
	for (int i=0;i<m_values.size();i++)
	{
		m_values[i].clear();
	}
	//m_values.clear();
	start = 0;
	stop = 0;
	selectedLine=0;
	repaint();
}
void EGraph::setTimerTimeout()
{
	if (m_setUpdate)
	{
		m_setUpdate = false;
		m_setNum++;
	}
	else
	{
	}
}
void EGraph::setGraphTimer(bool on)
{
	if (on)
	{
		m_setNum = 0;
	}
	m_useTimer = on;
}
