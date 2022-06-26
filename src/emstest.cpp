#include "emstest.h"
#include <QDebug>
#include "tsbpconfigdata.h"
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


    TSBPConfigData config;
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


    TSBPConfigData config;
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


    testSimple2DWrite();
    return;

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
    TSBPConfigData *data = qobject_cast<TSBPConfigData*>(m_comms->getConfigData("map_samplingWindowBins"));

    if (!data)
    {
        qDebug() << "Unable to cast data for mapErrorDetectionTooHigh";
        return;
    }
    qDebug() << "Got scalar!" << "size" << data->size() << "offset" << data->offset() << "value:" << data->getValue();
    for (int i=0;i<data->size();i++)
    {
        qDebug() << data->getValue(i);
    }
    qDebug() << data->getBytes().toHex();

}
