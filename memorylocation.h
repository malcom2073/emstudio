/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#ifndef MEMORYLOCATION_H
#define MEMORYLOCATION_H

#include <QObject>

class MemoryLocation
{
public:
    explicit MemoryLocation();
    bool isFlash;
    bool isRam;
    bool hasParent;
    bool isReadOnly;
    int type;
    unsigned short parent;
    unsigned short ramPage;
    unsigned short ramAddress;
    unsigned short flashPage;
    unsigned short flashAddress;
    unsigned short size;
    unsigned short locationid;
    void addChild(MemoryLocation *child);
    void setParent(MemoryLocation *parent);
    MemoryLocation *getParent() { return m_parent; }
    void setData(QByteArray data);
    bool isEmpty();
    void childChanged(MemoryLocation *child,QByteArray data);
    QByteArray data(MemoryLocation *child);
    QByteArray data();
private:
    QByteArray m_data;
    QList<MemoryLocation*> m_childList;
    MemoryLocation *m_parent;
signals:

public slots:

};

#endif // MEMORYLOCATION_H
