/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "tableview3d.h"
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QFile>
//#include "datafield.h"
#include "configview.h"
//#include "emsdata.h"
#include "memorymetadata.h"
#include "emscomms.h"
#include "configblock.h"
#include <QTimer>
#include "datapacketdecoder.h"
#include "comsettings.h"
#include "emsinfoview.h"
#include "tableview.h"
#include "rawdataview.h"
#include "gaugeview.h"
#include "flagview.h"
#include "packetstatusview.h"
#include "aboutview.h"
//#include "memorylocation.h"
#include "interrogateprogressview.h"
#include "firmwaremetadata.h"
#include "table2ddata.h"
#include "readonlyramview.h"
#include "emsstatus.h"
#include "tablemap3d.h"
#include "rawdata.h"
#include "serialportstatus.h"
#include <QPluginLoader>
#include "parameterview.h"
#include "ramdiffwindow.h"
#include "wizardview.h"
#include "firmwaredebugview.h"
class RawDataBlock
{
public:
	unsigned short locationid;
	QByteArray header;
	QByteArray data;
};

class Interrogation
{
public:
	QString firmwareVersion;
	QString interfaceVersion;
	QString compilerVersion;
	QString firmwareBuildDate;
	QString decoderName;
	QString maxPacketSize;
	QString operatingSystem;
	QString emstudioBuilDate;
	QString emstudioCommit;
	QString emstudioHash;
};


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void setDevice(QString dev);

	void connectToEms();
	//QString getSettingsFile() { return m_settingsFile; }
private:
	void closeEvent(QCloseEvent *evt);
	void loadWizards(QString dir);
	void loadDashboards(QString dir);
	QList<GaugeView*> m_dashboardList;
	QList<unsigned short> m_locationIdList;
	bool m_checkEmsDataInUse;
	bool m_offlineMode;
	QMap<unsigned short,MemoryLocationInfo> m_memoryInfoMap;
	Interrogation emsinfo;
	unsigned short m_currentRamLocationId;
	bool m_waitingForRamWriteConfirmation;
	unsigned short m_currentFlashLocationId;
	bool m_waitingForFlashWriteConfirmation;
//	QList<MemoryLocation*> m_tempMemoryList;
	QMap<unsigned short,QList<ConfigBlock> > m_configBlockMap;
	QMap<unsigned short,QMdiSubWindow*> m_table3DMapViewMap;
	QMap<unsigned short,TableMap3D*> m_table3DMapViewWidgetMap;
//	QList<ConfigData> m_configMetaData;
	MemoryMetaData *m_memoryMetaData;
	TableView *dataTables;
	GaugeView *dataGauges;
	EmsInfoView *emsInfo;
	FlagView *dataFlags;
	QString m_logFileName;
	PacketStatusView *packetStatus;
	AboutView *aboutView;
	EmsStatus *statusView;
	FirmwareMetaData *firmwareMetaData;
	FirmwareDebugView *firmwareDebugView;
	InterrogateProgressView *progressView;
	QList<int> interrogationSequenceList;
	QMap<unsigned short,QWidget*> m_rawDataView;
	QMap<unsigned short,ConfigView*> m_configDataView;
	QMdiSubWindow *tablesMdiWindow;
	QMdiSubWindow *firmwareMetaMdiWindow;
	QMdiSubWindow *interrogateProgressMdiWindow;
	QMdiSubWindow *emsMdiWindow;
	QMdiSubWindow *flagsMdiWindow;
	QMdiSubWindow *gaugesMdiWindow;
	QMdiSubWindow *packetStatusMdiWindow;
	QMdiSubWindow *aboutMdiWindow;
	QMdiSubWindow *emsStatusMdiWindow;
	QMdiSubWindow *firmwareDebugMdiWindow;
	RamDiffWindow *ramDiffWindow;
	QTimer *emsSilenceTimer;

	ParameterView *parameterView;
	QMdiSubWindow *parameterMdiWindow;

	//QFile *settingsFile;
	void checkMessageCounters(int sequencenumber);
	DataPacketDecoder *dataPacketDecoder;
	void populateDataFields();
	bool m_emsSilenceLabelIsRed;

	Ui::MainWindow ui;
	QString m_pluginFileName;
	QPluginLoader *pluginLoader;
	EmsComms *emsComms;
	EmsComms *m_emsCommsOffline;
	int pidcount;
	QTimer *timer;
	QTimer *guiUpdateTimer;
	QString m_comPort;
	int m_comBaud;
	int m_comInterByte;
	bool m_saveLogs;
	bool m_clearLogs;
	int m_logsToKeep;
	QString m_logDirectory;
	QString m_firmwareVersion;
	QString m_interfaceVersion;
	QFile *logfile;
	QList<QMdiSubWindow*> m_wizardList;
	bool m_debugLogs;
	void createView(unsigned short locid,FormatType type);
	int m_interrogationFailureCount;
	QList<int> m_locIdMsgList;
	QList<int> m_locIdInfoMsgList;
	void checkRamFlashSync();
	bool m_interrogationInProgress;
	//EmsData *emsData;
	//EmsData *checkEmsData;
	int m_currentEcuClock;
	QMap<QMdiSubWindow*,QAction*> m_mdiSubWindowToActionMap;
	bool m_EcuResetPopup;
	QMap<QString,QList<QAction*> > m_gaugeActionMap;
	qint64 m_emsSilentLastTime;
