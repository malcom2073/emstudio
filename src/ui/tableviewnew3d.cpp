/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tableviewnew3d.h"
#include <QPainter>
#include <QMouseEvent>

TableViewNew3D::TableViewNew3D(QWidget *parent) : QWidget(parent)
{
	m_itemHeight = 30;
	m_itemWidth = 60;
	multiSelect = false;
	m_rowCount = 16;
	m_columnCount = 16;
	setFocusPolicy(Qt::ClickFocus);
	m_traceEnabled = false;
	m_updateTimer = new QTimer(this);
	connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(update()));
	m_updateTimer->start(100);
	m_inEdit = false;
	qRegisterMetaType<QList<QPair<int,int> > >("QList<QPair<int,int> >");
	m_minSelectedRow = 0;
	m_maxSelectedRow = 0;
	m_minSelectedColumn = 0;
	m_maxSelectedColumn = 0;

}
void TableViewNew3D::setTracingValue(double x,double y)
{
	m_traceX = x;
	m_traceY = y;
}

void TableViewNew3D::addHotkey(int key,Qt::KeyboardModifier modifier)
{
	m_hotkeyMap.append(QPair<int,Qt::KeyboardModifier>(key,modifier));
}

void TableViewNew3D::resizeEvent(QResizeEvent *evt)
{
	m_itemWidth = this->width() / (m_columnCount + 1);
	m_itemHeight = this->height() / (m_rowCount + 1);
	this->update();
}

void TableViewNew3D::setXAxis(int index,QString number)
{
	xaxis[index] = number;
}

void TableViewNew3D::setYAxis(int index,QString number)
{
	yaxis[index] = number;
}
QString TableViewNew3D::xAxis(int index)
{
	return xaxis[index];
}

QString TableViewNew3D::yAxis(int index)
{
	return yaxis[index];
}

void TableViewNew3D::clear()
{
	//Clear everything out
	yaxis.clear();
	m_highlightList.clear();
	values.clear();
	xaxis.clear();
}
void TableViewNew3D::rebuildTable()
{
	yaxis.clear();
	m_highlightList.clear();
	values.clear();
	xaxis.clear();
	for (int y=0;y<m_rowCount;y++)
	{
		yaxis.append("0");
		QList<QString> row;
		QList<int> highlightrow;
		highlightrow.append(0); //For the Y column
		for (int x=0;x<m_columnCount;x++)
		{
			xaxis.append("0");
			row.append("0");
			highlightrow.append(0);
		}
		m_highlightList.append(highlightrow);
		if (y == 0)
		{
			m_highlightList.append(highlightrow); //Double add it for the last row
		}
		highlightrow.clear();
		values.append(row);
	}

}

void TableViewNew3D::setRowCount(int count)
{
	if (count < m_rowCount)
	{
		//Reducing the number of rows
	}
	else if (count > m_rowCount)
	{

	}
	else
	{
		//Row count stays the same
		return;
	}
	//Assume row count is changing from 0 to whatever
	m_rowCount = count;
	rebuildTable();
}
void TableViewNew3D::setItem(int row,int column,QString text)
{
	if (row == -1)
	{
		//X Axis
		if (column == -1)
		{
			//Ignore
			return;
		}
		setXAxis(column,text);
	}
	else if (column == -1)
	{
		//Y Axis
		setYAxis(row,text);
	}
	else
	{
		values[row][column] = text;
	}
	if (currentCell.x() == row && currentCell.y() == column)
	{
		emit currentSelectionChanged(m_selectionList);
	}
	update();
}
QString TableViewNew3D::item(int row,int column)
{
	if (row == -1)
	{
		//XAxis
		if (column == -1)
		{
			return "";
		}
		return xaxis[column];
	}
	else if (column == -1)
	{
		return yaxis[row];
	}
	else
	{
		return values[row][column];
	}
}
void TableViewNew3D::setColumnCount(int count)
{
	m_columnCount = count;
	rebuildTable();
}
void TableViewNew3D::setMaxValues(double maxx,double maxy,double maxz)
{
	m_maxXValue = maxx;
	m_maxYValue = maxy;
	m_maxZValue = maxz;
}

