#include "emap.h"
#include <QPainter>
EMap::EMap(QWidget *parent) : QWidget(parent)
{
}
void EMap::paintEvent(QPaintEvent *e)
{
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
	QPainter painter(this);
	for (int i=0;i<m_z.size();i++)
	{
		float percentx = (m_maxX - m_x.value(i)) / (xrange);
		float percenty = (m_maxY - m_y.value(i)) / (yrange);
		float percentz = (m_maxZ - m_z.value(i)) / (zrange);
		QPen p = painter.pen();
		p.setColor(QColor::fromRgb(255,255 * percentz,0));
		p.setWidth(6);
		painter.setPen(p);
		painter.drawPoint(width() - (width() * percentx),height() * percenty);
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
		painter.drawText(width() - (width() * percentx),height()-15,QString::number(xval,'f',2));

		double yval = m_minY + ((yrange / 10.0) * i);
		float percenty = (m_maxY - yval) / (yrange);
		//Position of values
		painter.drawText(10,height() * percenty,QString::number(yval,'f',2));
	}
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
	update();
}
