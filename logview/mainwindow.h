/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariantMap>
#include <QScrollArea>
#include <QProgressDialog>
#include "dataselectionscreen.h"
#include "qgraph.h"
#include "mapview.h"
#include "femsparser.h"
#include "msparser.h"
#include "eblparser.h"
#include <QPluginLoader>
#include "graphview.h"
#include "exporter_csv.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	QList<QVariantMap> variantList;
	QProgressDialog *m_progressDialog;
	EmsLogViewPlugin *m_currentPlugin;
	QMap<QAction*,QString> m_actionToFilenameMap;
	QString m_currentPluginFile;
	QString m_currentPluginName;
	QAction *femsAction;
	QAction *msAction;
	QList<MapView*> m_mapViewList;
	QList<GraphView*> m_graphViewList;
	QList<QPair<QString, QString> > m_logTypes;
	bool m_logLoaded;
	Exporter_Csv *m_csvExporter;
private slots:
	void logProgress(quint64 pos,quint64 total);
	void payloadDecoded(QVariantMap map);
	void threadDone();
	void fileCloseClicked();
	void loadMegasquirtLog();
	void loadLibreLog();
	void loadEBLLog();
	void fileQuitClicked();
	void addGraphButtonClicked();
	void addMapButtonClicked();
	void logData(QString key,QString val);
	void aboutMenuClicked();
	void exportCsvClicked();
	void exportProgress(int pos,int total);
	void exportThreadDone();
};

#endif // MAINWINDOW_H
