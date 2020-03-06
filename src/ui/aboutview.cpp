/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "aboutview.h"


AboutView::AboutView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
}

AboutView::~AboutView()
{
}
void AboutView::setVersionLabel(QString version)
{
	ui.versionLabel->setText(version);
}

void AboutView::setHash(QString hash)
{
	m_hash = hash;
	ui.commitHashLineEdit->setText(hash);
}
void AboutView::setDate(QString date)
{
	ui.dateLineEdit->setText(date);
}
void AboutView::setCommit(QString commit)
{
	m_commit = commit;
	ui.commitIdLineEdit->setText(commit);
}
void AboutView::closeEvent(QCloseEvent *event)
{
	event->ignore();
	((QMdiSubWindow*)this->parent())->hide();
	emit windowHiding((QMdiSubWindow*)this->parent());
}
