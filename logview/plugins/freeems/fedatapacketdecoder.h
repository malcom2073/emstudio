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

#ifndef FEDATAPACKETDECODER_H
#define FEDATAPACKETDECODER_H

#include <QObject>
#include <QMap>
#include <QVariantMap>
#include "datafield.h"
//#include "datapacketdecoder.h"
class FEDataPacketDecoder : public QObject
{
	Q_OBJECT
public:
	FEDataPacketDecoder();
	int fieldSize();
    //DataField getField(int num);
    QString getFieldName(int num);
    QString getFieldDescription(int num);
private:
	void loadDataFieldsFromValues();
	QList<DataField> m_dataFieldList;
signals:
	void payloadDecoded(QVariantMap data);
public slots:
	void decodePayload(QByteArray payload);
};

#endif // FEDATAPACKETDECODER_H
