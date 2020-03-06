/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "qgraph.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <math.h>

QGraph::QGraph(QWidget *parent) : QGLWidget(parent)
{
	m_mouseDown = false;

	m_maxViewX = 100;
	m_minViewX = 0;
	this->setMouseTracking(true);
}
void QGraph::addData(QList<float> data,QString name,QString suffix)
{
	if (m_dataList.size() > 0)
	{
		if (m_dataList[0].size() != data.size())
		{
			qDebug() << m_dataList[0].size() << data.size();
			return;
		}
	}
	m_dataList.append(data);
	m_colorList.append(QColor::fromRgb(100 + (rand() % 155),100 + (rand() % 155),100 + (rand() % 155)));
	float max = 0;
	for (int i=0;i<data.size();i++)
	{
		if (data[i] > max)
		{
			max = data[i];
		}
	}
	qDebug() << "Max:" << max;
	m_maxList.append(max);
	m_nameList.append(name);
	m_suffixList.append(suffix);
	if (m_dataList.size() == 1)
	{
		//first item added, resize
		m_maxViewX = m_dataList[0].size()-1;
	}
	recalculate();
	update();
}
void QGraph::clear()
{
	m_maxList.clear();
	m_dataList.clear();
	m_colorList.clear();
	m_nameList.clear();
	m_suffixList.clear();
	recalculate();
	update();
}

void QGraph::removeData(QString name)
{
	for (int i=0;i<m_nameList.size();i++)
	{
		if (m_nameList[i] == name)
		{
			m_maxList.removeAt(i);
			m_dataList.removeAt(i);
			m_colorList.removeAt(i);
			m_nameList.removeAt(i);
			m_suffixList.removeAt(i);
			recalculate();
			update();
			return;
		}
	}
}
QList<float> QGraph::downsample(QList<float> data, int threshold)
{
	QList<float> retval;
	if ((threshold > data.size()-1) || (threshold == 0))
	{
		return data;
	}

	int bucket_size = (data.size() - 2) / (threshold - 2);

	int a = 0;
	float max_area_point;
	float max_area;
	float area;
	int next_a;

	retval.append(data[a]);
	for (int i = 0; i < threshold - 2; i++)
	{
		float avg_x = 0;
		float avg_y = 0;
		float avg_range_start  = floor((float)( ( i + 1 ) * bucket_size )) + 1;
		float avg_range_end    = floor((float)( ( i + 2 ) * bucket_size )) + 1;
		avg_range_end = avg_range_end < data.size() ? avg_range_end : data.size();

		float avg_range_length = avg_range_end - avg_range_start;

		for ( ; avg_range_start<avg_range_end; avg_range_start++ )
		{
			avg_x += avg_range_start;
			avg_y += data[ avg_range_start ];
		}
		avg_x /= avg_range_length;
		avg_y /= avg_range_length;

		float  range_offs = floor((float)( (i + 0) * bucket_size )) + 1;
		float range_to   = floor((float)( (i + 1) * bucket_size )) + 1;

		float point_a_x = a;
		float point_a_y = data[ a ];

		max_area = area = -1;

		for ( ; range_offs < range_to; range_offs++ )
		{
			// Calculate triangle area over three buckets
			float area = abs( ( point_a_x - avg_x ) * ( data[ range_offs ] - point_a_y ) -
			( point_a_x - range_offs) * ( avg_y - point_a_y )
			) * 0.5;
			if ( area > max_area )
			{
				max_area = area;
				max_area_point = data[ range_offs ];
				next_a = range_offs; // Next a is this b
			}
		}
		retval.append(max_area_point);
		a = next_a;
	}
	retval.append(data[data.size()-1]);
	return retval;
}
void QGraph::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing,true);
	p.setPen(QColor::fromRgb(0,0,0));
	p.fillRect(0,0,width()-1,height()-1,Qt::SolidPattern);
	p.setPen(QColor::fromRgb(255,255,255));
	p.drawRect(0,0,width()-1,height()-1);
	p.drawRect(10,10,width()-21,height()-21);
	int minx = 10;
	int miny = height()-11;
	int maxx = width()-11;
	int maxy = 10;

