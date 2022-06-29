/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "emstest.h"
#include <QDebug>
//#include "tsbpconfigdata.h"
#include "arrayconfigdata.h"
#include "tableview3d.h"
#include "tablemap3d.h"
EMSTest::EMSTest(QObject *parent)
    : QObject{parent}
{

}

bool EMSTest::testSimple2DWrite()
{
    // Tests to make sure data modification works
    QByteArray inputbytes = QByteArray::fromHex("0000484400c0d24400b020450010584500b887450068a3450010bf4500c0da45");
    QByteArray outputbytes = QByteArray::fromHex("0000484400c0d24400002f450010584500b887450068a3450010bf4500c0da45");
    QList<float> outputvals;
    outputvals.append(800);
    outputvals.append(1686);
    outputvals.append(2800);
    outputvals.append(3457);
    outputvals.append(4343);
    outputvals.append(5229);
    outputvals.append(6114);
    outputvals.append(7000);


    ArrayConfigData config;
    config.setElementSize(4);
    config.setSize(8);
    config.setName("test");
    config.setType(ConfigData::ARRAY);
    config.setElementType(ConfigData::FLOAT_ELEMENT);
    config.setOffset(0);
    config.setData(inputbytes);
    config.setValue(2,2800);
    qDebug() << config.getBytes().toHex();
    for (int i=0;i<config.size();i++)
    {
        if (config.getValue(i) != outputvals.at(i))
        {
            return false;
        }
    }
    if (outputbytes != config.getBytes())
    {
        return false;
    }
    return true;

}


bool EMSTest::testSimple2DArray()
{
    // Tests to make sure conversion from bytes to array then back to bytes works.
    QByteArray inputbytes = QByteArray::fromHex("0000484400c0d24400b020450010584500b887450068a3450010bf4500c0da45");
    QList<float> outputvals;
    outputvals.append(800);
    outputvals.append(1686);
    outputvals.append(2571);
    outputvals.append(3457);
    outputvals.append(4343);
    outputvals.append(5229);
    outputvals.append(6114);
    outputvals.append(7000);


    ArrayConfigData config;
    config.setElementSize(4);
    config.setSize(8);
    config.setName("test");
    config.setType(ConfigData::ARRAY);
    config.setElementType(ConfigData::FLOAT_ELEMENT);
    config.setOffset(0);
    config.setData(inputbytes );
    for (int i=0;i<config.size();i++)
    {
        if (config.getValue(i) != outputvals.at(i))
        {
            return false;
        }
    }
    if (inputbytes != config.getBytes())
    {
        return false;
    }
    return true;

}
void EMSTest::startTest()
{
    // Because I hate conversions, let's make sure these work right. Hint: Before I did this, they did not.
    // This won't match what PC calculator will tell you, becaues of byte ordering... but it seems to match the ECU.
    QByteArray shorttest = QByteArray::fromHex("CDABCDAB");
    assert(ConfigData::BytesToValue(shorttest,4,false) == 2882382797);
    assert(ConfigData::BytesToValue(shorttest,4,true)== -1412584499);
    assert(ConfigData::BytesToValue(shorttest.mid(2),2,false) == 43981);
    assert(ConfigData::BytesToValue(shorttest.mid(2),2,true)== -21555);
    assert(ConfigData::BytesToValue(shorttest.mid(3),1,false) == 171);
    assert(ConfigData::BytesToValue(shorttest.mid(3),1,true)== -85);

    m_failure = false;
    int succeedcount = 0;
    int failcount = 0;
    if (!(m_failure = testSimple2DArray()))
    {
        failcount++;
        qDebug() << "testSimple2DArray failed";
    }
    else
    {
        qDebug() << "testSimple2DArray succeeded";
        succeedcount++;
    }
    if (!(m_failure = testSimple2DWrite()))
    {
        failcount++;
        qDebug() << "testSimple2DWrite failed";
    }
    else
    {
        succeedcount++;
        qDebug() << "testSimple2DWrite succeeded";
    }

    qDebug() << succeedcount << "tests passed." << failcount << "tests failed.";


    //testSimple2DWrite();
    //return;

    m_comms = new MSPComms();
    connect(m_comms,&MSPComms::interrogationComplete,this,&EMSTest::interrogationCompleted);
    m_comms->setPort("\\\\.\\COM100");
    m_comms->setBaud(115200);
    m_comms->connectSerial("\\\\.\\COM100",115200);
    m_comms->startInterrogation();
}
void EMSTest::interrogationCompleted()
{
    qDebug() << "Finished interrogation!";
    // At this point, all the memory in m_comms should be valid and equal to the msq from TS.
    //TSBPConfigData *data = qobject_cast<TSBPConfigData*>(m_comms->getConfigData("lambdaTable"));
    BitConfigData *bitdata = m_comms->getBitConfigData("map_sensor_type");
    if (!bitdata)
    {
        qDebug() << "Unable ot cast for map_sensor_type";
        return;
    }
    qDebug() << "Enginetype: " << bitdata->getValue("map_sensor_type");
    return;
    ArrayConfigData *data = m_comms->getArrayConfigData("lambdaLoadBins");

    if (!data)
    {
        qDebug() << "Unable to cast data for lambdaLoadBins";
        return;
    }
    qDebug() << "Got scalar!" << "size" << data->size() << "offset" << data->offset() << "value:" << data->getValue(0);
    for (int i=0;i<data->size();i++)
    {
        qDebug() << data->getValue(i);
    }
    qDebug() << data->getBytes().toHex();


    TableConfigData *tdata = m_comms->getTableConfigData("tpsTpsAccelTable");
    if (!tdata)
    {
        qDebug() << "Unable to cast data for tpsTpsAccelTable";
        return;
    }
    for (int x=0;x<tdata->xSize();x++)
    {
        QString linestr = "";
        for (int y=0;y<tdata->ySize();y++)
        {
            linestr += QString::number(tdata->getValue(x,y).toFloat()) + ",";
        }
        qDebug() << linestr;
    }
    QByteArray origbytes =tdata->getOrig();
    QByteArray newbytes = tdata->getBytes();
    qDebug() << "Org: " << origbytes.toHex();
    qDebug() << "New: " << newbytes.toHex();
    //x: lambdaRpmBins
    //y: lambdaLoadBins
    //z: lambdaTable
    ArrayConfigData *xdata = m_comms->getArrayConfigData("ignitionRpmBins");
    ArrayConfigData *ydata = m_comms->getArrayConfigData("ignitionLoadBins");
    TableConfigData *zdata = m_comms->getTableConfigData("ignitionTable");
    if (!xdata || !ydata || !zdata)
    {
        qDebug() << "Unable to cast data for 3d table!";
        return;
    }
    TableView3D *table = new TableView3D();
    table->setData("Lambda",xdata,ydata,zdata);
    table->show();

    /*TableMap3D *map = new TableMap3D();
    map->setData("Lambda",xdata,ydata,zdata);
    map->show();*/

}
