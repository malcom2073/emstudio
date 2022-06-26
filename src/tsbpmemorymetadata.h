/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef TSBPMEMORYMETADATA_H
#define TSBPMEMORYMETADATA_H

#include "readonlyramblock.h"
#include <QStringList>
#include "configblock.h"
#include "menusetup.h"
#include "table2dmetadata.h"
#include "table3dmetadata.h"
#include "lookupmetadata.h"
class TSBPMemoryMetaData
{
public:
    TSBPMemoryMetaData();
    void passConfigData(QMap<QString,QList<ConfigBlock> > block) { m_configMetaData = block; }
    void setMenuMetaData(MenuSetup menu) { m_menuMetaData = menu; }
    bool loadMetaDataFromFile(QStringList searchpaths);
    bool loadMetaDataFromFile(QString path){ return false; }
    bool parseMetaData(QString json);
    const QMap<unsigned short,QString> errorMap() { return m_errorMap; }


    bool has2DMetaData(unsigned short locationid);
    const QList<Table2DMetaData> table2DMetaData() { return m_table2DMetaData; }
    const Table2DMetaData get2DMetaData(unsigned short locationid);

    bool has3DMetaData(unsigned short locationid);
    const QList<Table3DMetaData> table3DMetaData() { return m_table3DMetaData; }
    const Table3DMetaData get3DMetaData(unsigned short locationid);

    bool hasRORMetaData(unsigned short locationid);
    const QList<ReadOnlyRamData> rORMetaData() { return m_readOnlyMetaData; }
    const ReadOnlyRamData getRORMetaData(unsigned short locationid);

    bool hasLookupMetaData(unsigned short locationid);
    const QMap<unsigned short,LookupMetaData> lookupMetaData() { return m_lookupMetaData; }
    const LookupMetaData getLookupMetaData(unsigned short locationid);

    bool hasConfigMetaData(QString name);
    const QMap<QString,QList<ConfigBlock> > configMetaData();
    const QList<ConfigBlock> getConfigMetaData(QString name);

    const MenuSetup menuMetaData();

    const QString getErrorString(unsigned short code);

private:
    MenuSetup m_menuMetaData;
    QMap<QString,QList<ConfigBlock> > m_configMetaData;
    QMap<unsigned short,QString> m_errorMap;
    QMap<unsigned short,LookupMetaData> m_lookupMetaData;
    QList<ReadOnlyRamData> m_readOnlyMetaData;
    QMap<unsigned short,ReadOnlyRamBlock> m_readOnlyMetaDataMap;
    QList<Table3DMetaData> m_table3DMetaData;
    QList<Table2DMetaData> m_table2DMetaData;

};

#endif // TSBPMEMORYMETADATA_H
