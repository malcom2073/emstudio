/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef CONFIGBLOCK_H
#define CONFIGBLOCK_H
#include <QString>
#include <QPair>
#include <QList>
class ConfigBlock
{
public:
    ConfigBlock() {}
    ConfigBlock(QString name,QString type, QString override,unsigned short locationid, unsigned short size,unsigned short elementsize,unsigned short offset, QList<QPair<QString,double> > calc)
    {
        m_name = name;
        m_type = type;
        m_sizeOverride = override;
        m_locationId = locationid;
        m_size = size;
        m_elementSize = elementsize;
        m_offset = offset;
        m_calc = calc;
    }

    void setName(QString name) { m_name = name; }
    void setType(QString type) { m_type = type; }
    void setSizeOverride(QString override) { m_sizeOverride = override; }
    void setSizeOverrideMult(double mult) { m_sizeOverrideMult = mult; }
    void setLocationId(unsigned short locationid) { m_locationId = locationid; }
    void setSize(unsigned short size) { m_size = size; }
    void setElementSize(unsigned short size) { m_elementSize = size; }
    void setOffset(unsigned short offset) { m_offset = offset; }
    void setCalc(QList<QPair<QString,double> > calc) { m_calc = calc; }
    QString name() { return m_name; }
    QString type() { return m_type; }
    QString sizeOverride() { return m_sizeOverride; }
    double sizeOverrideMult() { return m_sizeOverrideMult; }
    unsigned short locationId() { return m_locationId; }
    unsigned short offset() { return m_offset; }
    unsigned short size() { return m_size; }
    unsigned short elementSize() { return m_elementSize; }
    QList<QPair<QString,double> > calc() { return m_calc; }
private:
    QString m_name;
    QString m_type;
    QString m_sizeOverride;
    double m_sizeOverrideMult;
    unsigned short m_locationId;
    unsigned short m_size;
    unsigned short m_elementSize;
    unsigned short m_offset;
    QList<QPair<QString,double> > m_calc;


};

#endif // CONFIGBLOCK_H
