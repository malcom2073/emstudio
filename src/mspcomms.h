/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MSPCOMMS_H
#define MSPCOMMS_H

#include <QObject>
#include "tsbpdatapacketdecoder.h"
#include "tsbpmemorymetadata.h"
#include "tsbptable2ddata.h"
#include "tsbptable3ddata.h"
#include "tsbpheaders.h"
#include "baseconfigdata.h"
#include <QVariant>
#include <QTcpSocket>
#include "configdata.h"
#include "rawdata.h"
#include <QSerialPort>
#include <QMutex>
#include "memorylocationinfo.h"
#include "arrayconfigdata.h"
#include "tableconfigdata.h"
#include "scalarconfigdata.h"
#include "bitconfigdata.h"
#include "filedownloader.h"
class MSPComms : public QObject
{
    Q_OBJECT
//Q_INTERFACES(EmsComms)
public:
    enum RequestType
    {
        GET_INTERFACE_VERSION=0x0000,
        GET_FIRMWARE_VERSION=0x0002,
        GET_MAX_PACKET_SIZE=0x0004,
        ECHO_PACKET=0x0006,
        SOFT_RESET=0x0008,
        HARD_RESET=0x000A,
        UPDATE_BLOCK_IN_RAM=0x0100,
        UPDATE_BLOCK_IN_FLASH=0x0102,
        RETRIEVE_BLOCK_IN_RAM=0x0104,
        RETRIEVE_PAGE=0x1105,
        RETRIEVE_BLOCK_IN_FLASH=0x0106,
        BURN_BLOCK_FROM_RAM_TO_FLASH=0x0108,
        GET_LOCATION_ID_LIST=0xDA5E,
        GET_DECODER_NAME=0xEEEE,
        GET_FIRMWARE_BUILD_DATE=0xEEF0,
        GET_COMPILER_VERSION=0xEEF2,
        GET_OPERATING_SYSTEM=0xEEF4,
        GET_LOCATION_ID_INFO=0xF8E0,
        SERIAL_CONNECT=0xFFFF01,
        SERIAL_DISCONNECT=0xFFFF02,
        GET_DATA
    };
    enum ConnectionType
    {
        TCPSOCKET,
        SERIAL,
        CAN
    };

    class RequestClass
    {
    public:
        RequestClass()
        {
            retryCount=0;
        }
        unsigned char retryCount;
        RequestType type;
        QList<QVariant> args;
        QList<int> argsize;
        int sequencenumber;
        void addArg(QVariant arg,int size=0) { args.append(arg); argsize.append(size);}
    };
    MSPComms(QObject *parent=0);
    Table* getTableFromName(QString name);
    Curve* getCurveFromName(QString name);
    ArrayConfigData *getArrayConfigData(QString name);
    TableConfigData *getTableConfigData(QString name);
    ScalarConfigData *getScalarConfigData(QString name);
    BitConfigData *getBitConfigData(QString name);
    int sendBurn();


    QString getPluginCompat() { return QString("MEGASQUIRT"); }
    void stop();
    void setLogsEnabled(bool enabled);
    TSBPDataPacketDecoder *getDecoder();
    TSBPMemoryMetaData *getMetaParser();
    TSBPTable3DData *getNew3DTableData();
    TSBPTable2DData *getNew2DTableData();
    //void passLogger(QsLogging::Logger *log);
    int startBenchTest(unsigned char eventspercycle, unsigned short numcycles, unsigned short ticksperevent, QVariantList pineventmask, QVariantList pinmode);
    int stopBenchTest();
    int bumpBenchTest(unsigned char cyclenum);
    ConfigData* getConfigData(QString name);

