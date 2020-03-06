#include "mapview.h"
#include "ui_mapview.h"
#include <QDebug>
MapView::MapView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MapView)
{
	ui->setupUi(this);
	connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(goButtonClicked()));
}

MapView::~MapView()
{
	delete ui;
}
void MapView::addItem(QString item)
{
	ui->comboBox->addItem(item);
	ui->comboBox_2->addItem(item);
	ui->comboBox_3->addItem(item);
}
void MapView::goButtonClicked()
{
	if (m_data->size() > 0)
	{
		if (m_data->at(0).contains(ui->comboBox->currentText()) && m_data->at(0).contains(ui->comboBox_2->currentText()) && m_data->at(0).contains(ui->comboBox_3->currentText()))
		{
			QList<double> x;
			QList<double> y;
			QList<double> z;
			for (int i=0;i<m_data->size();i++)
			{
				x.append(m_data->at(i).value(ui->comboBox->currentText()).toDouble());
				y.append(m_data->at(i).value(ui->comboBox_2->currentText()).toDouble());
				z.append(m_data->at(i).value(ui->comboBox_3->currentText()).toDouble());
			}
			ui->widget->passData(x,y,z);
		}
		else
		{
			qDebug() << "ERROR!";
		}
	}

}
void MapView::setData(QList<QVariantMap> *data)
{
	m_data = data;
}
