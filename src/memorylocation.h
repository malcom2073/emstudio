/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2013  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of EMStudio                                                  *
 *                                                                                  *
 * EMStudio is free software; you can redistribute it and/or                        *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * EMStudio is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
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
