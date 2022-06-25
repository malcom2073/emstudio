/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "tsbpmemorymetadata.h"

TSBPMemoryMetaData::TSBPMemoryMetaData()
{
}

bool TSBPMemoryMetaData::loadMetaDataFromFile(QStringList searchpaths)
{
    Q_UNUSED(searchpaths)
    return true;
}

bool TSBPMemoryMetaData::parseMetaData(QString json)
{
    Q_UNUSED(json)
    return true;
}

bool TSBPMemoryMetaData::has2DMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return false;
}

const Table2DMetaData TSBPMemoryMetaData::get2DMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return Table2DMetaData();
}

bool TSBPMemoryMetaData::has3DMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return false;
}

const Table3DMetaData TSBPMemoryMetaData::get3DMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return Table3DMetaData();
}

bool TSBPMemoryMetaData::hasRORMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return false;
}

const ReadOnlyRamData TSBPMemoryMetaData::getRORMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return ReadOnlyRamData();
}

bool TSBPMemoryMetaData::hasLookupMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return false;
}

const LookupMetaData TSBPMemoryMetaData::getLookupMetaData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return LookupMetaData();
}
bool TSBPMemoryMetaData::hasConfigMetaData(QString name)
{
    return m_configMetaData.contains(name);
}

const QMap<QString,QList<ConfigBlock> > TSBPMemoryMetaData::configMetaData()
{
    return m_configMetaData;
}

const QList<ConfigBlock> TSBPMemoryMetaData::getConfigMetaData(QString name)
{
    if (m_configMetaData.contains(name))
    {
        return m_configMetaData[name];
    }
    else
    {
        return QList<ConfigBlock>();
    }
}
const MenuSetup TSBPMemoryMetaData::menuMetaData()
{
    return m_menuMetaData;
}

const QString TSBPMemoryMetaData::getErrorString(unsigned short code)
{
    Q_UNUSED(code)
    return "";
}
