/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef RAWDATAVIEW_H
#define RAWDATAVIEW_H

#include <QWidget>
#include "rawdata.h"
#include "dataview.h"
#include "ui_rawdataview.h"

class RawDataView : public DataView
{
	Q_OBJECT
	
public:
	explicit RawDataView(bool isram, bool isflash,QWidget *parent = 0);
	~RawDataView();
	bool setData(unsigned short locationid,DataBlock *tdata);
	//void verifyData(unsigned short locationid,QByteArray data);
	void passDatalog(QVariantMap data);
private:
	bool m_isRam;
	Ui::RawDataView ui;
	unsigned short m_locationId;
	RawData *m_data;

private slots:
	void saveFlashButtonClicked();
	void saveRamButtonClicked();
	void loadRamButtonClicked();
	void loadFlashButtonClicked();
	void update();
signals:
	void saveData(unsigned short locationid,QByteArray data,int physicallocation);
	void reloadData(unsigned short locationid,bool isram);
};

#endif // RAWDATAVIEW_H
