/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "rawdataview.h"
#include "QsLog.h"
#include <QMessageBox>
RawDataView::RawDataView(bool isram, bool isflash,QWidget *parent)
{
	Q_UNUSED(parent)
	m_data = 0;
	ui.setupUi(this);
	connect(ui.saveFlashPushButton,SIGNAL(clicked()),this,SLOT(saveFlashButtonClicked()));
	connect(ui.saveRamPushButton,SIGNAL(clicked()),this,SLOT(saveRamButtonClicked()));
	connect(ui.loadFlashPushButton,SIGNAL(clicked()),this,SLOT(loadFlashButtonClicked()));
	connect(ui.loadRamPushButton,SIGNAL(clicked()),this,SLOT(loadRamButtonClicked()));
	if (!isram)
	{
		//Is only flash
		ui.saveRamPushButton->setEnabled(false);
		ui.saveRamPushButton->setVisible(false);
		ui.loadRamPushButton->setEnabled(false);
		ui.loadRamPushButton->setVisible(false);
	}
	else if (!isflash)
	{
		//Is only ram
		ui.saveFlashPushButton->setEnabled(false);
		ui.saveFlashPushButton->setVisible(false);
		ui.loadFlashPushButton->setEnabled(false);
		ui.loadFlashPushButton->setVisible(false);
	}
	else
	{
		//Is both ram and flash, leave both sets of buttons enabled.
	}
}
void RawDataView::passDatalog(QVariantMap data)
{
	Q_UNUSED(data)
}
bool RawDataView::setData(unsigned short locationid,DataBlock *data)
{
	if (m_data)
	{
		disconnect(m_data,SIGNAL(update()),this,SLOT(update()));
	}
	m_data = (RawData*)(data);
	connect(m_data,SIGNAL(update()),this,SLOT(update()));
	m_locationId = locationid;
//	ui.hexEditor->setData(m_data->data());
	QHexDocument* document = QHexDocument::fromMemory(m_data->data());
	ui.hexEditor->setDocument(document);
	ui.locationIdLabel->setText("0x" + QString::number(locationid,16).toUpper());
	//We know that every 20 bytes is a line.
	//int lines = m_data->data().size() / 20;
	//Assume each line is 30 pixels?;
	//this->setMinimumHeight(lines * 35 + 50);
	return true;
}
void RawDataView::update()
{
	QHexDocument* document = QHexDocument::fromMemory(m_data->data());
	ui.hexEditor->setDocument(document);
}

void RawDataView::loadRamButtonClicked()
{
	if (QMessageBox::information(0,"Warning","Doing this will reload the block from ram, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		QLOG_DEBUG() << "Ok";
		m_data->updateFromRam();
		//emit reloadData(m_locationId,true);
	}
	else
	{
		QLOG_DEBUG() << "Not ok";
	}

}

void RawDataView::loadFlashButtonClicked()
{
	if (QMessageBox::information(0,"Warning","Doing this will reload the block from flash, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		QLOG_DEBUG() << "Ok";
		//emit reloadData(m_locationId,false);
		m_data->updateFromFlash();
	}
	else
	{
		QLOG_DEBUG() << "Not ok";
	}
}

RawDataView::~RawDataView()
{
	ui.hexEditor->document()->deleteLater();
}
void RawDataView::saveFlashButtonClicked()
{
	//emit saveData(m_locationId,ui.hexEditor->data(),1); //0 for RAM, 1 for flash.
//	m_data->updateValue(ui.hexEditor->data());
	m_data->updateValue(ui.hexEditor->document()->read(0));
}

void RawDataView::saveRamButtonClicked()
{
	//emit saveData(m_locationId,ui.hexEditor->data(),0); //0 for RAM, 1 for flash.
}
