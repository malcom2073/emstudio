/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "comsettings.h"
#include <QFileDialog>
#include <QColorDialog>
ComSettings::ComSettings(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.cancelPushButton,SIGNAL(clicked()),this,SIGNAL(cancelClicked()));
	connect(ui.savePushButton,SIGNAL(clicked()),this,SIGNAL(saveClicked()));
	connect(ui.datalogLocationBrowseButton,SIGNAL(clicked()),this,SLOT(browseLogFolderClicked()));
	connect(ui.chooseColorButton,SIGNAL(clicked(bool)),this,SLOT(chooseColorClicked()));
}

ComSettings::~ComSettings()
{
}
void ComSettings::setInterByteDelay(int delay)
{
	ui.interByteDelaySpinBox->setValue(delay);
}
void ComSettings::closeEvent(QCloseEvent *event)
{
	emit windowHiding((QMdiSubWindow*)this->parent());
	QWidget::closeEvent(event);
}

void ComSettings::browseLogFolderClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this,"Select Log Folder",".");
	if (dir == "")
	{
		return;
	}
	ui.datalogLocationLineEdit->setText(dir);
}

void ComSettings::setBaud(int baud)
{
	ui.baudRateLineEdit->setText(QString::number(baud));
}
void ComSettings::setComPort(QString port)
{
	ui.portNameLineEdit->setText(port);
}
QString ComSettings::getDataLogDir()
{
	return ui.datalogLocationLineEdit->text();
}

bool ComSettings::getSaveDataLogs()
{
	return (ui.saveDataLogCheckBox->checkState() == Qt::Checked);
}

bool ComSettings::getClearDataLogs()
{
	return (ui.clearDataLogsCheckBox->checkState() == Qt::Checked);
}

int ComSettings::getNumLogsToSave()
{
	return ui.logCountSpinBox->value();
}

void ComSettings::setDataLogDir(QString dir)
{
	ui.datalogLocationLineEdit->setText(dir);
}

void ComSettings::setSaveDataLogs(bool save)
{
	if (save)
	{
		ui.saveDataLogCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		ui.saveDataLogCheckBox->setCheckState(Qt::Unchecked);
	}
}

void ComSettings::setClearDataLogs(bool clear)
{
	if (clear)
	{
		ui.clearDataLogsCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		ui.clearDataLogsCheckBox->setCheckState(Qt::Unchecked);
	}
}

void ComSettings::setNumLogsToSave(int num)
{
	ui.logCountSpinBox->setValue(num);
}


int ComSettings::getInterByteDelay()
{
	return ui.interByteDelaySpinBox->value();
}
int ComSettings::getBaud()
{
	return ui.baudRateLineEdit->text().toInt();
}
QString ComSettings::getComPort()
{
	return ui.portNameLineEdit->text();
}
void ComSettings::chooseColorClicked()
{
	QColor color = QColorDialog::getColor();
	if (color.isValid())
	{
		ui.chooseColorButton->setStyleSheet(QString("background-color: ") + color.name());
	}
}