private slots:
	void showTable(QString table);
	void windowHidden(QMdiSubWindow* window);
	void bringToFrontAndShow();
	void windowDestroyed(QObject *window);
	void interrogationData(QMap<QString,QString> datamap);
	void emsCommsSilence(qint64 lasttime);
	void emsCommsSilenceBroken();
	void locationIdInfo(unsigned short locationid,MemoryLocationInfo info);
	void tableview3d_show3DTable(unsigned short locationid,Table3DData *data);
	void emsStatusHardResetRequested();
	void emsStatusSoftResetRequested();
	void tableMap3DDestroyed(QObject *object);
	void interrogateProgressViewDestroyed(QObject *object);
	void emsOperatingSystem(QString os);
	void emsDecoderName(QString name);
	void emsFirmwareBuildDate(QString date);
	void emsCommsDisconnected();
	void emsCompilerVersion(QString version);
	void checkSyncRequest();
	void rawDataViewDestroyed(QObject *object);
	void emsInfoDisplayLocationId(int locid,bool isram,FormatType type);
	void menu_file_saveOfflineDataClicked();
	void menu_file_loadOfflineDataClicked();
	void menu_windows_GaugesClicked();
	void menu_windows_EmsStatusClicked();
	void menu_windows_EmsInfoClicked();
	void menu_windows_TablesClicked();
	void menu_windows_FlagsClicked();
	void menu_windows_PacketStatusClicked();
	void menu_windows_interrogateProgressViewClicked();
	void menu_windows_ParameterViewClicked();
	void menu_windows_firmwareMetadataClicked();
	void menu_windows_firmwareDebugClicked();
	void menu_settingsClicked();
	void menu_connectClicked();
	void menu_aboutClicked();
	void menu_editPluginManager();
	void menu_enableDatalogsClicked();
	void menu_disableDatalogsClicked();
	void interrogateProgressViewCancelClicked();
	void ui_saveDataButtonClicked();
	void menu_disconnectClicked();
	void settingsSaveClicked();
	void settingsCancelClicked();
	void guiUpdateTimerTick();
	void timerTick();
	void connectButtonClicked();
	void dataLogDecoded(QVariantMap data);
	void logPayloadReceived(QByteArray header,QByteArray payload);
	void logProgress(qlonglong current,qlonglong total);
	void logFinished();
	void loadLogButtonClicked();
	void playLogButtonClicked();
	void pauseLogButtonClicked();
	void stopLogButtonClicked();
	void emsCommsConnected();
	void unknownPacket(QByteArray header,QByteArray payload);
	void interfaceVersion(QString version);
	void ecuResetDetected(int missedpackets);
	void firmwareVersion(QString version);
	void error(QString msg);
	void error(SerialPortStatus error,QString msg);
	void commandSuccessful(int sequencenumber);
	void commandFailed(int sequencenumber,unsigned short errornum);
	void commandTimedOut(int sequencenumber);
	void interByteDelayChanged(int num);
	void retrieveRamLocationId(unsigned short locationid);
	void retrieveFlashLocationId(unsigned short locationid);
	void interrogationProgress(int current, int total);
	void interrogationComplete();
	void interrogateTaskStart(QString task, int sequence);
	void interrogateTaskSucceed(int sequence);
	void interrogateTaskFail(int sequence);
	void locationIdList(QList<unsigned short> idlist);
	void subMdiWindowActivated(QMdiSubWindow* window);
	void emsMemoryDirty();
	void emsMemoryClean();
	void datalogDescriptor(QString data);
	void ramLocationDirty(unsigned short locationid);
	void flashLocationDirty(unsigned short locationid);
	void dirtyRamAcceptLocalChanges();
	void dirtyRamRejectLocalChanges();
	void emsCommsSilenceTimerTick();
	void deviceRamAcceptChanges();
	void deviceRamIgnoreChanges();

	void deviceRamLocationOutOfSync(unsigned short locationid);
public slots:
	void setPlugin(QString plugin);

};

#endif // MAINWINDOW_H
