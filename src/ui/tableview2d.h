/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TABLEVIEW2D_H
#define TABLEVIEW2D_H

#include <QWidget>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QClipboard>
#include <table2ddata.h>
#include <QPair>
#include "ui_tableview2d.h"
#include "dataview.h"
class TableView2D : public DataView
{
	Q_OBJECT
	
public:
	explicit TableView2D(QWidget *parent = 0);
	~TableView2D();
	//bool setData(unsigned short locationid,QByteArray data,TableData *newtableData=0);
	//bool setData(unsigned short locationid,QByteArray data,Table2DMetaData metadata,TableData *newtableData);
	//bool setData(unsigned short locationid,QByteArray rawdata);
	void setMetaData(Table2DMetaData metadata);
	bool setData(unsigned short locationid,DataBlock *data);
	bool setData(QString name,DataBlock *data);
	//void passData(unsigned short locationid,Table2DData data);
	void passDatalog(QVariantMap data);
private:
	bool m_isSignedData;
	bool m_isFlashOnly;
	bool m_tracingEnabled;
	bool metaDataValid;
	Table2DData *tableData;
	void setSilentValue(int row,int column,QString value);
	Table2DMetaData m_metaData;
	Ui::TableView2D ui;
	unsigned short m_locationid;
	int m_physicalid;
	double currentvalue;
	void resizeColumnWidths();
	int m_currRow;
	int m_currCol;
	void exportJson(QString filename);
	void setValue(int row, int column,double value);
	void writeTable(bool ram);
	QList<QPair<int,int> > m_highlightItemList;
	int m_oldXLoc;
	void setRange(QList<QPair<QPair<int,int>,double> > data);
	QString formatNumber(double num,int prec);
	QString verifyValue(int row,int column,QString item);
	void reColorTable(int rownum,int colnum);
	//QCPAxisRect *m_wideAxisRect;
protected:
	void resizeEvent(QResizeEvent *evt);
	void keyPressEvent(QKeyEvent *event);
public slots:
	bool updateTable();
private slots:
	void tracingCheckBoxStateChanged(int newstate);
	void exportClicked();
	void importClicked();
	void saveClicked();
	void loadFlashClicked();
	void loadRamClicked();
	void tableCellChanged(int row,int column);
	void tableCurrentCellChanged(int currentrow,int currentcolumn,int prevrow,int prevcolumn);
	void hotKeyPressed(int key,Qt::KeyboardModifiers modifier);
signals:
	void saveToFlash(unsigned short locationid);
	void saveData(unsigned short locationid,QByteArray data,int phyiscallocation);
	void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
	void reloadTableData(unsigned short locationid,bool ram);
};

#endif // TABLEVIEW2D_H