void TableViewNew3D::drawCell(QPainter *painter,int cellx,int celly,QString text,bool highlight, bool current)
{
	QPen oldpen = painter->pen();
	QPen pen = oldpen;
	if (current)
	{
		pen.setColor(QColor::fromRgb(255,255,255));
		pen.setWidth(2);
		painter->setPen(pen);
		painter->drawRect((cellx*m_itemWidth)+2,(celly*m_itemHeight)+2,m_itemWidth-2,m_itemHeight-2);
	}
	else if (highlight)
	{
		pen.setColor(QColor::fromRgb(0,0,255));
		pen.setWidth(2);
		painter->setPen(pen);
		painter->drawRect((cellx*m_itemWidth)+2,(celly*m_itemHeight)+2,m_itemWidth-2,m_itemHeight-2);
	}
	else
	{
		pen.setColor(QColor::fromRgb(0,0,0));
		painter->setPen(pen);
		painter->drawRect((cellx*m_itemWidth)+1,(celly*m_itemHeight)+1,m_itemWidth-1,m_itemHeight-1);
	}

	pen.setColor(QColor::fromRgb(0,0,0));

	//For now, disable coloring for axis
	if (current || cellx == 0 || celly == m_rowCount || highlight)
	{
		painter->fillRect((m_itemWidth*cellx)+4,(m_itemHeight*celly)+4,m_itemWidth-5,m_itemHeight-5,QColor::fromRgb(255,255,255));
	}
	else
	{
		double val = text.toDouble();
		double max = 255;
		if (cellx == 0)
		{
			max = m_maxYValue;
		}
		else if (celly == m_rowCount)
		{
			max = m_maxXValue;
		}
		else
		{
			max = m_maxZValue;
		}

		if (val < max/4.0)
		{
			QColor bgcolor = QColor::fromRgb(0,(255*((val)/(max/4.0))),255);
			painter->fillRect((m_itemWidth*cellx)+2,(m_itemHeight*celly)+2,m_itemWidth-2,m_itemHeight-2,bgcolor);
		}
		else if (val < ((max/4.0)*2))
		{
			QColor bgcolor = QColor::fromRgb(0,255,255-(255*((val-((max/4.0)))/(max/4.0))));
			painter->fillRect((m_itemWidth*cellx)+2,(m_itemHeight*celly)+2,m_itemWidth-2,m_itemHeight-2,bgcolor);
		}
		else if (val < ((max/4.0)*3))
		{
			QColor bgcolor = QColor::fromRgb((255*((val-((max/4.0)*2))/(max/4.0))),255,0);
			painter->fillRect((m_itemWidth*cellx)+2,(m_itemHeight*celly)+2,m_itemWidth-2,m_itemHeight-2,bgcolor);
		}
		else
		{
			QColor bgcolor = QColor::fromRgb(255,255-(255*((val-((max/4.0)*3))/(max/4.0))),0);
			painter->fillRect((m_itemWidth*cellx)+2,(m_itemHeight*celly)+2,m_itemWidth-2,m_itemHeight-2,bgcolor);
		}
	}


	QFont font = painter->font();
	font.setPixelSize(m_itemHeight*0.7);
	painter->setFont(font);
	painter->setPen(pen);
	int width = painter->fontMetrics().width(text);
	if (width > m_itemWidth)
	{

	}
	painter->drawText(((cellx)*m_itemWidth) + (m_itemWidth/2.0) - (width / 2.0),(celly)*m_itemHeight + ((m_itemHeight/2.0)-2) + (painter->fontMetrics().height()/2.0),text);
	painter->setPen(oldpen);
}

