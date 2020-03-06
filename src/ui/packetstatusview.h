/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef PACKETSTATUSVIEW_H
#define PACKETSTATUSVIEW_H

#include <QWidget>
#include <QMdiSubWindow>
#include "ui_packetstatusview.h"
class PacketStatusView : public QWidget
{
	Q_OBJECT
	
public:
	explicit PacketStatusView(QWidget *parent = 0);
	~PacketStatusView();
	void clear();
public slots:
	void passPacketSent(unsigned short locationid,QByteArray header,QByteArray payload);
	void passPacketAck(unsigned short locationid,QByteArray header,QByteArray payload);
	void passPacketNak(unsigned short locationid,QByteArray header,QByteArray payload,unsigned short errornum);
	void passDecoderFailure(QByteArray packet);
protected:
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
private:
	Ui::PacketStatusView ui;
signals:
	void windowHiding(QMdiSubWindow *parent);
};

#endif // PACKETSTATUSVIEW_H
