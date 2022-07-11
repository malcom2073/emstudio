#include "logfilemanager.h"
#include <QByteArray>
#include <QDebug>
#include "configdata.h"
#include <QTimer>
LogFileManager::LogFileManager(QObject *parent) : QObject(parent)
{
    m_logFile = new QFile("776.mlg");
    m_logFile->open(QIODevice::ReadOnly);
    if (m_logFile->read(6) != QByteArray::fromHex("4d4c564c4700"))
    {
        qDebug() << "Invalid log";
        return;
    }
    QByteArray format = m_logFile->read(2);
    QByteArray time = m_logFile->read(4);
    qDebug() << "Unix time:" << ConfigData::BytesToValue(time,4,false,true).toUInt();
    infodatastart_offset = ConfigData::BytesToValue(m_logFile->read(2),2,false,true).toUInt();
    databegin_offset = ConfigData::BytesToValue(m_logFile->read(4),4,false,true).toUInt();
    record_length = ConfigData::BytesToValue(m_logFile->read(2),2,false,true).toUInt();
    loggerfield_count = ConfigData::BytesToValue(m_logFile->read(2),2,false,true).toUInt();
    QByteArray loggerfields = m_logFile->read(loggerfield_count*55);


    for (int i=0;i<loggerfields.size();i+=55)
    {
        LogField field;
        uint8_t type = loggerfields[i];
        if (type < 8)
        {
            field.scale = ConfigData::BytesToFloat(loggerfields.mid(i+46,4)).toFloat();
            field.transform = ConfigData::BytesToFloat(loggerfields.mid(i+50,4)).toFloat();
        }
        if (type == 0)
        {
            field.bytes = 1;
            field.issigned = false;
            field.isfloat = false;
        }
        else if (type == 1)
        {
            field.bytes = 1;
            field.issigned = true;
            field.isfloat = false;
        }
        else if (type == 2)
        {
            field.bytes = 2;
            field.issigned = false;
            field.isfloat = false;
        }
        else if (type == 3)
        {
            field.bytes = 2;
            field.issigned = true;
            field.isfloat = false;
        }
        else if (type == 4)
        {
            field.bytes = 4;
            field.issigned = false;
            field.isfloat = false;
        }
        else if (type == 5)
        {
            field.bytes = 4;
            field.issigned = true;
            field.isfloat = false;
        }
        else if (type == 6)
        {
            field.bytes = 8;
            field.issigned = true;
            field.isfloat = false;
        }
        else if (type == 7)
        {
            field.bytes = 4;
            field.issigned = true;
            field.isfloat = true;
        }
        else if (type == 10)
        {
            field.bytes = 1;
            field.issigned = true;
            field.isfloat = false;
        }

        else if (type == 11)
        {
            field.bytes = 2;
            field.issigned = true;
            field.isfloat = false;
        }

        else if (type == 12)
        {
            field.bytes = 4;
            field.issigned = true;
            field.isfloat = false;
        }
        field.name = QString(loggerfields.mid(i+1,34).replace('\0',' ')).trimmed();
        field.units = QString(loggerfields.mid(i+35,10).replace('\0',' ')).trimmed();
        qDebug() << "Field name:" << QString(loggerfields.mid(i+1,34).replace('\0',' ')).trimmed();
        //blockordering.append(QString(loggerfields.mid(i+1,34).replace('\0',' ')).trimmed());
        blockordering.append(field);
        qDebug() << "UM:" << QString(loggerfields.mid(i+35,10).replace('\0',' ')).trimmed();
    }
    m_logFile->seek(databegin_offset);
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&LogFileManager::timerTick);
    timer->start(2);
}
void LogFileManager::timerTick()
{
    m_blockpos += 1;
    emit logFileProgress(m_logFile->pos(),m_logFile->size());
    //QThread::msleep(1);
    uint8_t blocktype = m_logFile->read(1).toUInt();
    QVariantMap valuemap;
    if (blocktype == 0)
    {
        QByteArray record = m_logFile->read(4+record_length);
        //qDebug() << "Normal block";
        uint64_t current = 3;
        for (int i=0;i<blockordering.size();i++)
        {
                QByteArray currrecord = record.mid(current,blockordering[i].bytes);
                if (blockordering[i].isfloat)
                {
                    float val = ConfigData::BytesToFloat(currrecord).toFloat();
                    val = (val + blockordering[i].transform) * blockordering[i].scale;
                   // qDebug() << blockordering[i].name << val;
                    valuemap.insert(blockordering[i].name,val);
                }
                else
                {
                    float val = ConfigData::BytesToValue(currrecord,blockordering[i].bytes,blockordering[i].issigned,true).toUInt();
                    val = (val + blockordering[i].transform) * blockordering[i].scale;
                    //qDebug() << blockordering[i].name << val;
                    valuemap.insert(blockordering[i].name,val);
                }
                if (blockordering[i].name == "RPM")
                {
                    int stopper = 1;
                }
                if (blockordering[i].name == "CLT")
                {
                    int stopper = 1;
                }
            //}
            current += blockordering[i].bytes;
        }
    }
    else if (blocktype == 1)
    {
        QByteArray markerblock = m_logFile->read(53);
        qDebug() << "Marker Block";
    }
    else
    {
        qDebug() << "Invalid block type!";
        return;
    }
    //QByteArray record = log.read(record_length);
    emit dataLogPayloadDecoded(valuemap);
}