void TableViewNew3D::paintEvent (QPaintEvent *evt)
{
	Q_UNUSED(evt)
	QPainter painter(this);
	painter.setPen(QColor::fromRgb(0,0,0));
	painter.drawRect(0,0,width()-1,height()-1);
	//item width = 40
	//item height = 20
	double m_currentTrace = 0;
	double drawTraceY = 0;
	bool foundy = false;

	double drawTraceX = 0;
	bool foundx = false;

	for (int y=0;y<m_rowCount;y++)
	{
		if (currentCell.x() == -1 && currentCell.y() == y)
		{
			if (m_inEdit)
			{
				drawCell(&painter,0,y,m_editText,false,true);
			}
			else
			{
				drawCell(&painter,0,y,yaxis.at(y),false,true);
			}
		}
		else
		{
			drawCell(&painter,0,y,yaxis.at(y),false,false);
		}
		painter.setPen(QColor::fromRgb(0,0,0));
		if (m_traceEnabled)
		{
			m_currentTrace = yaxis.at(y).toDouble();
			if (m_traceY < m_currentTrace)
			{
				if (y == m_rowCount-1)
				{
					double curr = yaxis.at(y).toDouble();
					double next = 0;
					if (m_traceY < next)
					{
						//The trace is below the line, limit it to the line
						drawTraceY = (y * m_itemHeight) + ((m_itemHeight));
						foundy = true;
					}
					else
					{
						//The trace is above the line
						double percent = (next - m_traceY) / (next - curr);
						drawTraceY = ((y * m_itemHeight) + (m_itemHeight)) - (percent * (m_itemHeight/2.0));
						foundy = true;

					}
				}
			}
			else if (!foundy)
			{
				double prev = 0;
				double lastY = 0;
				if (y == 0)
				{
					//Value is between the top and null values
					prev = yaxis.at(y).toDouble() - ((yaxis.at(y+1).toDouble() - yaxis.at(y).toDouble()) / 2.0);
					lastY = 0;
				}
				else
				{
					prev = yaxis.at(y-1).toDouble();
					lastY = (y-1)*m_itemHeight + ((m_itemHeight/2.0)-2);
				}
				//Between the current trace and the last one, we have our value

				double diff = prev - m_currentTrace;
				double percent =(prev - m_traceY) / diff;
				double currentY = (y)*m_itemHeight + ((m_itemHeight/2.0)-2);
				drawTraceY = (lastY + (percent * (currentY - lastY)));
				if (drawTraceY < 0)
				{
					//Cap it at 0
					drawTraceY = 0;
				}
				foundy = true;
			}
		}
	}
	for (int x=0;x<m_columnCount;x++)
	{
		if (currentCell.y() == -1 && currentCell.x() == x)
		{
			if (m_inEdit)
			{
				drawCell(&painter,x+1,m_rowCount,m_editText,false,true);
			}
			else
			{
				drawCell(&painter,x+1,m_rowCount,xaxis.at(x),false,true);
			}
		}
		else
		{
			drawCell(&painter,x+1,m_rowCount,xaxis.at(x),false,false);
		}
		painter.setPen(QColor::fromRgb(0,0,0));

		if (m_traceEnabled)
		{
			m_currentTrace = xaxis.at(x).toDouble();
			if (m_traceX > m_currentTrace)
			{
				if (x == m_columnCount-1)
				{
					double prev = xaxis.at(x-1).toDouble();
					double curr = xaxis.at(x).toDouble();
					double next = curr + ((curr - prev) / 2.0);
					if (m_traceX > next)
					{
						//Trace is at or beyond the end
						drawTraceX = m_itemWidth + (x * m_itemWidth) + (m_itemWidth);
						foundx = true;
					}
					else
					{
						double percent = (m_traceX - curr) / (next - curr);
						drawTraceX = (((x+1) * m_itemWidth) + (m_itemWidth / 2.0)) + (percent * (m_itemWidth/2.0));
						foundx = true;
					}
				}
			}
			else if (!foundx)
			{
				double prev = 0;
				double lastx;
				if (x == 0)
				{
					//Value is between the top and null values
					//prev = xaxis.at(x).toDouble() - ((xaxis.at(x+1).toDouble() - xaxis.at(x).toDouble()));
					prev = 0;
					lastx = m_itemWidth;
				}
				else
				{
					prev = xaxis.at(x-1).toDouble();
					lastx = m_itemWidth + ((x-1) * m_itemWidth) + (m_itemWidth / 2.0);
				}
				//Between the current trace and the last one, we have our value

				double diff = m_currentTrace - prev;
				double percent =(m_traceX - prev) / diff;
				//Percent is a 0.0-1.0 of where the trace should lie,between i-1, and i;
				double currentX = m_itemWidth + ((x)*m_itemWidth) + ((m_itemWidth/2.0));
				drawTraceX = (lastx + (percent * (currentX - lastx)));
				if (drawTraceX < m_itemWidth)
				{
					drawTraceX = m_itemWidth;
				}
				foundx = true;
			}
		}


	}

	for (int y=0;y<m_rowCount;y++)
	{
		for (int x=0;x<m_columnCount;x++)
		{
			if (m_minSelectedRow <= y && m_maxSelectedRow >= y && m_minSelectedColumn <= x && m_maxSelectedColumn >= x)
			{
				if (m_inEdit)
				{
					drawCell(&painter,x+1,y,m_editText,true,true);
				}
				else
				{
					drawCell(&painter,x+1,y,values.at(y).at(x),true,true);
				}
			}
			else if (currentCell.y() == y && currentCell.x() == x)
			{
				if (m_inEdit)
				{
					drawCell(&painter,x+1,y,m_editText,true,true);
				}
				else
				{
					drawCell(&painter,x+1,y,values.at(y).at(x),true,true);
				}
			}
			else
			{
				drawCell(&painter,x+1,y,values.at(y).at(x),false,false);
			}
		}
	}
	if (foundy && foundx && m_traceEnabled)
	{
		QPen pen = painter.pen();
		pen.setWidth(6);
		pen.setColor(QColor::fromRgb(0,0,0));
		painter.setPen(pen);
		painter.drawLine(0,drawTraceY,drawTraceX - (m_itemWidth/2),drawTraceY);
		painter.drawLine(drawTraceX + (m_itemWidth/2.0),drawTraceY,width(),drawTraceY);
		painter.drawLine(drawTraceX,0,drawTraceX,drawTraceY - (m_itemHeight/2.0));
		painter.drawLine(drawTraceX,drawTraceY + (m_itemHeight/2.0),drawTraceX,height());
		painter.drawRect(drawTraceX-(m_itemWidth/2.0),drawTraceY-(m_itemHeight/2.0),m_itemWidth,m_itemHeight);

		pen.setWidth(4);
		pen.setColor(QColor::fromRgb(160,32,240));
		painter.setPen(pen);
		painter.drawLine(0,drawTraceY,drawTraceX - (m_itemWidth/2),drawTraceY);
		painter.drawLine(drawTraceX + (m_itemWidth/2.0),drawTraceY,width(),drawTraceY);
		painter.drawLine(drawTraceX,0,drawTraceX,drawTraceY - (m_itemHeight/2.0));
		painter.drawLine(drawTraceX,drawTraceY + (m_itemHeight/2.0),drawTraceX,height());
		painter.drawRect(drawTraceX-(m_itemWidth/2.0),drawTraceY-(m_itemHeight/2.0),m_itemWidth,m_itemHeight);
	}
}