/*
	for (int j=0;j<m_dataList.size();j++)
	{
		QPainterPath path;
		p.setPen(m_colorList[j]);
		path.moveTo(minx,miny);
		//int count = (float)(m_maxViewX - m_minViewX) / (float)(maxx-minx);
		//int count = m_maxViewX - m_minViewX; //Full Span
		int count = 1;
		//qDebug() << "Count:" << count;
		if (count == 0)
		{
			count = 1;
		}
		float mindist = ((float)(maxx-minx) / (float)(m_maxViewX-m_minViewX)) * count; //Will be closer than this to a X point;
		mindist = mindist / 2.0;
		for (int i=m_minViewX;i<=m_maxViewX;i+=count)
		{
			float x = minx + (((float)(i-m_minViewX) / (float)(m_maxViewX-m_minViewX)) * (maxx - minx));
			if ((x-m_mouseX < mindist && x-m_mouseX > 0) || (m_mouseX-x < mindist && m_mouseX-x > 0))
			{
				//qDebug() << x << m_mouseX << mindist;
				p.drawLine(x,miny,x,maxy);
				p.drawText(x+10,m_mouseY + (20 * j),QString::number(m_dataList[j][i]).append("\t").append(m_nameList[j]));
			}
			if (m_maxList[j] != 0)
			{
				float y = miny + ((m_dataList[j][i]/m_maxList[j]) * (maxy-miny));
				path.lineTo(x,y);
				//path.addEllipse(x-2.5,y-2.5,5,5);
				path.moveTo(x,y);
			}
		}

		p.drawPath(path);
	}*/
	for (int j=0;j<m_parsedDataList.size();j++)
	{
		QPainterPath path;
		p.setPen(m_colorList[j]);
		path.moveTo(minx,miny);
		//int count = (float)(m_maxViewX - m_minViewX) / (float)(maxx-minx);
		//int count = m_maxViewX - m_minViewX; //Full Span
		int count = 1;
		//qDebug() << "Count:" << count;
		if (count == 0)
		{
			count = 1;
		}
		//float mindist = ((float)(maxx-minx) / (float)(m_maxViewX-m_minViewX)) * count; //Will be closer than this to a X point;
		float mindist = ((float)(maxx-minx) / (m_parsedDataList[j].size())) * count; //Will be closer than this to a X point;
		mindist = mindist / 2.0;
		for (int i=0;i<m_parsedDataList[j].size();i+=count)
		{
			float x = minx + (((float)(i) / (float)(m_parsedDataList[j].size())) * (maxx - minx));
			if ((x-m_mouseX < mindist && x-m_mouseX > 0) || (m_mouseX-x < mindist && m_mouseX-x > 0))
			{
				//qDebug() << x << m_mouseX << mindist;
				p.drawLine(x,miny,x,maxy);
				p.drawText(x+10,m_mouseY + (20 * j),QString::number(m_parsedDataList[j][i]).append(" ").append(m_suffixList[j]).append("\t").append(m_nameList[j]));
			}
			if (m_maxList[j] != 0)
			{
				float y = miny + ((m_parsedDataList[j][i]/m_maxList[j]) * (maxy-miny));
				path.lineTo(x,y);
				//path.addEllipse(x-2.5,y-2.5,5,5);
				path.moveTo(x,y);
			}
		}

		p.drawPath(path);
	}

}
void QGraph::mouseMoveEvent(QMouseEvent *event)
{
	if (m_dataList.size() == 0)
	{
		return;
	}
	if (m_mouseDown)
	{
		double mult = (double)(m_maxViewX - m_minViewX) / (double)width();
		int newminval = m_mouseDownMinViewX + ((m_mouseDownX - event->pos().x()) * mult);
		int newmaxval = m_mouseDownMaxViewX + ((m_mouseDownX - event->pos().x()) * mult);
		if (newminval < 0)
		{
			m_minViewX = 0;
			m_maxViewX = (newmaxval + (0 - newminval));
		}
		else if (newmaxval > m_dataList[0].size()-1)
		{
			m_maxViewX = m_dataList[0].size()-1;
			m_minViewX = newminval - (newmaxval - (m_dataList[0].size()-1));
		}
		else
		{
			m_minViewX = m_mouseDownMinViewX + (m_mouseDownX - event->pos().x()) * mult;
			m_maxViewX = m_mouseDownMaxViewX + (m_mouseDownX - event->pos().x()) * mult;
		}


		if (m_maxViewX >= m_dataList[0].size()-1) m_maxViewX = m_dataList[0].size()-1;
	}
	m_mouseX = event->pos().x();
	m_mouseY = event->pos().y();
	recalculate();
	update();
}

void QGraph::mousePressEvent(QMouseEvent *event)
{
	m_mouseDown = true;
	m_mouseX = event->pos().x();
	m_mouseY = event->pos().y();
	m_mouseDownMinViewX = m_minViewX;
	m_mouseDownMaxViewX = m_maxViewX;
	m_mouseDownX = m_mouseX;
	m_mouseDownY = m_mouseY;
}

void QGraph::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseDown = false;
}

void QGraph::wheelEvent(QWheelEvent *event)
{
	if (m_dataList.size() == 0)
	{
		return;
	}
	float deltaval = 0 - (event->delta() / 2.0);
	if (deltaval > 0)
	{
		deltaval = (m_maxViewX - m_minViewX) * 0.10;
	}
	else
	{
		deltaval = 0- ((m_maxViewX - m_minViewX) * 0.10);
	}
	//Take into account mouse position, 1.0 == right corner, 0 == left corner
	double ratio = (double)m_mouseX / (double)width();
	if (((m_minViewX - (deltaval * ratio)) >= 0) && ((m_maxViewX + (deltaval * (1.0 - ratio))) <= m_dataList[0].size()-1))
	{
		if (abs((m_minViewX - (deltaval * ratio)) - (m_maxViewX + (deltaval * (1.0-ratio)))) > 5)
		{
			m_minViewX -= deltaval * ratio;
			m_maxViewX += deltaval * (1.0-ratio);
		}
	}
	else if ((m_minViewX - deltaval) >= 0)
	{
		m_minViewX -= deltaval;
	}
	else if ((m_maxViewX + deltaval) <= m_dataList[0].size()-1)
	{
		m_maxViewX += deltaval;
	}
	recalculate();
	update();
}
void QGraph::recalculate()
{
	m_parsedDataList.clear();
	for (int j=0;j<m_dataList.size();j++)
	{
		QList<float> sample;
		for (int i=m_minViewX;i<=m_maxViewX;i+=1)
		{
			sample.append(m_dataList[j][i]);
		}
		m_parsedDataList.append(downsample(sample,width()));
	}
}
