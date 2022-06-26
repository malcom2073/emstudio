#include "emstest.h"
#include <QDebug>
#include "tsbpconfigdata.h"
EMSTest::EMSTest(QObject *parent)
    : QObject{parent}
{

}


void EMSTest::startTest()
{
    TSBPConfigData config;
    config.setElementSize(4);
    config.setSize(8);
    config.setName("test");
    config.setType(ConfigData::ARRAY);
    config.setElementType(ConfigData::FLOAT_ELEMENT);
    QByteArray mybytes = QByteArray::fromHex("0000484400c0d24400b020450010584500b887450068a3450010bf4500c0da45");
    config.setOffset(0);
    //config.setSize(mybytes.length());
    config.setData(mybytes);
    for (int i=0;i<config.size();i++)
    {
        qDebug() << config.getValue(i);
    }
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