void TableViewNew3D::mouseMoveEvent(QMouseEvent *evt)
{
	m_x = evt->x() / m_itemWidth;
	m_y = evt->y() / m_itemHeight;
	update();
}
void TableViewNew3D::mousePressEvent(QMouseEvent *evt)
{
	m_x = evt->x() / m_itemWidth;
	m_y = evt->y() / m_itemHeight;
	if (m_y == m_rowCount)
	{
		m_y = -1;
	}
	QPoint oldpoint = currentCell;
	currentCell.setX(m_x-1);
	currentCell.setY(m_y);

	if (evt->modifiers() & Qt::ShiftModifier && currentCell.x() != -1 && currentCell.y() != -1)
	{
		if (oldpoint.x() == -1)
		{
			oldpoint.setX(0);
		}
		if (oldpoint.x() < currentCell.x())
		{
			m_minSelectedColumn = oldpoint.x();
			m_maxSelectedColumn = currentCell.x();
		}
		else
		{
			m_minSelectedColumn = currentCell.x();
			m_maxSelectedColumn = oldpoint.x();
		}
		if (oldpoint.y() == -1)
		{
			oldpoint.setY(m_rowCount-1);
		}
		if (oldpoint.y() < currentCell.y())
		{

			m_minSelectedRow = oldpoint.y();
			m_maxSelectedRow = currentCell.y();
		}
		else
		{
			m_minSelectedRow = currentCell.y();
			m_maxSelectedRow = oldpoint.y();
		}
	}
	else
	{
		if (currentCell.x() == -1)
		{
			m_minSelectedColumn = -1;
			m_maxSelectedColumn = -1;
		}
		else
		{
			m_minSelectedColumn = currentCell.x();
			m_maxSelectedColumn = currentCell.x();
		}
		if (currentCell.y() == -1)
		{
			m_minSelectedRow = -1;
			m_maxSelectedRow = -1;
		}
		else
		{
			m_minSelectedRow = currentCell.y();
			m_maxSelectedRow = currentCell.y();
		}

	}
	//if (currentCell)
	/*if (m_highlightList.size() > m_y)
	{
		if (m_highlightList.at(m_y).size() > m_x)
		{
			m_highlightList[m_y][m_x] = 1;
		}
	}*/

	update();
}
QList<QPoint> TableViewNew3D::selectedItems()
{
	QList<QPoint> l;
	for (int x=m_minSelectedColumn;x<=m_maxSelectedColumn;x++)
	{
		for (int y=m_minSelectedRow;y<=m_maxSelectedRow;y++)
		{
			l.append(QPoint(x,y));
		}
	}
	//l.append(currentCell);
	return l;
}
void TableViewNew3D::mouseReleaseEvent(QMouseEvent *evt)
{
	Q_UNUSED(evt)
}
void TableViewNew3D::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() >= Qt::Key_0 && evt->key() <= Qt::Key_9)
	{
		//It's a number key!
		if (!m_inEdit)
		{
			m_inEdit = true;
		}

		m_editText += QString::number(evt->key()-0x30,'f',0);
		update();
	}
	else if (evt->key() == Qt::Key_Period)
	{
		if (!m_inEdit)
		{
			m_inEdit = true;
			m_editText = "0";
		}

		m_editText += ".";
		update();
	}
	else if (evt->key() == Qt::Key_Backspace)
	{
		if (m_inEdit)
		{
			m_editText = m_editText.mid(0,m_editText.length()-1);
			update();
		}
	}
	else if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
	{
		if (m_inEdit)
		{
			m_inEdit = false;
			//emit itemChangeRequest(currentCell.y(),currentCell.x(),m_editText);
			emit itemChangeRequest(m_minSelectedRow,m_maxSelectedRow,m_minSelectedColumn,m_maxSelectedColumn,m_editText);
			m_editText = "";
			update();
		}
	}
	else if (evt->key() == Qt::Key_Escape)
	{
		if (m_inEdit)
		{
			m_inEdit = false;
			m_editText = "";
			update();
		}
	}
	if (evt->key() == Qt::Key_Up)
	{
		if (currentCell.y() > 0 || currentCell.y() == -1)
		{
			if (m_inEdit)
			{
				m_inEdit = false;
				//emit itemChangeRequest(currentCell.y(),currentCell.x(),m_editText);
				emit itemChangeRequest(m_minSelectedRow,m_maxSelectedRow,m_minSelectedColumn,m_maxSelectedColumn,m_editText);
				m_editText = "";
			}
			if (currentCell.y() == -1)
			{
				currentCell.setY(m_rowCount-1);
				m_minSelectedRow = m_rowCount;
				m_maxSelectedRow = m_rowCount-1;
			}
			else
			{
				currentCell.setY(currentCell.y()-1);
			}
			if (evt->modifiers() & Qt::ShiftModifier && currentCell.x() != -1)
			{
				//Shift modifier, let's decrease m_minSelectedRow
				if (currentCell.y()+1 == m_minSelectedRow)
				{
					m_minSelectedRow--;
				}
				else
				{
					m_maxSelectedRow--;
				}
			}
			else
			{
				if (currentCell.x() == -1)
				{
					m_minSelectedColumn = -1;
					m_maxSelectedColumn = -1;
				}
				else
				{
					m_minSelectedColumn = currentCell.x();
					m_maxSelectedColumn = currentCell.x();
				}
				if (currentCell.y() == -1)
				{
					m_minSelectedRow = -1;
					m_maxSelectedRow = -1;
				}
				else
				{
					m_minSelectedRow = currentCell.y();
					m_maxSelectedRow = currentCell.y();
				}
			}
			m_selectionList.clear();
			m_selectionList.append(QPair<int,int>(currentCell.x(),currentCell.y()));
			emit currentSelectionChanged(m_selectionList);
			update();
			return;
		}
	}
	if (evt->key() == Qt::Key_Down)
	{
		if (currentCell.y() < m_rowCount && currentCell.y() != -1)
		{
			if (m_inEdit)
			{
				m_inEdit = false;
				//emit itemChangeRequest(currentCell.y(),currentCell.x(),m_editText);
				emit itemChangeRequest(m_minSelectedRow,m_maxSelectedRow,m_minSelectedColumn,m_maxSelectedColumn,m_editText);
				m_editText = "";
			}
			if (currentCell.y() == m_rowCount-1)
			{
				currentCell.setY(-1);
				if (currentCell.x() == -1)
				{
					//We're at the bottom cell of the axis, flip over the other axis
					currentCell.setX(0);
				}
			}
			else
			{
				currentCell.setY(currentCell.y()+1);
			}
			if (evt->modifiers() & Qt::ShiftModifier && currentCell.x() != -1 && currentCell.y() != -1)
			{
				//Shift modifier, let's decrease m_minSelectedRow
				if (currentCell.y()-1 == m_maxSelectedRow)
				{
					m_maxSelectedRow++;
				}
				else
				{
					m_minSelectedRow++;
				}
			}
			else
			{
				if (currentCell.x() == -1)
				{
					m_minSelectedColumn = -1;
					m_maxSelectedColumn = -1;
				}
				else
				{
					m_minSelectedColumn = currentCell.x();
					m_maxSelectedColumn = currentCell.x();
				}
				if (currentCell.y() == -1)
				{
					m_minSelectedRow = -1;
					m_maxSelectedRow = -1;
				}
				else
				{
					m_minSelectedRow = currentCell.y();
					m_maxSelectedRow = currentCell.y();
				}
			}
			m_selectionList.clear();
			m_selectionList.append(QPair<int,int>(currentCell.x(),currentCell.y()));
			emit currentSelectionChanged(m_selectionList);
			update();
			return;
		}
	}
	if (evt->key() == Qt::Key_Left)
	{
		if (currentCell.x() >= 0)
		{
			if (m_inEdit)
			{
				m_inEdit = false;
//				emit itemChangeRequest(currentCell.y(),currentCell.x(),m_editText);
				emit itemChangeRequest(m_minSelectedRow,m_maxSelectedRow,m_minSelectedColumn,m_maxSelectedColumn,m_editText);
				m_editText = "";
			}
			if (currentCell.x() == 0 && currentCell.y() == -1)
			{
				//At the leftmost axis cell, flip to the other axis
				currentCell.setY(m_rowCount-1);
			}
			currentCell.setX(currentCell.x()-1);
			if (evt->modifiers() & Qt::ShiftModifier && currentCell.x() != -1  && currentCell.y() != -1)
			{
				//Shift modifier, let's decrease m_minSelectedRow
				if (currentCell.x()+1 == m_minSelectedColumn)
				{
					m_minSelectedColumn--;
				}
				else
				{
					m_maxSelectedColumn--;
				}
			}
			else
			{
				if (currentCell.x() == -1)
				{
					m_minSelectedColumn = -1;
					m_maxSelectedColumn = -1;
				}
				else
				{
					m_minSelectedColumn = currentCell.x();
					m_maxSelectedColumn = currentCell.x();
				}
				if (currentCell.y() == -1)
				{
					m_minSelectedRow = -1;
					m_maxSelectedRow = -1;
				}
				else
				{
					m_minSelectedRow = currentCell.y();
					m_maxSelectedRow = currentCell.y();
				}



			}

			m_selectionList.clear();
			m_selectionList.append(QPair<int,int>(currentCell.x(),currentCell.y()));
			emit currentSelectionChanged(m_selectionList);
			update();
			return;
		}
	}
	if (evt->key() == Qt::Key_Right)
	{
		if (currentCell.x() < m_columnCount-1)
		{
			if (m_inEdit)
			{
				m_inEdit = false;
				//emit itemChangeRequest(currentCell.y(),currentCell.x(),m_editText);
				emit itemChangeRequest(m_minSelectedRow,m_maxSelectedRow,m_minSelectedColumn,m_maxSelectedColumn,m_editText);
				m_editText = "";
			}
			if (currentCell.x() == -1)
			{
				m_minSelectedColumn = 0;
			}
			currentCell.setX(currentCell.x()+1);
			if (evt->modifiers() & Qt::ShiftModifier && currentCell.y() != -1)
			{
				//Shift modifier, let's decrease m_minSelectedRow
				if (currentCell.x()-1 == m_maxSelectedColumn )
				{
					m_maxSelectedColumn++;
				}
				else
				{
					m_minSelectedColumn++;
				}
			}
			else
			{
				if (currentCell.x() == -1)
				{
					m_minSelectedColumn = -1;
					m_maxSelectedColumn = -1;
				}
				else
				{
					m_minSelectedColumn = currentCell.x();
					m_maxSelectedColumn = currentCell.x();
				}
				if (currentCell.y() == -1)
				{
					m_minSelectedRow = -1;
					m_maxSelectedRow = -1;
				}
				else
				{
					m_minSelectedRow = currentCell.y();
					m_maxSelectedRow = currentCell.y();
				}
			}

			m_selectionList.clear();
			m_selectionList.append(QPair<int,int>(currentCell.x(),currentCell.y()));
			emit currentSelectionChanged(m_selectionList);
			update();
			return;
		}
	}
	if (m_inEdit)
	{
		return;
	}
	for (int i=0;i<m_hotkeyMap.size();i++)
	{
		if (m_hotkeyMap.at(i).first == evt->key())
		{
			if (m_hotkeyMap.at(i).second != Qt::NoModifier)
			{
				if (evt->modifiers() & m_hotkeyMap.at(i).second)
				{
					emit hotKeyPressed(evt->key(),m_hotkeyMap.at(i).second);
				}
			}
			else
			{
				emit hotKeyPressed(evt->key(),Qt::NoModifier);
			}
		}
	}
}
