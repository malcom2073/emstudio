/**************************************************************************************
 * EMStudio - Open Source ECU tuning software                                         *
 * Copyright (C) 2013  Michael Carpenter (malcom2073@gmail.com)                       *
 *                                                                                    *
 * This file is a part of EMStudio                                                    *
 *                                                                                    *
 * EMStudio is free software; you can redistribute it and/or                          *
 * modify it under the terms of the GNU Lesser General Public                         *
 * License as published by the Free Software Foundation, version                      *
 * 2.1 of the License.                                                                *
 *                                                                                    *
 * EMStudio is distributed in the hope that it will be useful,                        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                  *
 * Lesser General Public License and GNU General Public License for                   *
 * more details.                                                                      *
 *                                                                                    *
 * You should have received a copy of the GNU Lesser General Public License           *
 * along with this program; if not, write to Free Software Foundation, Inc.,          *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                       *
 *                                                                                    *
 *------------------------------------------------------------------------------------*
 *                                                                                    *
 * BSD 3-Clause License Usage                                                         *
 * Alternatively, this file may also be used under the terms of the                   *
 * BSD 3-Clause license, detailed below:                                              *
 *                                                                                    *
 * Redistribution and use in source and binary forms, with or without                 *
 * modification, are permitted provided that the following conditions are met:        *
 *     * Redistributions of source code must retain the above copyright               *
 *       notice, this list of conditions and the following disclaimer.                *
 *     * Redistributions in binary form must reproduce the above copyright            *
 *       notice, this list of conditions and the following disclaimer in the          *
 *       documentation and/or other materials provided with the distribution.         *
 *     * Neither the name EMStudio nor the                                            *
 *       names of its contributors may be used to endorse or promote products         *
 *       derived from this software without specific prior written permission.        *
 *                                                                                    *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND    *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED      *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE             *
 * DISCLAIMED. IN NO EVENT SHALL MICHAEL CARPENTER BE LIABLE FOR ANY                  *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES         *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;       *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND        *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS      *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                       *
 **************************************************************************************/

#ifndef EMSCOMMS_H
#define EMSCOMMS_H

#include <QThread>
#include <QFile>
#include <QMutex>
#include <QVariant>
#include <QDateTime>
#include <QtPlugin>
#include "table3ddata.h"
#include "table2ddata.h"
#include "datapacketdecoder.h"
#include "memorymetadata.h"
#include "rawdata.h"
#include "configdata.h"
//#include "QsLog.h"

namespace QsLogging
{
	class Logger;
}

class EmsComms : public QObject
{
	Q_OBJECT
public:
	EmsComms(QObject *parent) : QObject(parent) {}
	virtual QString getPluginCompat()=0;
	virtual void passLogger(QsLogging::Logger *log)=0;
	virtual void stop()=0;
	virtual void setLogsEnabled(bool enabled)=0;
	virtual DataPacketDecoder *getDecoder()=0;
	virtual MemoryMetaData *getMetaParser()=0;
	virtual Table3DData *getNew3DTableData()=0;
	virtual Table2DData *getNew2DTableData()=0;
	virtual void setLogDirectory(QString dir)=0;
	virtual void setPort(QString portname)=0;
	virtual void setBaud(int baudrate)=0;
	virtual void setLogFileName(QString filename)=0;
	virtual int getInterfaceVersion()=0;
	virtual int getFirmwareVersion()=0;
	virtual int echoPacket(QByteArray packet)=0;
	virtual int getLocationIdList(unsigned char listtype, unsigned short listmask)=0;
	virtual int softReset()=0;
	virtual int hardReset()=0;
	virtual int Q_INVOKABLE startBenchTest(unsigned char eventspercycle,unsigned short numcycles,unsigned short ticksperevent,QVariantList pineventmask,QVariantList pinmode)=0;
	virtual int Q_INVOKABLE stopBenchTest()=0;
	virtual int Q_INVOKABLE bumpBenchTest(unsigned char cyclenum)=0;
	virtual bool sendSimplePacket(unsigned short payload)=0;
	virtual void connectSerial(QString port,int baud)=0;
	virtual void disconnectSerial()=0;
	virtual void loadLog(QString filename)=0;
	virtual void playLog()=0;
	virtual void startInterrogation()=0;
	virtual Table2DData* get2DTableData(unsigned short locationid)=0;
	virtual Table3DData* get3DTableData(unsigned short locationid)=0;
	virtual Table2DData* get2DTableData(QString locationname)=0;
	virtual Table3DData* get3DTableData(QString locationname)=0;
	virtual RawData* getRawData(unsigned short locationid)=0;
	virtual ConfigData* getConfigData(QString name) = 0;
	virtual QList<QString> getConfigList()=0;
	//virtual void populateDataFields()=0;
	virtual int updateBlockInRam(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)=0;
	virtual int updateBlockInFlash(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)=0;
	virtual int retrieveBlockFromRam(unsigned short location, unsigned short offset, unsigned short size,bool mark=true)=0;
	virtual int retrieveBlockFromFlash(unsigned short location, unsigned short offset, unsigned short size,bool mark=true)=0;
	virtual int burnBlockFromRamToFlash(unsigned short location,unsigned short offset, unsigned short size)=0;
	virtual void setInterByteSendDelay(int milliseconds)=0;
	virtual void setlogsDebugEnabled(bool enabled)=0;
	virtual int enableDatalogStream()=0;
	virtual int disableDatalogStream()=0;
	virtual void writeAllRamToRam()=0;
	virtual void acceptLocalChanges()=0;
	virtual void rejectLocalChanges()=0;
};
Q_DECLARE_INTERFACE(EmsComms,"EmsComms/1.0")
#endif // EMSCOMMS_H
