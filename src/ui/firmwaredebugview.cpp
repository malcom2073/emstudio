/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "firmwaredebugview.h"


FirmwareDebugView::FirmwareDebugView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.clearPushButton,SIGNAL(clicked()),this,SLOT(clearButtonClicked()));
}

FirmwareDebugView::~FirmwareDebugView()
{
}
void FirmwareDebugView::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
}
void FirmwareDebugView::firmwareMessage(QString text)
{
	ui.debugPlainTextEdit->moveCursor(QTextCursor::End);
	ui.debugPlainTextEdit->insertPlainText(text);
	ui.debugPlainTextEdit->moveCursor(QTextCursor::End);
	ui.debugPlainTextEdit->ensureCursorVisible();
}
void FirmwareDebugView::clearButtonClicked()
{
	ui.debugPlainTextEdit->clear();
}
