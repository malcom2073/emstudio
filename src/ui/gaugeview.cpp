/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "gaugeview.h"
#include <QMdiSubWindow>

GaugeView::GaugeView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	//widget = new GaugeWidget(this);
	m_gaugeWidget = new GaugeWidget();
	widget = QWidget::createWindowContainer(m_gaugeWidget,this);
	widget->setGeometry(0,0,1200,700);
	widget->show();

	guiUpdateTimer = new QTimer(this);
	connect(guiUpdateTimer,SIGNAL(timeout()),this,SLOT(guiUpdateTimerTick()));
	guiUpdateTimer->start(250);
}

GaugeView::~GaugeView()
{
}
void GaugeView::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
	emit windowHiding((QMdiSubWindow*)this->parent());
}
void GaugeView::passData(QVariantMap data)
{
	m_valueMap = data;
}
QString GaugeView::setFile(QString file)
{
	QString result = m_gaugeWidget->setFile(file);
	propertiesInUse = m_gaugeWidget->getPropertiesInUse();
	return result;
}

void GaugeView::passDecoder(DataPacketDecoder *decoder)
{
	Q_UNUSED(decoder)
}

void GaugeView::guiUpdateTimerTick()
{

	QVariantMap::const_iterator i = m_valueMap.constBegin();
	while (i != m_valueMap.constEnd())
	{
		if (propertiesInUse.contains(i.key()))
		{
			m_gaugeWidget->propertyMap.setProperty(i.key().toLatin1(),i.value());
		}
		//ui.tableWidget->item(m_nameToIndexMap[i.key()],1)->setText(QString::number(i.value()));
		i++;
	}

}
