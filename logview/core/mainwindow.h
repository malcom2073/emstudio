#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariantMap>
#include <QScrollArea>
#include <QProgressDialog>
#include "dataselectionscreen.h"
#include "qgraph.h"
#include "mapview.h"
#include "emslogviewplugin.h"
#include <QPluginLoader>

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
	QGraph *graph;
	DataSelectionScreen *dataSelectionScreen;
	QProgressDialog *m_progressDialog;
	MapView *mapView;
	EmsLogViewPlugin *m_currentPlugin;
	QMap<QAction*,QString> m_actionToFilenameMap;
	QString m_currentPluginFile;
	QString m_currentPluginName;
private slots:
	void actionTriggered(bool checked);
	void logProgress(quint64 pos,quint64 total);
	void payloadDecoded(QVariantMap map);
	void threadDone();
	void boxClicked(bool checked);
	void fileOpenClicked();
	void fileCloseClicked();
	void fileQuitClicked();
	void itemEnabled(QString name);
	void itemDisabled(QString name);
	void selectDialogClicked();
};

#endif // MAINWINDOW_H
