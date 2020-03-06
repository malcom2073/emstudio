/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "emap.h"
#include <QPainter>
EMap::EMap(QWidget *parent) : QWidget(parent)
{
	m_mapImage = 0;
	m_redrawBackground =  true;
}
void EMap::reDrawMap(QPainter *painter2)
{
	if (!m_mapImage)
	{
		return;
	}
	QPainter painter(m_mapImage);
	QPen pen = painter.pen();
	pen.setWidth(2);
	pen.setColor(QColor::fromRgb(0,0,0));
	painter.setPen(pen);
	painter.fillRect(0,0,width(),height(),Qt::SolidPattern);
	if (m_x.size() == 0 || m_y.size() == 0 || m_z.size() == 0)
	{
		return;
	}
	double xrange = m_maxX - m_minX;
	double yrange = m_maxY - m_minY;
	double zrange = m_maxZ - m_minZ;
	if (zrange == 0)
	{
		zrange = 1;
	}

	painter.drawRect(0,0,width()-1,height());
	int dataoffsetx = 40;
	int dataoffsety = 20;
	int datawidth = width() - (dataoffsetx * 2.0);
	int dataheight = height() - (dataoffsety * 2.0);
	painter.drawRect(dataoffsetx,dataoffsety,datawidth - 1,dataheight-1);
	pen.setWidth(1);
	painter.setPen(pen);

	for (int i=0;i<m_z.size();i++)
	{
		float percentx = (m_maxX - m_x.value(i)) / (xrange);
		float percenty = (m_maxY - m_y.value(i)) / (yrange);
		float percentz = (m_maxZ - m_z.value(i)) / (zrange);
		if (percentx < 0 || percentx > 1)
		{
		    continue;
		}
		if (percenty < 0 || percenty > 1)
		{
		    continue;
		}
		if (percentz < 0 || percentz > 1)
		{
		    //continue;
		}
		QPen p = painter.pen();
		p.setColor(QColor::fromRgb(255,255 * percentz,0));
		p.setWidth(1);
		painter.setPen(p);
		painter.drawPoint(dataoffsetx + (datawidth - (datawidth * percentx)),dataoffsety + (dataheight * percenty));
	}
	QPen p = painter.pen();
	p.setColor(QColor::fromRgb(0,0,0));
	p.setWidth(1);
	painter.setPen(p);
	for (int i=0;i<=10;i++)
	{
		double xval = m_minX + ((xrange / 10.0) * i);
		float percentx = (m_maxX - xval) / (xrange);
		//Position of values
		painter.drawText(dataoffsetx + (datawidth - (datawidth * percentx)),height()-5,QString::number(xval,'f',2));

		double yval = m_minY + ((yrange / 10.0) * i);
		float percenty = (m_maxY - yval) / (yrange);
		//Position of values
		painter.drawText(5,dataheight * percenty,QString::number(yval,'f',2));
	}
}
void EMap::resizeEvent(QResizeEvent *evt)
{
	//Q_UNUSED(oldgeometry);
	delete m_mapImage;
	m_mapImage = new QImage(evt->size().width(),evt->size().height(),QImage::Format_ARGB32);
	m_redrawBackground = true;
	//drawBackground();
}
void EMap::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (!m_mapImage)
	{
		return;
	}
	if (m_redrawBackground)
	{
		m_redrawBackground = false;
		reDrawMap(&painter);
	}

	painter.drawImage(0,0,*m_mapImage);



}
void EMap::passXData(QList<double> x)
{
    //Automatically scale when new data is received.
	m_x = x;
	if (m_x.size() > 0)
	{
		m_minX = m_x.value(0);
		m_maxX = m_x.value(0);
		for (int i=0;i<m_x.size();i++)
		{
			if (m_minX > m_x.value(i))
			{
				m_minX = m_x.value(i);
			}
			if (m_maxX < m_x.value(i))
			{
				m_maxX = m_x.value(i);
			}
		}
	}
	m_autoMinX = m_minX;
	m_autoMaxX = m_maxX;
	emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
	update();
}
void EMap::passYData(QList<double> y)
{
    //Automatically scale when new data is received.
    m_y = y;
    if (m_y.size() > 0)
    {
        m_minY = m_y.value(0);
        m_maxY = m_y.value(0);
        for (int i=0;i<m_y.size();i++)
        {
            if (m_minY > m_y.value(i))
            {
                m_minY = m_y.value(i);
            }
            if (m_maxY < m_y.value(i))
            {
                m_maxY = m_y.value(i);
            }
        }
    }
    m_autoMinY = m_minY;
    m_autoMaxY = m_maxY;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
    update();
}
void EMap::passZData(QList<double> z)
{
    //Automatically scale when new data is received.
    m_z = z;
    if (m_z.size() > 0)
    {
        m_minZ = m_z.value(0);
        m_maxZ = m_z.value(0);
        for (int i=0;i<m_z.size();i++)
        {
            if (m_minZ > m_z.value(i))
            {
                m_minZ = m_z.value(i);
            }
            if (m_maxZ < m_z.value(i))
            {
                m_maxZ = m_z.value(i);
            }
        }
    }
    m_autoMinZ = m_minZ;
    m_autoMaxZ = m_maxZ;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
    update();
}

void EMap::passData(QList<double> x,QList<double> y,QList<double> z)
{
	m_x = x;
	m_y = y;
	m_z = z;
	if (m_x.size() > 0)
	{
		m_minX = m_x.value(0);
		m_maxX = m_x.value(0);
		for (int i=0;i<m_x.size();i++)
		{
			if (m_minX > m_x.value(i))
			{
				m_minX = m_x.value(i);
			}
			if (m_maxX < m_x.value(i))
			{
				m_maxX = m_x.value(i);
			}
		}
	}
	if (m_y.size() > 0)
	{
		m_minY = m_y.value(0);
		m_maxY = m_y.value(0);
		for (int i=0;i<m_y.size();i++)
		{
			if (m_minY > m_y.value(i))
			{
				m_minY = m_y.value(i);
			}
			if (m_maxY < m_y.value(i))
			{
				m_maxY = m_y.value(i);
			}
		}
	}
	if (m_z.size() > 0)
	{
		m_minZ = m_z.value(0);
		m_maxZ = m_z.value(0);
		for (int i=0;i<m_z.size();i++)
		{
			if (m_minZ > m_z.value(i))
			{
				m_minZ = m_z.value(i);
			}
			if (m_maxZ < m_z.value(i))
			{
				m_maxZ = m_z.value(i);
			}
		}
	}
    m_autoMinX = m_minX;
    m_autoMaxX = m_maxX;
    m_autoMinY = m_minY;
    m_autoMaxY = m_maxY;
    m_autoMinZ = m_minZ;
    m_autoMaxZ = m_maxZ;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
	update();
}
void EMap::setXMinMax(double min,double max)
{
    m_minX = min;
    m_maxX = max;
    update();
}

void EMap::setYMinMax(double min,double max)
{
    m_minY = min;
    m_maxY = max;
    update();
}

void EMap::setZMinMax(double min,double max)
{
    m_minZ = min;
    m_maxZ = max;
    update();
}

void EMap::autoScaleX()
{
    m_minX = m_autoMinX;
    m_maxX = m_autoMaxX;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
    update();
}

void EMap::autoScaleY()
{
    m_minY = m_autoMinY;
    m_maxY = m_autoMaxY;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
    update();
}

void EMap::autoScaleZ()
{
    m_minZ = m_autoMinZ;
    m_maxZ = m_autoMaxZ;
    emit rangesSet(m_minX,m_maxX,m_minY,m_maxY,m_minZ,m_maxZ);
    update();
}
