/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include <QDebug>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFileDialog>

#include <QSettings>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "egraph.h"
#include "qgraph.h"
#include "graphview.h"
#include "aboutdialog.h"
#include "scatterplotview.h"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_progressDialog=0;
//	mapView = 0;
	connect(ui->actionOpen_Megasquirt_Log,SIGNAL(triggered()),this,SLOT(loadMegasquirtLog()));
	connect(ui->actionOpen_LibreEMS_Log,SIGNAL(triggered()),this,SLOT(loadLibreLog()));
	connect(ui->actionOpen_Embedded_Lockers_Log,SIGNAL(triggered()),this,SLOT(loadEBLLog()));
	connect(ui->actionClose_Log,SIGNAL(triggered()),this,SLOT(fileCloseClicked()));
	connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(fileQuitClicked()));
	connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutMenuClicked()));
	connect(ui->actionExport_as_CSV,SIGNAL(triggered()),this,SLOT(exportCsvClicked()));
    //graph = new QGraph(this);
    //this->setCentralWidget(graph);
	//graph->setGeometry(100,100,800,600);
    //graph->show();
    //dataSelectionScreen = new DataSelectionScreen();
    //connect(dataSelectionScreen,SIGNAL(itemEnabled(QString)),this,SLOT(itemEnabled(QString)));
    //connect(dataSelectionScreen,SIGNAL(itemDisabled(QString)),this,SLOT(itemDisabled(QString)));
    //connect(ui->actionSelect_Dialog,SIGNAL(triggered()),this,SLOT(selectDialogClicked()));

    //dataSelectionScreen->setVisible(false);
	this->setAttribute(Qt::WA_DeleteOnClose,true);

    this->menuBar()->setNativeMenuBar(false);

//    connect(ui->addGraphButton,SIGNAL(clicked()),this,SLOT(addGraphButtonClicked()));
 //   connect(ui->addMapButton,SIGNAL(clicked()),this,SLOT(addMapButtonClicked()));
    m_logLoaded = false;



}

void MainWindow::addGraphButtonClicked()
{
    if (!m_logLoaded)
    {
        QMessageBox::information(0,"Error","Please load a log first");
        return;
    }
    GraphView *view = new GraphView();
    view->show();
    view->passData(&variantList,m_logTypes);
    m_graphViewList.append(view);
}
void MainWindow::addMapButtonClicked()
{
	ScatterPlotView *view = new ScatterPlotView();
	view->setData(&variantList,m_logTypes);
	view->show();
	return;
	if (!m_logLoaded)
	{
		QMessageBox::information(0,"Error","Please load a log first");
		return;
	}
	MapView *mapView = new MapView();
	mapView->show();
	mapView->setGeometry(100,100,800,600);
	mapView->setData(&variantList,m_logTypes);
	m_mapViewList.append(mapView);
}

void MainWindow::logProgress(quint64 pos,quint64 total)
{
	m_progressDialog->setValue(100.0 * ((double)pos / (double)total));
}

MainWindow::~MainWindow()
{
	//delete ui;
	//delete dataSelectionScreen;
	if (m_progressDialog)
	{
		delete m_progressDialog;
	}
	if (m_logLoaded)
	{
		//m_progressDialog = new QProgressDialog("Closing...","Cancel",0,100);
		//m_progressDialog->show();
		//QApplication::instance()->processEvents();
		//fileCloseClicked();
	}
}
void MainWindow::payloadDecoded(QVariantMap map)
{
	if (variantList.size() == 0)
	{
		//First incoming map.
	}
	variantList.append(map);
}