    QList<QString> getConfigList();
    void writeAllRamToRam();
    void setLogDirectory(QString dir);
    void setPort(QString portname);
    void setBaud(int baudrate);
    void setLogFileName(QString filename);
    void sendPacket(RequestClass req);
    int getInterfaceVersion();
    int getFirmwareVersion();
    int echoPacket(QByteArray packet);
    int getLocationIdList(unsigned char listtype, unsigned short listmask);
    int softReset();
    int hardReset();
    QMap<int,int> m_pageSizeMap;
    bool sendSimplePacket(unsigned short payload);
    void connectSerial(bool isserial,QString port,int baud);
    void disconnectSerial();
    void loadLog(QString filename);
    void playLog();
    void startInterrogation();
    TSBPTable2DData* get2DTableData(unsigned short locationid);
    TSBPTable2DData* get2DTableData(QString varname);
    TSBPTable3DData* get3DTableData(unsigned short locationid);
    TSBPTable3DData* get3DTableData(QString locationname);
    RawData* getRawData(unsigned short locationid);
    //ConfigData *getConfigData(QString varname);
    QByteArray generatePacket(QByteArray header,QByteArray payload);
    int updateBlockInRam(unsigned short location,unsigned short offset, unsigned short size,QByteArray data);
    int updateBlockInFlash(unsigned short location,unsigned short offset, unsigned short size,QByteArray data);
    int retrieveBlockFromRam(unsigned short location, unsigned short offset, unsigned short size,bool mark=true);
    int retrieveBlockFromFlash(unsigned short location, unsigned short offset, unsigned short size,bool mark=true);
    int burnBlockFromRamToFlash(unsigned short location,unsigned short offset, unsigned short size);
    void setInterByteSendDelay(int milliseconds);
    void setlogsDebugEnabled(bool enabled);
    int enableDatalogStream();
    int disableDatalogStream();
    void acceptLocalChanges() { };
    void rejectLocalChanges() { };
private:
    QMap<QString,TableConfigData*> m_tableDataMap;
    QMap<QString,ArrayConfigData*> m_arrayDataMap;
    QMap<QString,ScalarConfigData*> m_scalarDataMap;
    QMap<QString,Table*> tableMap;
    QMap<QString,Curve*> curveMap;
    QMap<unsigned int,BitConfigData*> m_bitDataLocationMap;
    QMap<QString,BitConfigData*> m_bitDataNameMap;
    void loadIniFile(QFile *inifile);
    bool m_iniFileLoaded;
    FileDownloader m_fileDownloader;
private slots:
    void memorySaveSlot();
private:
    int requestPage(QByteArray pagereqstring,int length);
    unsigned long currentPacketNum;
    unsigned int currentPacketCount;
    QByteArray readPacket(QSerialPort *port);
    QMap<QString,QMap<QString,scalarclass> > m_pageMap;
    QMutex reqListMutex;
    QList<RequestClass> m_reqList;
    RequestClass m_currentRequest;
    QList<RequestClass> m_privReqList;
    TSBPDataPacketDecoder *m_dataDecoder;
    QString m_portName;
    ConnectionType m_connectionType;
    QTcpSocket *m_tcpPort;
    QSerialPort *m_serialPort;
    QByteArray m_serialPortBuffer;
    TSBPMemoryMetaData *m_memoryMetaData;
    bool m_connected;
    bool m_interrogateInProgress;
    QList<int> m_interrogateList;
    QList<int> m_pageSizeList;
    QList<int> m_pageList;
    QList<QString> m_pageReqStringList;
    QMap<QString,TSBPConfigData*> m_configDataMap;
    QList<QString> m_configNameList;
    QMap<QString,TSBPTable2DData*> m_2dTableData;
    QMap<QString,TSBPTable3DData*> m_3dTableData;
    void triggerNextSend();

    //QMap<QString,QMap<QString,scalarclass> > pageMap;
    QMap<QString,QMap<QString,arrayclass> > m_pageArrayMap;
    QMap<int,QByteArray> m_pageBufferMap;
    QMap<int,int> m_pageBufferFilledMap;

    QMap<int,QList<TSBPTable2DData*> > m_pageTo2DTableList;
    QMap<int,QList<TSBPTable3DData*> > m_pageTo3DTableList;
    QList<int> m_dirtyPageList;
    QStringList splitline(QString csvline);
    QTimer *savePageTimer;
    QList<int> m_savePageList;
    bool m_threadRun;
    int m_state;
    bool m_waitingForPacketResponse;
signals:
    void packetSent(unsigned short locationid,QByteArray header,QByteArray payload);
    void packetAcked(unsigned short locationid,QByteArray header,QByteArray payload);
    void packetNaked(unsigned short locationid,QByteArray header,QByteArray payload,unsigned short errornum);
    void decoderFailure(QByteArray packet);
    void decoderName(QString name);
    void firmwareBuild(QString date);
    void compilerVersion(QString version);
    void operatingSystem(QString os);
    void connected();
    void disconnected();
    void locationIdList(QList<unsigned short> idlist);
    void locationIdInfo(unsigned short locationid,MemoryLocationInfo info);
    void ramBlockRetrieved(unsigned short locationid, QByteArray header,QByteArray payload);
    void flashBlockRetrieved(unsigned short locationid,QByteArray header,QByteArray payload);
    void dataLogPayloadReceived(QByteArray header,QByteArray payload);
    void dataLogPayloadReceived(QByteArray packet);
    void interfaceVersion(QString version);
    void firmwareVersion(QString version);
    void debugVerbose(QString msg);
    void unknownPacket(QByteArray header,QByteArray payload);
    void debug(QString msg);
    //void error(QString msg);
    //void error(QString msg);
    //void error(SerialPortStatus error,QString msg = QString());
    void commandSuccessful(int sequencenumber);
    void commandFailed(int sequencenumber,unsigned short errornum);
    void commandTimedOut(int sequencenumber);
    void interrogationComplete();
    void emsSilenceStarted();
    void emsSilenceBroken();
    void interrogateTaskStart(QString task, int sequence);
    void interrogateTaskSucceed(int sequence);
    void interrogateTaskFail(int sequence);
    void interrogationProgress(int current, int total);
    void interrogationData(QMap<QString,QString> datamap);
    void dataLogPayloadDecoded(QVariantMap data);
    void memoryDirty();
    void memoryClean();

private slots:
    void fileDownloaded(QFile *file);


    void handleReadyRead();
    void savePageTimerTick();
    void packetCounter();
    void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
    void pageBurnRequested(unsigned int page);
    void parseBuffer(QByteArray data);
    void serialPortConnected();
protected:
    void run();
};

#endif // MSPCOMMS_H
