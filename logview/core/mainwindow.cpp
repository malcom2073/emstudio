#include <QDebug>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFileDialog>
#include <qjson/parser.h>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "egraph.h"
#include "qgraph.h"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_progressDialog=0;
	mapView = 0;
	connect(ui->actionOpen_Log,SIGNAL(triggered()),this,SLOT(fileOpenClicked()));
	connect(ui->actionClose_Log,SIGNAL(triggered()),this,SLOT(fileCloseClicked()));
	connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(fileQuitClicked()));
	graph = new QGraph(this);
	this->setCentralWidget(graph);
	//graph->setGeometry(100,100,800,600);
	graph->show();
	dataSelectionScreen = new DataSelectionScreen();
	connect(dataSelectionScreen,SIGNAL(itemEnabled(QString)),this,SLOT(itemEnabled(QString)));
	connect(dataSelectionScreen,SIGNAL(itemDisabled(QString)),this,SLOT(itemDisabled(QString)));
	connect(ui->actionSelect_Dialog,SIGNAL(triggered()),this,SLOT(selectDialogClicked()));

	dataSelectionScreen->setVisible(false);
	this->setAttribute(Qt::WA_DeleteOnClose,true);

	QFile pluginfile("plugins.json");
	pluginfile.open(QIODevice::ReadOnly);
	QJson::Parser parser;
	bool ok = false;
	QVariant result = parser.parse(pluginfile.readAll(),&ok);
	pluginfile.close();
	if (!ok)
	{
		qDebug() << "Unable to load plugin list:" << parser.errorString();
		return;
	}
	QVariantMap resultmap = result.toMap();
	QVariantList list = resultmap.value("plugins").toList();
	QSettings settings;
	QString last = settings.value("lastusedplugin").toString();
	for (int i=0;i<list.size();i++)
	{
		QVariantMap item = list.at(i).toMap();
		QString file = item.value("filename").toString();
		QString name = item.value("name").toString();
		QAction *action = new QAction(this);
		connect(action,SIGNAL(triggered(bool)),this,SLOT(actionTriggered(bool)));
		action->setCheckable(true);
		action->setText(name);
		if (last != "" && name == last)
		{
			action->setChecked(true);
			m_currentPluginName = name;
			m_currentPluginFile = file;
			this->setWindowTitle("EMSLogView - Plugin: " + m_currentPluginName);
		}
		m_actionToFilenameMap[action] = file;
		ui->menuActive_Plugin->addAction(action);
	}

}
void MainWindow::actionTriggered(bool checked)
{
	if (checked)
	{
		for (int i=0;i<ui->menuActive_Plugin->actions().size();i++)
		{
			if (ui->menuActive_Plugin->actions().at(i) != sender())
			{
				ui->menuActive_Plugin->actions().at(i)->setChecked(false);
			}
			else
			{
				if (m_actionToFilenameMap.contains(ui->menuActive_Plugin->actions().at(i)))
				{
					QSettings settings;
					settings.setValue("lastusedplugin",m_currentPluginName);
					settings.sync();
					m_currentPluginFile = m_actionToFilenameMap.value(ui->menuActive_Plugin->actions().at(i));
					m_currentPluginName = ui->menuActive_Plugin->actions().at(i)->text();
					this->setWindowTitle("EMSLogView - Plugin: " + m_currentPluginName);
				}
			}
		}
	}
	else
	{
		for (int i=0;i<ui->menuActive_Plugin->actions().size();i++)
		{
			if (ui->menuActive_Plugin->actions().at(i) == sender())
			{
				ui->menuActive_Plugin->actions().at(i)->setChecked(true);
			}
		}
	}
}

void MainWindow::logProgress(quint64 pos,quint64 total)
{
	m_progressDialog->setValue(100.0 * ((double)pos / (double)total));
}

MainWindow::~MainWindow()
{
	//delete ui;
	//delete dataSelectionScreen;
	if (dataSelectionScreen)
	{
		delete dataSelectionScreen;
	}
	if (m_progressDialog)
	{
		delete m_progressDialog;
	}
	if (mapView)
	{
		delete mapView;
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
void MainWindow::itemEnabled(QString name)
{
	QList<float> list;
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i][name].toFloat());
	}
	graph->addData(list,name);
}

void MainWindow::itemDisabled(QString name)
{
	graph->removeData(name);
}

void MainWindow::boxClicked(bool checked)
{
	QCheckBox *sendbox = qobject_cast<QCheckBox*>(sender());
	if (!sendbox)
	{
		return;
	}
	QString name = sendbox->text();
	if (checked)
	{
		QList<float> list;
		for (int i=0;i<variantList.size();i++)
		{
			list.append(variantList[i][name].toFloat());
		}
		graph->addData(list,name);
	}
	else
	{
		graph->removeData(name);
	}
}
void MainWindow::selectDialogClicked()
{
	dataSelectionScreen->show();
	QApplication::postEvent(dataSelectionScreen, new QEvent(QEvent::Show));
	QApplication::postEvent(dataSelectionScreen, new QEvent(QEvent::WindowActivate));
}

void MainWindow::threadDone()
{
	mapView = new MapView();

	m_progressDialog->hide();
	delete m_progressDialog;
	m_progressDialog = 0;
	qDebug() << variantList.size() << "records loaded";
	for (QVariantMap::const_iterator i=variantList[0].constBegin();i!=variantList[0].constEnd();i++)
	{
		dataSelectionScreen->addItem(i.key());
		mapView->addItem(i.key());
	}
	dataSelectionScreen->show();
	dataSelectionScreen->setGeometry(100,100,400,600);
	mapView->show();
	mapView->setGeometry(100,100,800,600);
	mapView->setData(&variantList);
}
void MainWindow::fileOpenClicked()
{
	QString filename = QFileDialog::getOpenFileName(this,"Select log file to open");
	if (filename == "")
	{
		return;
	}
	fileCloseClicked();
	//decoder = new FEDataPacketDecoder();
	QPluginLoader *loader = new QPluginLoader(this);
	loader->setFileName(m_currentPluginFile);
	if (!loader->load())
	{
		qDebug() << "Error loading plugin:" << loader->errorString();
		return;
	}
	m_currentPlugin = qobject_cast<EmsLogViewPlugin*>(loader->instance());

	//connect(loader,SIGNAL(incomingDatalogPacket(QByteArray)),decoder,SLOT(decodePayload(QByteArray)));
	connect(m_currentPlugin,SIGNAL(done()),this,SLOT(threadDone()));
	connect(m_currentPlugin,SIGNAL(loadProgress(quint64,quint64)),this,SLOT(logProgress(quint64,quint64)));
	//connect(decoder,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	connect(m_currentPlugin,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));

	m_progressDialog = new QProgressDialog("Loading file....","Cancel",0,100);
	m_progressDialog->show();
	m_currentPlugin->loadLog(filename);

}

void MainWindow::fileCloseClicked()
{
	graph->clear();
	if (mapView)
	{
		delete mapView;
		mapView = 0;
	}
	dataSelectionScreen->hide();
}

void MainWindow::fileQuitClicked()
{

}