void MainWindow::threadDone()
{
	m_progressDialog->hide();
	delete m_progressDialog;
	m_progressDialog = 0;
	qDebug() << variantList.size() << "records loaded";
	m_logLoaded = true;
	//sender()->deleteLater();
	FEMSParser *parser = qobject_cast<FEMSParser*>(sender());
	if (parser)
	{
		//It's a FEMS parser that sent this
		m_logTypes = parser->getLogTypes();
	}

}
void MainWindow::loadMegasquirtLog  ()
{
	QString filename = QFileDialog::getOpenFileName(this,"Select log file to open");
	if (filename == "")
	{
		return;
	}
	fileCloseClicked();
	MSParser *parser = new MSParser(this);
	connect(parser,SIGNAL(done()),this,SLOT(threadDone()));
	connect(parser,SIGNAL(loadProgress(quint64,quint64)),this,SLOT(logProgress(quint64,quint64)));
	connect(parser,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	connect(parser,SIGNAL(logData(QString,QString)),this,SLOT(logData(QString,QString)));
	m_progressDialog = new QProgressDialog("Loading file....","Cancel",0,100);
	m_progressDialog->show();
	parser->loadLog(filename);
}

void MainWindow::loadLibreLog()
{
	QString filename = QFileDialog::getOpenFileName(this,"Select log file to open",QString(),QString(),0,QFileDialog::DontUseCustomDirectoryIcons);
	if (filename == "")
	{
		return;
	}
	fileCloseClicked();
	FEMSParser *parser = new FEMSParser(this);
	connect(parser,SIGNAL(done()),this,SLOT(threadDone()));
	connect(parser,SIGNAL(loadProgress(quint64,quint64)),this,SLOT(logProgress(quint64,quint64)));
	connect(parser,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	connect(parser,SIGNAL(logData(QString,QString)),this,SLOT(logData(QString,QString)));
	m_progressDialog = new QProgressDialog("Loading file....","Cancel",0,100);
	m_progressDialog->show();
	parser->loadLog(filename);
}

void MainWindow::loadEBLLog()
{
	QString filename = QFileDialog::getOpenFileName(this,"Select log file to open");
	if (filename == "")
	{
		return;
	}
	fileCloseClicked();
	EBLParser *parser = new EBLParser(this);
	connect(parser,SIGNAL(done()),this,SLOT(threadDone()));
	connect(parser,SIGNAL(loadProgress(quint64,quint64)),this,SLOT(logProgress(quint64,quint64)));
	connect(parser,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	connect(parser,SIGNAL(logData(QString,QString)),this,SLOT(logData(QString,QString)));
	m_progressDialog = new QProgressDialog("Loading file....","Cancel",0,100);
	m_progressDialog->show();
	parser->loadLog(filename);
}

void MainWindow::fileCloseClicked()
{
	variantList.clear();
	for (int i=0;i<m_mapViewList.size();i++)
	{
		delete m_mapViewList.at(i);
	}
	m_mapViewList.clear();
	for (int i=0;i<m_graphViewList.size();i++)
	{
		delete m_graphViewList.at(i);
	}
	m_graphViewList.clear();
	m_logLoaded = false;
}

void MainWindow::fileQuitClicked()
{
    this->close();
}
void MainWindow::logData(QString key,QString val)
{
    //ui->textBrowser->append(key + ": " + val);
}

void MainWindow::aboutMenuClicked()
{
    AboutDialog *d = new AboutDialog(this);
    d->show();
}
void MainWindow::exportCsvClicked()
{
	QString filename = QFileDialog::getSaveFileName(this);
	if (filename == "")
	{
		return;
	}
	m_csvExporter = new Exporter_Csv(this);
	connect(m_csvExporter,SIGNAL(progress(int,int)),this,SLOT(exportProgress(int,int)));
	connect(m_csvExporter,SIGNAL(done()),this,SLOT(exportThreadDone()));
	m_csvExporter->startExport(filename,m_logTypes,variantList);
	m_progressDialog = new QProgressDialog("Exporting....","Cancel",0,100,this);
	m_progressDialog->show();


}
void MainWindow::exportProgress(int pos,int total)
{
	m_progressDialog->setValue(100.0 * ((double)pos / (double)total));
}
void MainWindow::exportThreadDone()
{
	m_progressDialog->hide();
	delete m_progressDialog;
	m_progressDialog = 0;
}
