/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tsbpcomms.h"
#include "tsbpdatapacketdecoder.h"
#include "serialport.h"
#include "QsLog.h"
#include <QDebug>
#include <QMap>
#include <QStringList>
#include "tsbptable2ddata.h"


static unsigned long Crc32_ComputeBuf( unsigned long inCrc32, const void *buf,
				       size_t bufLen )
{
    static const unsigned long crcTable[256] = {
   0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
   0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
   0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
   0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
   0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
   0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
   0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
   0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
   0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
   0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
   0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
   0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
   0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
   0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
   0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
   0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
   0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
   0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
   0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
   0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
   0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
   0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
   0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
   0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
   0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
   0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
   0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
   0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
   0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
   0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
   0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
   0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
   0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
   0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
   0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
   0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
   0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D };
    unsigned long crc32;
    unsigned char *byteBuf;
    size_t i;

    /** accumulate crc32 for buffer **/
    crc32 = inCrc32 ^ 0xFFFFFFFF;
    byteBuf = (unsigned char*) buf;
    for (i=0; i < bufLen; i++) {
	crc32 = (crc32 >> 8) ^ crcTable[ (crc32 ^ byteBuf[i]) & 0xFF ];
    }
    return( crc32 ^ 0xFFFFFFFF );
}


TSBPComms::TSBPComms(QObject *parent) : EmsComms(parent)
{
	qRegisterMetaType<QMap<QString,QString> >("QMap<QString,QString>");
	m_connected = false;
	m_dataDecoder = new TSBPDataPacketDecoder();
	connect(this,SIGNAL(dataLogPayloadReceived(QByteArray)),m_dataDecoder,SLOT(decodePayload(QByteArray)));
	connect(m_dataDecoder,SIGNAL(payloadDecoded(QVariantMap)),this,SIGNAL(dataLogPayloadDecoded(QVariantMap)));
	m_memoryMetaData = new TSBPMemoryMetaData();
	qRegisterMetaType<QList<unsigned short> >("QList<unsigned short>");
	currentPacketNum=1;
	m_interrogateInProgress = false;
	QTimer *timer = new QTimer();
	connect(timer,SIGNAL(timeout()),this,SLOT(packetCounter()));
	timer->start(10000);
	m_threadRun = true;
	m_waitingForPacketResponse = false;

	QFile inifile("ms.ini");
	inifile.open(QIODevice::ReadOnly);
	QString inistring = inifile.readAll();
	inifile.close();
	QStringList inilist = inistring.split("\n");
	QVariantMap map;
	QMap<QString,scalarclass> scalarMap;
	QMap<QString,arrayclass> arrayMap;
	QList<DataField> dataLogMap;
	QString pagenum = "";
	int section = -1;
	QString currenttablevar;
	QString currentcurvevar;
	//QList<QPair<QString,QString> > submenulist;
	//QMap<QString,QList<QPair<QString,QString> > > menulist;
	//QMap<QString,QList<QPair<QString,QString> > > dialogmap;
	QList<SubMenuItem> currSubMenuList;
	MenuItem currentMenuItem;
	QList<DialogField> currDialogFieldList;
	DialogItem currentDialogItem;
	QString currentdialog;
	bool intable = false;
	QMap<QString,QMap<QString,scalarclass> > pageMap;
	QMap<QString,QMap<QString,arrayclass> > pageArrayMap;
	QList<QString> pageIdent;
	QList<QString> pageActivate;
	QList<int> pageSize;
	//QList<QString> menuorder;
	bool inmenu = false;
	QString menuname;
	bool indialog = false;
	MenuSetup menu;
	int pagenumint = 0;
	savePageTimer = new QTimer(this);
	connect(savePageTimer,SIGNAL(timeout()),this,SLOT(savePageTimerTick()));

	foreach (QString line,inilist)
	{
		if (line.startsWith(";"))
		{
			continue;
		}
		if (line.startsWith("[Constants]"))
		{
			section = 1;
		}
		if (line.startsWith("[BurstMode]"))
		{
			section = 2;
		}
		if (line.startsWith("[OutputChannels]"))
		{
			section = 3;
		}
		if (line.trimmed().startsWith("menuDialog"))
		{
			section = 4;
		}
		if (line.trimmed().startsWith("[UserDefined]"))
		{
			section = 5;
		}
		if (line.trimmed().startsWith("[TableEditor]"))
		{
			section = 6;
		}
		if (line.trimmed().startsWith("[CurveEditor]"))
		{
			section = 7;
		}
		if (section == 2)
		{
			//Get burst mode command here
		}
		if (section == 5)
		{
			//Dialog stuff
			if (!line.startsWith(";"))
			{
				QStringList linesplit = line.split("=");
				if (linesplit.size() > 1)
				{
					if (linesplit[0].trimmed() == "dialog")
					{
						if (indialog)
						{
							//menu.dialoglist.append(currDialogFieldList);
							currentDialogItem.fieldList.append(currDialogFieldList);
							currDialogFieldList.clear();
							menu.dialoglist.append(currentDialogItem);
							currentDialogItem = DialogItem();
						}
						QStringList currdialogitemsplit = linesplit[1].split(",");
						if (currdialogitemsplit.size() >= 2)
						{
							//Has a variable and title
							currentDialogItem.variable = currdialogitemsplit[0].replace("\"","").trimmed();
							currentDialogItem.title = currdialogitemsplit[1].replace("\"","").trimmed();
						}
						if (currdialogitemsplit.size() >= 3)
						{
							//Unknown, but sometimes present?
						}
						//currentdialog = linesplit[1].split(",")[0].replace("\"","").trimmed();
						//currentdialog = linesplit[1].split(",")[1].replace("\"","").trimmed();
						indialog = true;

					}
					else if (linesplit[0].trimmed() == "field")
					{
						//qDebug() << "Line" << line;
						//if (linesplit[1].split(",").size() > 1) //We actually have a field
						//{
							//dialogmap[currentdialog].append(QPair<QString,QString>(linesplit[1].split(",")[0].replace("\"","").trimmed(),linesplit[1].split(",")[1].replace("\"","").trimmed()));
							DialogField field;
							//QStringList dialogfieldsplit = linesplit[1].split(",");
							//QStringList outlist;
							//QString current = "";
							//for (int i=0;i<dialogfieldsplit.size();i++)
							//{
							//	QString splitter = dialogfieldsplit[i];
							//	if (splitter.count("\"") == 1)
							//	{
							//		//Merge with the next one;
//
							//	/}
							//}
							QStringList dialogfieldsplit = splitline(linesplit[1]);
							if (dialogfieldsplit.size() >= 1)
							{
								field.title = dialogfieldsplit[0].replace("\"","").trimmed();
							}
							else
							{
								//Nothing??
								qDebug() << "error in dialogfieldsplit";
							}
							if (dialogfieldsplit.size() >= 2)
							{
								//There is a variable
								field.variable = dialogfieldsplit[1].replace("\"","").trimmed();
							}
							if (dialogfieldsplit.size() >= 3)
							{
								//There is a condition
								field.condition = dialogfieldsplit[2].replace("\"","").trimmed();
							}
							currDialogFieldList.append(field);
						//}
						//qDebug() << "dLine";
					}
					else if (linesplit[0].trimmed() == "panel")
					{
						//Sub dialog window
						QStringList panelsplit = splitline(linesplit[1]);
						if (panelsplit.size() >= 2)
						{
						QString panelname = panelsplit[0];
						QString panelloc = panelsplit[1];
						currentDialogItem.panelList.append(QPair<QString,QString>(panelloc,panelname));
						}
					}
				}
			}
		}
		else
		{
			if (indialog)
			{
				//Do some last cleanup here.
				currentDialogItem.fieldList.append(currDialogFieldList);
				currDialogFieldList.clear();
				menu.dialoglist.append(currentDialogItem);
				currentDialogItem = DialogItem();
				indialog = false;
			}
		}
		if (section == 4)
		{
			if (!line.startsWith(";"))
			{
				QStringList linesplit = line.split("=");
				if (linesplit.size() > 1)
				{
					if (linesplit[0].trimmed() == "menu")
					{
						if (inmenu)
						{
							menu.menulist.append(currentMenuItem);
							currentMenuItem = MenuItem();
						}
						inmenu = true;
						QStringList menusplit = linesplit[1].split(",");
						if (menusplit.size() >= 1)
						{
							//Has a title
							currentMenuItem.title = menusplit[0].replace("\"","").trimmed();
						}
						if (menusplit.size() >= 2)
						{
							//Has a condition
							currentMenuItem.condition = menusplit[1].replace("\"","").trimmed();
						}
						menuname = linesplit[1].replace("\"","").trimmed();
					}
					else if (linesplit[0].trimmed() == "subMenu")
					{
						//currentMenuItem.subMenuList.append();
						SubMenuItem item;
						item.is_seperator = false;
						QStringList submenusplit = linesplit[1].split(",");
						if (linesplit[1].contains("std_separator"))
						{
							item.title = "-------------------------";
							item.is_seperator = true;
						}
						else
						{
							if (submenusplit.size() >= 1)
							{
								//Has a variable name
								item.variable = submenusplit[0].replace("\"","").trimmed();
							}
							else
							{
								//no split!
								qDebug() << "Sub menu has no split:" << linesplit[1];
							}
							if (submenusplit.size() >= 2)
							{
								//Has a title
								item.title = submenusplit[1].replace("\"","").trimmed();
							}
							if (submenusplit.size() >= 3)
							{
								//Has a var
								item.var = submenusplit[2].replace("\"","").trimmed();
							}
							if (submenusplit.size() >= 4)
							{
								//Has a condition
								item.condition = submenusplit[3].replace("\"","").trimmed();
							}
						}
						currentMenuItem.subMenuList.append(item);
					}
				}
			}
		}
		else
		{
			if (inmenu)
			{
				//Do some last cleanup here
				menu.menulist.append(currentMenuItem);
				currentMenuItem = MenuItem();
				inmenu = false;
			}
		}
		if (section == 3)
		{
			//OutputChannels, Datalog definitions
			QStringList linesplit = line.split("=");
			if (linesplit.size() > 1)
			{
				QStringList linevalsplit = linesplit[1].split(",");
				if (linevalsplit.size() > 5)
				{
					if (linevalsplit[0].trimmed() == "scalar")
					{
						scalarclass scalar;
						//qDebug() << linevalsplit[2].trimmed() << linevalsplit[4].trimmed();
						scalar.offset = linevalsplit[2].trimmed().toInt();
						if (linevalsplit[4].trimmed().startsWith("."))
						{
							scalar.scale = QString("0").append(linevalsplit[4].trimmed()).toFloat();
						}
						else
						{
							scalar.scale = linevalsplit[4].trimmed().toFloat();
						}
						if (linevalsplit[5].trimmed().startsWith("."))
						{
							scalar.translate = QString("0").append(linevalsplit[5].trimmed()).toFloat();
						}
						else
						{
							scalar.translate = linevalsplit[5].trimmed().toFloat();
						}
						//scalarMap[linesplit[0].trimmed()] = scalar;

						if (linevalsplit[1].trimmed() == "S16")
						{
							scalar.size = 2;
							scalar.signedval = true;
						}
						else if (linevalsplit[1].trimmed() == "U08")
						{
							scalar.size = 1;
							scalar.signedval = false;
						}
						else if (linevalsplit[1].trimmed() == "U16")
						{
							scalar.size = 2;
							scalar.signedval = false;
						}
						else if (linevalsplit[1].trimmed() == "U32")
						{
							scalar.size = 4;
							scalar.signedval = false;
						}
						else if (linevalsplit[1].trimmed() == "S32")
						{
							scalar.size = 4;
							scalar.signedval = true;
						}
						scalar.unit = linevalsplit[3].trimmed().mid(1,linevalsplit[3].trimmed().length()-2);
						//dataLogList.append(scalar);
						DataField f(linesplit[0].trimmed(),"",scalar.offset,scalar.size,scalar.scale,scalar.translate);
						dataLogMap.append(f);
						//dataLogMap[linesplit[0].trimmed()] = scalar;
					}
				}
			}
		}
		if (section == 1)
		{
			if (line.trimmed().startsWith("page=") || line.trimmed().startsWith("page "))
			{
				qDebug() << "Page found:" << line.replace("\r","").replace("\n","");
				if (pagenum != "")
				{
					pageMap[pagenum] = scalarMap;
					pageArrayMap[pagenum] = arrayMap;
					qDebug() << "Page saved:" << pagenum;
					scalarMap.clear();
					arrayMap.clear();
				}
				//New page
				//Page lineb
				if (line.split("=").size() > 0)
				{
					pagenum = line.split("=")[1];
					pagenum = pagenum.mid(0,pagenum.indexOf(";")).trimmed();
					qDebug() << "New pagenum" << pagenum;

					pagenumint = pagenum.toInt();
					//QStringList split = pageIdent[pagenumint].split("\\");
					//int pagesize = pageSize[pagenumint];
					//bool ok = false;
					//QString pagestr = QString("0" + split[split.size()-1].replace("\"",""));
					//pagenumint = pagestr.toInt(&ok,16);
					//qDebug() << ok << pagenum << i.key().toInt() << pagestr;
					QList<ConfigBlock> blocklist;
					//m_pageSizeMap[pagenumint] = pagesize;
				}
				else
				{
					qDebug() << "Page iwthout" << line;
				}
				//qDebug() << "Page Num:" << pagenum;
			}
			else if (!line.startsWith(";"))
			{
				if (line.trimmed().startsWith("pageIdentifier"))
				{
					//Page identifiers, these are important!
					QString idents = line.trimmed().split("=")[1];
					QStringList identsplit = idents.split(",");
					pageIdent.append("");
					for (int j=0;j<identsplit.size();j++)
					{
						pageIdent.append(identsplit[j].trimmed());
					}
				}
				if (line.trimmed().startsWith("pageSize"))
				{
					QString pagesizes = line.trimmed().split("=")[1];
					QStringList sizesplit = pagesizes.split(",");
					int pageint = 0;
					for (int j=0;j<sizesplit.size();j++)
					{
						pageint = sizesplit[j].trimmed().toInt();
						pageSize.append(pageint);
					}
					if (!m_pageBufferMap.contains(0))
					{
						m_pageBufferMap[0] = QByteArray();
						m_pageBufferFilledMap[0] = 0;
					}
					m_pageBufferMap[0].resize(pageint);
				}
				if (line.trimmed().startsWith("pageActivate"))
				{
					//Activation commands for pages.
					QString pagesizes = line.trimmed().split("=")[1];
					QStringList sizesplit = pagesizes.split(",");
					for (int j=0;j<sizesplit.size();j++)
					{
						QString value = sizesplit[j].trimmed();
						value = value.replace("\"","");
						value = QString::fromUtf8(value.mid(1).toStdString().c_str());
						pageActivate.append(value);
					}
				}
				QStringList linesplit = line.split("=");
				if (linesplit.size() > 1)
				{
					QStringList linevalsplit = linesplit[1].split(",");
					if (linevalsplit.size() > 5)
					{
						//qDebug() << linesplit[1];
						if (linevalsplit[0].trimmed() == "scalar")
						{
							//Scalar
							scalarclass scalar;
							//qDebug() << linevalsplit[2].trimmed() << linevalsplit[4].trimmed();
							scalar.offset = linevalsplit[2].trimmed().toInt();
							//if (linevalsplit[0].trimmed() == "scalar")
							//{
								scalar.type = "scalar";
								if (linevalsplit[4].trimmed().startsWith("."))
								{
									scalar.scale = QString("0").append(linevalsplit[4].trimmed()).toFloat();
								}
								else
								{
									scalar.scale = linevalsplit[4].trimmed().toFloat();
								}
								if (linevalsplit[5].trimmed().startsWith("."))
								{
									scalar.translate = QString("0").append(linevalsplit[5].trimmed()).toFloat();
								}
								else
								{
									scalar.translate = linevalsplit[5].trimmed().toFloat();
								}
							//}
							//else if (linevalsplit[0].trimmed() == "array")
							//{
							//	scalar.type = "array";
							//}
							//else if (linevalsplit[0].trimmed() == "bits")
							//{
							//	scalar.type == "bits";
							//}
							if (linevalsplit[1].trimmed() == "S16")
							{
								scalar.size = 2;
								scalar.signedval = true;
							}
							else if (linevalsplit[1].trimmed() == "U08")
							{
								scalar.size = 1;
								scalar.signedval = false;
							}
							else if (linevalsplit[1].trimmed() == "U16")
							{
								scalar.size = 2;
								scalar.signedval = false;
							}
							else if (linevalsplit[1].trimmed() == "U32")
							{
								scalar.size = 4;
								scalar.signedval = false;
							}
							else if (linevalsplit[1].trimmed() == "S32")
							{
								scalar.size = 4;
								scalar.signedval = true;
							}
							scalarMap[linesplit[0].trimmed()] = scalar;

							TSBPConfigData *data = new TSBPConfigData();
							connect(data,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SLOT(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
							connect(data,SIGNAL(pageBurnRequested(uint)),this,SLOT(pageBurnRequested(uint)));
							data->setElementSize(scalar.size);
							data->setSize(1);
							data->setName(linesplit[0].trimmed());
							data->setOffset(scalar.offset);
							data->setLocationId(pagenumint);
							data->setType(ConfigData::VALUE);
							QList<QPair<QString,double> > calclist;
							calclist.append(QPair<QString,double>("add",scalar.translate));
							calclist.append(QPair<QString,double>("mult",scalar.scale));
							data->setCalc(calclist);
							m_configDataMap[linesplit[0].trimmed()] = data;
							m_configNameList.append(linesplit[0].trimmed());
							//pageMap[pagenum][linesplit[0].trimmed()] = scalar;
						} //if (linevalsplit[0].trimmed() == "scalar")
						else if (linevalsplit[0].trimmed() == "bits")
						{
							TSBPConfigData *data = new TSBPConfigData();
							connect(data,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SLOT(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
							connect(data,SIGNAL(pageBurnRequested(uint)),this,SLOT(pageBurnRequested(uint)));
							unsigned int offset = linevalsplit[2].trimmed().toInt();
							QString bits = linevalsplit[3].trimmed().replace("[","").replace("]","");
							unsigned int min = bits.split(":")[0].toInt();
							unsigned int max = bits.split(":")[1].toInt();
							QStringList bitarray;
							for (int i=4;i<linevalsplit.size();i++)
							{
								bitarray.append(linevalsplit[i].trimmed().replace("\"",""));
							}
							data->setEnumValues(bitarray);
							data->setEnumBits(min,max);
							data->setType(ConfigData::ENUM);
							data->setOffset(offset);
							data->setElementSize(1);
							data->setSize(1);
							data->setName(linesplit[0].trimmed());
							data->setLocationId(pagenumint);
							m_configDataMap[linesplit[0].trimmed()] = data;
							m_configNameList.append(linesplit[0].trimmed());
						}
						else if (linevalsplit[0].trimmed() == "array")
						{
							//arrayMap
							arrayclass array;
							array.offset = linevalsplit[2].trimmed().toInt();
							if (linevalsplit[5].trimmed().startsWith("."))
							{
								array.scale = QString("0").append(linevalsplit[5].trimmed()).toFloat();
							}
							else
							{
								array.scale = linevalsplit[5].trimmed().toFloat();
							}
							if (linevalsplit[6].trimmed().startsWith("."))
							{
								array.translate = QString("0").append(linevalsplit[6].trimmed()).toFloat();
							}
							else
							{
								array.translate = linevalsplit[6].trimmed().toFloat();
							}
							array.isfloat = false;
							array.unit = linevalsplit[1].trimmed();
							QString format = linevalsplit[3].trimmed();
							if (linevalsplit[1].trimmed() == "S16")
							{
								array.isSigned = true;
								array.elementSize = 2;
							}
							else if (linevalsplit[1].trimmed() == "U08")
							{
								array.isSigned = false;
								array.elementSize = 1;
							}
							else if (linevalsplit[1].trimmed() == "U16")
							{
								array.isSigned = false;
								array.elementSize = 2;
							}
							else if (linevalsplit[1].trimmed() == "U32")
							{
								array.isSigned = false;
								array.elementSize = 4;
							}
							else if (linevalsplit[1].trimmed() == "S32")
							{
								array.isSigned = true;
								array.elementSize = 4;
							}
							else if (linevalsplit[1].trimmed() == "F32")
							{
								array.isSigned = true;
								array.elementSize = 4;
								array.isfloat = true;
							}
							if (format.contains("x"))
							{
								//It's a 3d array.
								array.is2d = false;
								array.xdim = format.mid(1,format.size()-2).split("x")[0].toInt();
								array.ydim = format.mid(1,format.size()-2).split("x")[1].toInt();
								array.size = array.xdim * array.ydim;
							}
							else
							{
								array.is2d = true;

								array.xdim = format.mid(1,format.size()-2).trimmed().toInt();
								array.size = array.xdim;
								//data->setAxisMetaData(elementsize,array.scale,array.translate,issigned);
								//m_2dTableData[linesplit[0].trimmed()] = data;
							}
							//7,8 min,max
							if (linesplit.size() > 8)
							{
								array.min = linesplit[7].trimmed().toFloat();
								array.max = linesplit[8].trimmed().toFloat();
							}
							else
							{
								array.min = -65535;
								array.max = 65535;
							}
							arrayMap[linesplit[0].trimmed()] = array;
							//pageArrayMap[pagenum][linesplit[0].trimmed()] = array;
						}
					}
				}
			}
		}
		if (section == 6)
		{
			if (!line.trimmed().startsWith(";") && !line.trimmed().startsWith("#"))
			{
				//Not a comment
				QStringList linesplit = line.trimmed().split("=");
				if (linesplit.size() > 0)
				{
					if (linesplit[0].trimmed() == "table")
					{
						if (intable)
						{
							//Next table
						}
						else
						{
							intable = true;
						}
						QStringList tabledescsplit = linesplit[1].trimmed().split(",");
						if (tabledescsplit.size() > 0)
						{
							QString tablevar = tabledescsplit[0].trimmed();
							QString tablemap = tabledescsplit[1].trimmed();
							QString tablename = tabledescsplit[2].trimmed();
							currenttablevar = tablevar;
							Table *table = new Table();
							tableMap[tablevar] = table;
							tableMap[tablevar]->title = tablename;
							tableMap[tablevar]->mapid = tablemap;
						}
					}
					else if (linesplit[0].trimmed() == "xBins")
					{
						tableMap[currenttablevar]->xbin = linesplit[1].trimmed();
					}
					else if (linesplit[0].trimmed() == "yBins")
					{
						tableMap[currenttablevar]->ybin = linesplit[1].trimmed();
					}
					else if (linesplit[0].trimmed() == "zBins")
					{
						tableMap[currenttablevar]->zbin = linesplit[1].trimmed();
					}
					else if (linesplit[0].trimmed() == "upDownLabel")
					{
						QStringList updownsplit = linesplit[1].trimmed().split(",");
						if (updownsplit.size() > 1)
						{
							tableMap[currenttablevar]->upDownLabel = QPair<QString,QString>(updownsplit[0].trimmed().replace("\"",""),updownsplit[1].trimmed().replace("\"",""));
						}
					}
					else if (linesplit[0].trimmed() == "gridHeight")
					{
						tableMap[currenttablevar]->gridHeight = linesplit[1].trimmed().toFloat();
					}
					else if (linesplit[0].trimmed() == "gridOrient")
					{
					}

				}
			}
		}
		else if (section == 7)
		{
			if (!line.trimmed().startsWith(";") && !line.trimmed().startsWith("#"))
			{
				//Not a comment
				QStringList linesplit = line.trimmed().split("=");
				if (linesplit.size() > 0)
				{
					if (linesplit[0].trimmed() == "curve")
					{
						QStringList curvedescsplit = linesplit[1].trimmed().split(",");
						if (curvedescsplit.size() > 1)
						{
							QString curvevar = curvedescsplit[0].trimmed();
							QString curvename = curvedescsplit[1].trimmed();
							currentcurvevar = curvevar;
							//Table *table = new Table();
							Curve *curve = new Curve();
							curveMap[curvevar] = curve;
							curveMap[curvevar]->title = curvename;
						}
					}
					else if (linesplit[0].trimmed() == "xBins")
					{
						curveMap[currentcurvevar]->xbin = linesplit[1].trimmed();
					}
					else if (linesplit[0].trimmed() == "yBins")
					{
						curveMap[currentcurvevar]->ybin = linesplit[1].trimmed();
					}
					else if (linesplit[0].trimmed() == "columnLabel")
					{
						QStringList columnamesplit = linesplit[1].split(",");
						curveMap[currentcurvevar]->columnLabels = QPair<QString,QString>(columnamesplit[0].trimmed().replace("\"",""),columnamesplit[1].trimmed().replace("\"",""));
					}
				}
			}
		}
	}


	//Populate the saved page array map, with proper page keys.

//	for (int i=0;i<arrayMap.keys().size();i++)
//	{
//		int pagenum = 0;
//		m_pageArrayMap[pagenum] = arrayMap[arrayMap.keys()[i]];
//	}
	/*for (int i=0;i<pageArrayMap.keys().size();i++)
	{
		int pagenum = pageArrayMap.keys()[i].toInt();
		QStringList split = pageIdent[pagenum].split("\\");
		bool ok = false;
		QString pagestr = QString("0" + split[split.size()-1].replace("\"",""));
		pagenum = pagestr.toInt(&ok,16);
		m_pageArrayMap[QString::number(pagenum)] = pageArrayMap.value(pageArrayMap.keys()[i]);
	}*/


	//Setup the 3d tables
	for (QMap<QString,Table*>::const_iterator i=tableMap.constBegin();i!=tableMap.constEnd();i++)
	{
		//X is axis
		//Y is values
		TSBPTable3DData *curve = new TSBPTable3DData();
		connect(curve,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SLOT(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
		connect(curve,SIGNAL(burnPage(uint)),this,SLOT(pageBurnRequested(uint)));
		QString xbin = i.value()->xbin;
		QString ybin = i.value()->ybin;
		QString zbin = i.value()->zbin;
		if (xbin.indexOf(",") != -1)
		{
			xbin = xbin.split(",")[0].trimmed();
		}
		if (ybin.indexOf(",") != -1)
		{
			ybin = ybin.split(",")[0].trimmed();
		}
		if (zbin.indexOf(",") != -1)
		{
			zbin = zbin.split(",")[0].trimmed();
		}
		//QMap<QString,arrayclass>
		for (QMap<QString,arrayclass>::const_iterator k = arrayMap.constBegin();k != arrayMap.constEnd();k++)
		{
			if (k.key() == xbin)
			{
				curve->setXAxisMetaData(0,k.value().offset,k.value().size,k.value().elementSize,k.value().scale,k.value().translate,k.value().isSigned,k.value().isfloat,k.value().min,k.value().max);
			}
			if (k.key() == ybin)
			{
				curve->setYAxisMetaData(0,k.value().offset,k.value().size,k.value().elementSize,k.value().scale,k.value().translate,k.value().isSigned,k.value().isfloat,k.value().min,k.value().max);
			}
			if (k.key() == zbin)
			{
				curve->setValueMetaData(0,k.value().offset,k.value().size,k.value().elementSize,k.value().scale,k.value().translate,k.value().isSigned,k.value().isfloat,k.value().xdim,k.value().ydim,k.value().min,k.value().max);
			}

		}
		for (QMap<QString,QMap<QString,arrayclass> >::const_iterator k = m_pageArrayMap.constBegin();k!=m_pageArrayMap.constEnd();k++)
		{
			for (QMap<QString,arrayclass>::const_iterator j = k.value().constBegin();j!=k.value().constEnd();j++)
			{

				if (j.key() == xbin)
				{
					curve->setXAxisMetaData(k.key().toInt(),j.value().offset,j.value().size,j.value().elementSize,j.value().scale,j.value().translate,j.value().isSigned,j.value().isfloat,j.value().min,j.value().max);
					if (!m_pageTo3DTableList.contains(k.key().toInt()))
					 {
						 m_pageTo3DTableList[k.key().toInt()] = QList<TSBPTable3DData*>();
					 }
					 if (!m_pageTo3DTableList.value(k.key().toInt()).contains(curve))
					 {
						 m_pageTo3DTableList[k.key().toInt()].append(curve);
					 }
				}
				if (j.key() == ybin)
				{
					curve->setYAxisMetaData(k.key().toInt(),j.value().offset,j.value().size,j.value().elementSize,j.value().scale,j.value().translate,j.value().isSigned,j.value().isfloat,j.value().min,j.value().max);
					if (!m_pageTo3DTableList.contains(k.key().toInt()))
					 {
						 m_pageTo3DTableList[k.key().toInt()] = QList<TSBPTable3DData*>();
					 }
					 if (!m_pageTo3DTableList.value(k.key().toInt()).contains(curve))
					 {
						 m_pageTo3DTableList[k.key().toInt()].append(curve);
					 }
				}
				if (j.key() == zbin)
				{
					curve->setValueMetaData(k.key().toInt(),j.value().offset,j.value().size,j.value().elementSize,j.value().scale,j.value().translate,j.value().isSigned,j.value().isfloat,j.value().xdim,j.value().ydim,j.value().min,j.value().max);
					if (!m_pageTo3DTableList.contains(k.key().toInt()))
					{
						m_pageTo3DTableList[k.key().toInt()] = QList<TSBPTable3DData*>();
					}
					if (!m_pageTo3DTableList.value(k.key().toInt()).contains(curve))
					{
						m_pageTo3DTableList[k.key().toInt()].append(curve);
					}
				}
			}
		}
		m_3dTableData[i.key()] = curve;
	}

	//Setup the 2d tables
	for (QMap<QString,Curve*>::const_iterator i=curveMap.constBegin();i!=curveMap.constEnd();i++)
	{
		//X is axis
		//Y is values
		TSBPTable2DData *curve = new TSBPTable2DData();
		connect(curve,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SLOT(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
		connect(curve,SIGNAL(burnPage(uint)),this,SLOT(pageBurnRequested(uint)));
		QString xbin = i.value()->xbin;
		QString ybin = i.value()->ybin;
		if (xbin.indexOf(",") != -1)
		{
			xbin = xbin.split(",")[0].trimmed();
		}
		if (ybin.indexOf(",") != -1)
		{
			ybin = ybin.split(",")[0].trimmed();
		}
		curve->setAxisLabel(i.value()->columnLabels.first);
		curve->setValueLabel(i.value()->columnLabels.second);
		curve->setAxisVariable(xbin);
		curve->setValueVariable(ybin);
		bool foundAxisMeta = false;
		bool foundValueMetaData = false;
		for (QMap<QString,arrayclass>::const_iterator k = arrayMap.constBegin();k != arrayMap.constEnd();k++)
		{
			if (k.key() == xbin)
			{
				curve->setAxisMetaData(0,k.value().offset,k.value().size,k.value().elementSize,k.value().scale,k.value().translate,k.value().isSigned,k.value().isfloat);
			}
			if (k.key() == ybin)
			{
				curve->setValueMetaData(0,k.value().offset,k.value().size,k.value().elementSize,k.value().scale,k.value().translate,k.value().isSigned,k.value().isfloat);
			}

		}
		for (QMap<QString,QMap<QString,arrayclass> >::const_iterator k = m_pageArrayMap.constBegin();k!=m_pageArrayMap.constEnd();k++)
		{
			for (QMap<QString,arrayclass>::const_iterator j = k.value().constBegin();j!=k.value().constEnd();j++)
			{

				if (j.key() == xbin)
				{
					foundAxisMeta = true;
					curve->setAxisMetaData(k.key().toInt(),j.value().offset,j.value().size,j.value().elementSize,j.value().scale,j.value().translate,j.value().isSigned,j.value().isfloat);
					if (!m_pageTo2DTableList.contains(k.key().toInt()))
					 {
						 m_pageTo2DTableList[k.key().toInt()] = QList<TSBPTable2DData*>();
					 }
					 if (!m_pageTo2DTableList.value(k.key().toInt()).contains(curve))
					 {
						 m_pageTo2DTableList[k.key().toInt()].append(curve);
					 }
				}
				if (j.key() == ybin)
				{
					foundValueMetaData = true;
					curve->setValueMetaData(k.key().toInt(),j.value().offset,j.value().size,j.value().elementSize,j.value().scale,j.value().translate,j.value().isSigned,j.value().isfloat);
					if (!m_pageTo2DTableList.contains(k.key().toInt()))
					{
						m_pageTo2DTableList[k.key().toInt()] = QList<TSBPTable2DData*>();
					}
					if (!m_pageTo2DTableList.value(k.key().toInt()).contains(curve))
					{
						m_pageTo2DTableList[k.key().toInt()].append(curve);
					}
				}
			}
		}
		//if (foundValueMetaData && foundAxisMeta)
		{
			m_2dTableData[i.key()] = curve;
		}
	}





	QMap<QString,QList<ConfigBlock> > configmap;
	for (QMap<QString,QMap<QString,scalarclass> >::const_iterator i = pageMap.constBegin();i!=pageMap.constEnd();i++)
	{
		int pagenum = i.key().toInt();
		qDebug() << "Page:" << pagenum;
		//QStringList split = pageIdent[pagenum].split("\\");
		int pagesize = pageSize[pagenum];
		//bool ok = false;
		//QString pagestr = QString("0" + split[split.size()-1].replace("\"",""));
		//pagenum = pagestr.toInt(&ok,16);
		//qDebug() << ok << pagenum << i.key().toInt() << pagestr;
		QList<ConfigBlock> blocklist;
		m_pageSizeList.append(pagesize);
		m_pageReqStringList.append(pageActivate[pagenum]);
		for (QMap<QString,scalarclass>::const_iterator j = i.value().constBegin();j!=i.value().constEnd();j++)
		{
			TSBPConfigData *data = new TSBPConfigData();
			data->setElementSize(j.value().size);
			data->setSize(1);
			data->setName(j.key());
			data->setOffset(j.value().offset);
			data->setLocationId(pagenum);
			data->setType(ConfigData::VALUE);

			//ConfigBlock b;
			//b.setLocationId(pagenum);
			//b.setElementSize(j.value().size);
			//b.setSize(1);
			//b.setName(j.key());
			//b.setOffset(j.value().offset);
			QList<QPair<QString,double> > calclist;
			calclist.append(QPair<QString,double>("add",j.value().translate));
			calclist.append(QPair<QString,double>("mult",j.value().scale));
			data->setCalc(calclist);
			//b.setCalc(calclist);
			//qDebug() << "Type:" << j.value().type;
			//qDebug() << "Page:" << i.key() << "Scalar:" << j.key() << j.value().offset << j.value().scale << j.value().translate;
			//b.setType(j.value().type);
			//configmap[j.key()] = b;
			//blocklist.append(b);
			m_configDataMap[j.key()] = data;
			m_configNameList.append(j.key());


		}
		configmap[QString::number(pagenum)] = blocklist;
	}
	for (QMap<QString,QMap<QString,arrayclass> >::const_iterator i = pageArrayMap.constBegin();i!=pageArrayMap.constEnd();i++)
	{
		for (QMap<QString,arrayclass>::const_iterator j = i.value().constBegin();j!=i.value().constEnd();j++)
		{
			if (j.value().is2d)
			{
			}
			else
			{
			}

		}

	}
	//m_dsataDecoder->setDataDefinitions(dataLogMap);
	m_memoryMetaData->passConfigData(configmap);
	m_dataDecoder->setDataDefinitions(dataLogMap);
	//const QMap<QString,QList<ConfigBlock> > configMetaData();
	//QList<QPair<QString,QString> > internaldialoglist;
	/*for (QMap<QString,QList<QPair<QString,QString> > >::const_iterator i = dialogmap.constBegin();i!=dialogmap.constEnd();i++)
	{
		qDebug() << "dialog:" << i.key();
		for (int j=0;j<i.value().size();j++)
		{
			qDebug() << "Field:" << i.value()[j].first << i.value()[j].second;
			internaldialoglist.append(i.value()[j]);
		}
		menu.dialogmap[i.key()] = internaldialoglist;
	}
	QMap<QString,QList<ConfigBlock> > config;
	QList<QString> internalmenulist;
	for(QMap<QString,QList<QPair<QString,QString> > >::const_iterator i = menulist.constBegin();i != menulist.constEnd();i++)
	{
		qDebug() << "Menu" << i.key() << "size" << i.value().size();
		QList<ConfigBlock> blocks;
		internalmenulist.clear();
		for (int j=0;j<i.value().size();j++)
		{
			menu.menunamemap[i.value()[j].second] = i.value()[j].first;
			internalmenulist.append(i.value()[j].first);
			if (dialogmap.contains(i.value()[j].first))
			{
				//It's a dialog!
			}
			if (configmap.contains(i.value()[j].first))
			{
				//It's a scalar!
				blocks.append(configmap[i.value()[j].first]);
			}
			qDebug() << "subMenu:" << i.value()[j].first << i.value()[j].second;
		}
		menu.menulist.append(QPair<QString,QList<QString> >(i.key(),internalmenulist));
		config[i.key()] = blocks;
	}*/

	//_memoryMetaData->passConfigData(config);
	for (int i=0;i<menu.dialoglist.size();i++)
	{
		for (int j=0;j<menu.dialoglist.at(i).panelList.size();j++)
		{
			QString varname = menu.dialoglist.at(i).panelList.at(j).second;
			DialogItem dlogitem = menu.dialoglist.at(i);
			for (int k=0;k<menu.dialoglist.size();k++)
			{
				if (menu.dialoglist.at(k).title == varname)
				{
					dlogitem.panelMap.insert(menu.dialoglist.at(i).panelList.at(j).second,menu.dialoglist.at(k));
				}
			}
			menu.dialoglist.replace(i,dlogitem);
		}
	}
	m_memoryMetaData->setMenuMetaData(menu);
}
QStringList TSBPComms::splitline(QString csvline)
{
	QString current;
	QStringList result;
	bool inquotes = false;
	for (int i=0;i<csvline.size();i++)
	{
		if (csvline[i] == '\"')
		{
			if (inquotes)
			{
				inquotes = false;
			}
			else
			{
				inquotes = true;
			}
		}
		else if (csvline[i] == ',' && !inquotes)
		{
			result += current;
			current = "";
		}
		else if (csvline[i] == ';' && !inquotes)
		{
			//Everything else is a comment
			i = csvline.size();
		}
		else
		{
			current += csvline[i];
		}
	}
	if (current != "")
	{
		result += current;
	}
	return result;
}

void TSBPComms::saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size)
{
	updateBlockInFlash(locationid,offset,size,data);
	if (!m_dirtyPageList.contains(locationid))
	{
		m_dirtyPageList.append(locationid);
		emit memoryDirty();
	}

}
void TSBPComms::pageBurnRequested(unsigned int page)
{
	if (!m_savePageList.contains(page))
	{
		m_savePageList.append(page);
		if (!savePageTimer->isActive())
		{
			savePageTimer->start(2000);
		}
		else
		{
			savePageTimer->stop();
			savePageTimer->start(2000);
		}
	}
}
void TSBPComms::savePageTimerTick()
{
	qDebug() << "Saving all pages";
	savePageTimer->stop();
	for (int i=0;i<m_savePageList.size();i++)
	{
		qDebug() << "Saving page:" << m_savePageList[i];
		if (m_dirtyPageList.contains(m_savePageList[i]))
		{
			m_dirtyPageList.removeOne(m_savePageList[i]);
		}
		burnBlockFromRamToFlash(m_savePageList[i],0,0);
	}
	m_savePageList.clear();
	if (m_dirtyPageList.size() == 0)
	{
		emit memoryClean();
	}

}

void TSBPComms::passLogger(QsLogging::Logger *log)
{
	QsLogging::Logger::instance(log);
	QLOG_DEBUG() << "Logging from the plugin!!!";
}
int TSBPComms::startBenchTest(unsigned char eventspercycle, unsigned short numcycles, unsigned short ticksperevent, QVariantList pineventmask, QVariantList pinmode)
{
	Q_UNUSED(eventspercycle)
	Q_UNUSED(numcycles)
	Q_UNUSED(ticksperevent)
	Q_UNUSED(pineventmask)
	Q_UNUSED(pinmode)
	return -1; //Not supported
}

int TSBPComms::stopBenchTest()
{
	return -1; //Not supported

}

int TSBPComms::bumpBenchTest(unsigned char cyclenum)
{
	Q_UNUSED(cyclenum)
	return -1; //Not supported

}

ConfigData* TSBPComms::getConfigData(QString name)
{
	if (m_configDataMap.contains(name))
	{
		return m_configDataMap[name];
	}
	return 0;
}

QList<QString> TSBPComms::getConfigList()
{
	return m_configNameList;
}

void TSBPComms::writeAllRamToRam()
{

}
Table2DData* TSBPComms::get2DTableData(QString varname)
{
	if (m_2dTableData.contains(varname))
	{
		return m_2dTableData.value(varname);
	}
	else
	{
		return 0;
	}
}

Table2DData* TSBPComms::get2DTableData(unsigned short locationid)
{
	Q_UNUSED(locationid)
	return 0;
}

Table3DData* TSBPComms::get3DTableData(unsigned short locationid)
{
	Q_UNUSED(locationid)
	return 0;
}
Table3DData* TSBPComms::get3DTableData(QString locationname)
{
	if (m_3dTableData.contains(locationname))
	{
		return m_3dTableData.value(locationname);
	}
	else
	{
		return 0;
	}
}

RawData* TSBPComms::getRawData(unsigned short locationid)
{
	Q_UNUSED(locationid)
	return 0;
}
void TSBPComms::startInterrogation()
{
	if (!m_interrogateInProgress)
	{
		//m_interrogatePacketList.clear();
		m_interrogateInProgress = true;
		for (int i=0;i<m_pageReqStringList.size();i++)
		{
			qDebug() << "Requesting page:" << i << m_pageReqStringList.at(i) << m_pageSizeList.at(i);
		}
		interrogateTaskStart("Ecu Info",getFirmwareVersion());
		QByteArray pagereq;
		pagereq.append("R");
		pagereq.append((char)0x0);
		pagereq.append((char)0x0);
		//pagereq.append((char)0x0);
		//pagereq.append((char)0x0);
		//pagereq.append((char)0x4E);
		//pagereq.append((char)0x20);
		interrogateTaskStart("Location ID 1",requestPage(pagereq,288));
		//interrogateTaskStart("Location ID 2",requestPage("P\002",64));
		//interrogateTaskStart("Location ID 3",requestPage("P\003",288));
		//interrogateTaskStart("Location ID 4",requestPage("P\004",64));
		//interrogateTaskStart("Location ID 5",requestPage("P\005",288));
		//interrogateTaskStart("Location ID 6",requestPage("P\006",64));
		//interrogateTaskStart("Location ID 7",requestPage("P\007",64));
		//interrogateTaskStart("Location ID 10",requestPage("P\010",160));
		//interrogateTaskStart("Location ID 11",requestPage("P\011",192));
		//m_interrogatePacketList.append(getFirmwareVersion());
		//m_interrogatePacketList.append(getInterfaceVersion());
		//m_interrogatePacketList.append(getCompilerVersion());
		//m_interrogatePacketList.append(getDecoderName());
		//m_interrogatePacketList.append(getFirmwareBuildDate());
		//m_interrogatePacketList.append(getMaxPacketSize());
		//m_interrogatePacketList.append(getOperatingSystem());
		//m_interrogatePacketList.append(getLocationIdList(0x00,0x00));
		//m_interrogateTotalCount=1;
	}
}

void TSBPComms::stop()
{
	m_threadRun = false;
}

void TSBPComms::setLogsEnabled(bool enabled)
{
	Q_UNUSED(enabled)
	//Q_UNUSED(locationid)
}
MemoryMetaData *TSBPComms::getMetaParser()
{
	return m_memoryMetaData;
}

DataPacketDecoder *TSBPComms::getDecoder()
{
	return m_dataDecoder;
}

Table3DData *TSBPComms::getNew3DTableData()
{
	return 0;
}

Table2DData *TSBPComms::getNew2DTableData()
{
	return 0;
}

void TSBPComms::setLogDirectory(QString dir)
{
	Q_UNUSED(dir)
}
int TSBPComms::enableDatalogStream()
{
	return 0;
}

int TSBPComms::disableDatalogStream()
{
	return 0;
}

void TSBPComms::setPort(QString portname)
{
	m_portName = portname;
}

void TSBPComms::setBaud(int baudrate)
{
	Q_UNUSED(baudrate)
}

void TSBPComms::setLogFileName(QString filename)
{
	Q_UNUSED(filename)
}


void TSBPComms::sendPacket(RequestClass req)
{
	//qDebug() << "sendPacket called";
	m_waitingForPacketResponse = true;
	if (req.type == RETRIEVE_PAGE)
	{
		//Retrieve page!
		QByteArray pagereqstr = req.args.at(0).toByteArray(); //Page Name
		int offset = req.args.at(1).toInt(); //Size
		int size = req.args.at(2).toInt(); //Size
		qDebug() << "Sending page retrieve" << pagereqstr << offset << size;
		m_currentRequest = req;
		//m_serialPort->requestPacket(pagename.toLocal8Bit(),0);
		//m_serialPort->requestPacket(QString("V").toLocal8Bit(),size);
		pagereqstr.append(offset & 0xFF);
		pagereqstr.append((offset >> 8) & 0xFF);
		pagereqstr.append(size & 0xFF);
		pagereqstr.append((size >> 8) & 0xFF);
		uint32_t crc = Crc32_ComputeBuf(0,pagereqstr.data(),pagereqstr.size());
		qDebug() << "Pagestr:" << pagereqstr.toHex();
		QByteArray packet;
		packet.append((char)0x00);
		packet.append(pagereqstr.length());
		packet.append(pagereqstr);
		packet.append(crc >> 24);
		packet.append(crc >> 16);
		packet.append(crc >> 8);
		packet.append(crc >> 0);
		m_serialPort->requestPacket(packet,0);
		//m_serialPort->writePacket(QString().toLatin1());
	}
	else if (req.type == GET_FIRMWARE_VERSION)
	{
		qDebug() << "Getting firmware version";
		m_currentRequest = req;
		//m_serialPort->requestPacket(QByteArray("H"),0);
		QByteArray req = "S";
		uint32_t crc = Crc32_ComputeBuf(0,req.data(),req.size());
		QByteArray packet;
		packet.append((char)0x00);
		packet.append(0x01);
		packet.append(req);
		packet.append(crc >> 24);
		packet.append(crc >> 16);
		packet.append(crc >> 8);
		packet.append(crc >> 0);
		m_serialPort->requestPacket(packet,0);
	}
	else if (req.type == UPDATE_BLOCK_IN_FLASH)
	{
		//cmd8 offset16 count16 valueN
		//req.type = UPDATE_BLOCK_IN_FLASH;
		//req.sequencenumber = currentPacketNum++;
		//req.addArg(location,2);
		//req.addArg(offset,2);
		//req.addArg(size,2);
		//req.addArg(data,data.size());
		//m_reqList.append(req);
		m_currentRequest = req;

		int offset = req.args.at(1).toInt(); //Size
		int size = req.args.at(2).toInt(); //Size
		QByteArray data = req.args.at(3).toByteArray();

		//m_serialPort->requestPacket(QByteArray("H"),0);
		QByteArray reqdata = "C";
		reqdata.append(static_cast<char>(0x0));
		reqdata.append(static_cast<char>(0x0));
		qDebug() << "Sending update block:" << data.toHex() << offset << size;
		m_currentRequest = req;
		//m_serialPort->requestPacket(pagename.toLocal8Bit(),0);
		//m_serialPort->requestPacket(QString("V").toLocal8Bit(),size);
		reqdata.append(static_cast<char>(offset & 0xFF));
		reqdata.append(static_cast<char>((offset >> 8) & 0xFF));
		reqdata.append(static_cast<char>(size & 0xFF));
		reqdata.append(static_cast<char>((size >> 8) & 0xFF));
		reqdata.append(data);
		uint32_t crc = Crc32_ComputeBuf(0,reqdata.data(),static_cast<size_t>(reqdata.size()));


		QByteArray packet;
		packet.append(static_cast<char>((reqdata.length() >> 8) & 0xFF));
		packet.append(static_cast<char>(reqdata.length() & 0xFF));
		packet.append(reqdata);
		packet.append(static_cast<char>((crc >> 24) & 0xFF));
		packet.append(static_cast<char>((crc >> 16) & 0xFF));
		packet.append(static_cast<char>((crc >> 8) & 0xFF));
		packet.append(static_cast<char>((crc >> 0) & 0xFF));
		m_serialPort->requestPacket(packet,0);


	}
	else if (req.type == GET_DATA)
	{
		//qDebug() << "Getting data frame";
		m_currentRequest = req;
		//m_serialPort->requestPacket(QByteArray("H"),0);
		QByteArray req = "O";
		req.append((char)0x0);
		req.append((char)0x0);
		req.append(240);
		req.append((char)0x0);
		uint32_t crc = Crc32_ComputeBuf(0,req.data(),req.size());
		QByteArray packet;
		packet.append((char)0x00);
		packet.append(0x05);
		packet.append(req);
		packet.append(crc >> 24);
		packet.append(crc >> 16);
		packet.append(crc >> 8);
		packet.append(crc >> 0);
		m_serialPort->requestPacket(packet,0);
	}
}



int TSBPComms::getInterfaceVersion()
{
	qDebug() << "getInterfaceVersion";
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = GET_INTERFACE_VERSION;
	req.sequencenumber = currentPacketNum++;
	m_reqList.append(req);
	return currentPacketNum-1;
}

int TSBPComms::getFirmwareVersion()
{
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = GET_FIRMWARE_VERSION;
	req.sequencenumber = currentPacketNum++;
	m_reqList.append(req);
	triggerNextSend();
	return currentPacketNum-1;
}


int TSBPComms::echoPacket(QByteArray packet)
{
	Q_UNUSED(packet)
	return -1;
}

int TSBPComms::getLocationIdList(unsigned char listtype, unsigned short listmask)
{
	Q_UNUSED(listtype)
	Q_UNUSED(listmask)
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = GET_LOCATION_ID_LIST;
	req.sequencenumber = currentPacketNum++;
	m_reqList.append(req);
	return currentPacketNum-1;
}

int TSBPComms::softReset()
{
	return -1;
}

int TSBPComms::hardReset()
{
	return -1;
}

bool TSBPComms::sendSimplePacket(unsigned short payload)
{
	Q_UNUSED(payload)
	return false;
}

void TSBPComms::connectSerial(QString port,int baud)
{
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = SERIAL_CONNECT;
	req.addArg(port);
	req.addArg(baud);
	//m_reqList.append(req);

	m_state = 1;


	m_serialPort = new SerialPort();

	//connect(serialPort,SIGNAL(packetReceived(QByteArray)),this,SLOT(parseEverything(QByteArray)));
	connect(m_serialPort,SIGNAL(bytesReady(QByteArray)),this,SLOT(parseBuffer(QByteArray)),Qt::QueuedConnection);
	//connect(serialPort,SIGNAL(bytesReady(QByteArray)),this,SLOT(dataLogRead(QByteArray)));
	connect(m_serialPort,SIGNAL(connected()),this,SLOT(serialPortConnected()),Qt::QueuedConnection);
	//connect(serialPort,SIGNAL(unableToConnect(QString)),this,SLOT(serialPortUnableToConnect(QString)));
	//connect(serialPort,SIGNAL(error(QString)),this,SLOT(serialPortError(QString)));
	//connect(serialPort,SIGNAL(disconnected()),this,SLOT(serialPortDisconnected()));
	m_serialPort->openPort(port,baud,false);

}
void TSBPComms::serialPortConnected()
{
	//m_serialPort->writePacket(QString("Q\r\n").toLatin1());
	getFirmwareVersion();
//	m_waitingForPacketResponse =  true;
//	m_serialPort->requestPacket(QByteArray("H"),20);
//	this->sendPacket()
	m_state = 2;
	emit connected();
}
int TSBPComms::requestPage(QByteArray pagereqstring,int length)
{
	QMutexLocker locker(&reqListMutex);
	for (int i=0;i<20000;i++)
	{
		RequestClass req;
		req.type = RETRIEVE_PAGE;
		req.sequencenumber = currentPacketNum++;
		emit interrogateTaskStart(QString("Location ID ") + QString(pagereqstring),currentPacketNum-1);
		req.addArg(pagereqstring);
		req.addArg(i);
		req.addArg(((20000 - i >= 256) ? 256 : 20000-i));
		i += 255;
		m_reqList.append(req);
	}
	triggerNextSend();
	return currentPacketNum-1;
}
void TSBPComms::triggerNextSend()
{
	if (!m_waitingForPacketResponse)
	{
		if (m_reqList.size() > 0)
		{
			QLOG_DEBUG() << "Sending next packet:" << "0x" + QString::number(m_reqList.at(0).type,16) << "Packets left:" << m_reqList.size();
			RequestClass req = m_reqList.at(0);
			m_reqList.removeAt(0);
			sendPacket(req);
		}
		else
		{
			//No requests, create a datalog request
			//RequestClass req;
			//req.type = GET_DATA;
			//req.sequencenumber = currentPacketNum++;
			//sendPacket(req);
		}
	}
}


void TSBPComms::parseBuffer(QByteArray data)
{
	//qDebug() << "Bytes available:" << data.size();
	//qDebug() << data.toHex();
	//qDebug() << QString(data);
	if (m_waitingForPacketResponse == true)
	{
		if (m_currentRequest.type == GET_FIRMWARE_VERSION)
		{
			QString versionstr(data.mid(1,data.length()-5));
			emit interrogateTaskSucceed(m_currentRequest.sequencenumber);
			qDebug() << "Got firmware version:" << versionstr;
		}
		else if (m_currentRequest.type == GET_DATA)
		{
			QByteArray realtimedata = data.mid(1,data.size()-5);
			//qDebug() << "Data:" << realtimedata;
			emit dataLogPayloadReceived(realtimedata,QByteArray());
			currentPacketCount++;
		}
		else if (m_currentRequest.type == UPDATE_BLOCK_IN_FLASH)
		{
			qDebug() << "Update block response:" << data.toHex();

		}
		else if (m_currentRequest.type == RETRIEVE_PAGE)
		{
			QByteArray pagereqstr = m_currentRequest.args.at(0).toByteArray(); //Page Name
			int offset = m_currentRequest.args.at(1).toInt(); //Size
			int size = m_currentRequest.args.at(2).toInt(); //Size
			qDebug() << "Got page:" << pagereqstr << "size" << size << "offset" << offset << "actualsize" << data.size();
			//Now we have selected the page, submit a request for its data.
			m_pageBufferMap[0].replace(offset,size,data.mid(1,data.size()-5));
			m_pageBufferFilledMap[0] += size;
			qDebug() << "Size of page 0 is now:" << m_pageBufferFilledMap[0] << ":" << m_pageBufferMap[0].size();
			//m_pageBuffer.append(data);
			emit interrogateTaskSucceed(m_currentRequest.sequencenumber);
			if (m_reqList.size() == 0)
			{
				for (QMap<QString,TSBPTable3DData*>::const_iterator i=m_3dTableData.constBegin();i!=m_3dTableData.constEnd();i++)
				{
					i.value()->setData(0,false,m_pageBufferMap[0]);
				}
				for (QMap<QString,TSBPTable2DData*>::const_iterator i=m_2dTableData.constBegin();i!=m_2dTableData.constEnd();i++)
				{
					i.value()->setData(0,false,m_pageBufferMap[0]);
				}

				emit interrogationComplete();
			}
			QString pagenum = m_currentRequest.args.at(0).toString();
			pagenum = pagenum.mid(1);
			int i = 0;
			while(i < pagenum.length() && pagenum[i]=='0'){
			 i++;
			}

			pagenum.remove(0,i);
			int locid = pagenum[0].toLatin1();

//	QMap<QString,TSBPTable3DData*> m_3dTableData;
			/*
			for (QMap<QString,TSBPTable3DData*>::const_iterator i=m_3dTableData.constBegin();i!=m_3dTableData.constEnd();i++)
			{
				i.value()->setData(0,false,data);
			}


			//qDebug() << "Packet:" << locid << "retrieved. Size:" << finalbuf.size() << "expected:" << maxoffset;;
			//emit flashBlockRetrieved(locid,QByteArray(),finalbuf);
//			m_pageArrayMap

			if (m_pageTo2DTableList.contains(locid))
			{
				for (int i=0;i<m_pageTo2DTableList.value(locid).size();i++)
				{
					if (m_pageTo2DTableList.value(locid)[i]->axisPage() == locid)
					{
						m_pageTo2DTableList.value(locid)[i]->setAxis(data);
					}
					else
					{
						qDebug() << "Bad";
					}
					if (m_pageTo2DTableList.value(locid)[i]->valuePage() == locid)
					{
						m_pageTo2DTableList.value(locid)[i]->setValues(data);
					}
					else
					{
						qDebug() << "Bad";
					}
				}
			}
			if (m_pageTo3DTableList.contains(locid))
			{
				for (int i=0;i<m_pageTo3DTableList.value(locid).size();i++)
				{
					if (m_pageTo3DTableList.value(locid)[i]->xAxisPage() == locid)
					{
						m_pageTo3DTableList.value(locid)[i]->setXAxis(data);
					}
					if (m_pageTo3DTableList.value(locid)[i]->yAxisPage() == locid)
					{
						m_pageTo3DTableList.value(locid)[i]->setYAxis(data);
					}
					if (m_pageTo3DTableList.value(locid)[i]->valuePage() == locid)
					{
						m_pageTo3DTableList.value(locid)[i]->setValues(data);
					}
					else
					{
						qDebug() << "Bad";
					}
				}
			}*/
			/*if (m_interrogateList.contains(m_privReqList[i].sequencenumber))
			{
				emit interrogateTaskSucceed(m_privReqList[i].sequencenumber);
				m_interrogateList.removeOne(m_privReqList[i].sequencenumber);
				if (m_interrogateList.size() == 0)
				{
					emit interrogationComplete();
				}
			}*/
			/*for (int j=0;j<m_configNameList.size();j++)
			{
				if (m_configDataMap[m_configNameList[j]]->locationId() == locid)
				{
					//We're on the current page
					m_configDataMap[m_configNameList[j]]->setData(data);
				}
			}*/
		}
		m_waitingForPacketResponse = false;
	}
	triggerNextSend();
	return;
	//Parse incoming data here.
	qDebug() << data;
	if (m_state == 2)
	{
		//Done waiting
		m_state = 3;
		for (QMap<QString,QList<ConfigBlock> >::const_iterator j=m_memoryMetaData->configMetaData().constBegin(); j!=m_memoryMetaData->configMetaData().constEnd();j++)
		{
			qDebug() << "Getting memory for:" << j.key();
			//m_serialPort->writePacket(QString("P") + j.key() + "\r\n");
			unsigned short maxoffset = 0;
			for (int k=0;k<j.value().size();k++)
			{
				ConfigBlock b = j.value()[k];
				b.locationId();
				b.offset();
				b.size();
				if (b.offset() + b.size() > maxoffset)
				{
					maxoffset = b.offset() + b.size();
				}
			}

			j++;
		}
	}
}

void TSBPComms::disconnectSerial()
{

}

void TSBPComms::loadLog(QString filename)
{
	Q_UNUSED(filename)
}

void TSBPComms::playLog()
{

}

//virtual void populateDataFields()=0;
QByteArray TSBPComms::generatePacket(QByteArray header,QByteArray payload)
{
	Q_UNUSED(header)
	Q_UNUSED(payload);
	return QByteArray();
}

int TSBPComms::updateBlockInRam(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)
{
	Q_UNUSED(location)
	Q_UNUSED(offset)
	Q_UNUSED(size)
	Q_UNUSED(data)
	return -1;
}

int TSBPComms::updateBlockInFlash(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)
{
	QMutexLocker locker(&reqListMutex);
	if (data.size() > 255)
	{
		for (int i=0;i<data.size();i+=255)
		{
			int cursize = (i+255 < data.size()) ? 255 :  data.size() - i;
			RequestClass req;
			req.type = UPDATE_BLOCK_IN_FLASH;
			req.sequencenumber = currentPacketNum++;
			req.addArg(location,2);
			req.addArg(offset + i,2);
			req.addArg(cursize,2);
			req.addArg(data.mid(i,cursize),cursize);
			m_reqList.append(req);
		}
	}
	else
	{
		RequestClass req;
		req.type = UPDATE_BLOCK_IN_FLASH;
		req.sequencenumber = currentPacketNum++;
		req.addArg(location,2);
		req.addArg(offset,2);
		req.addArg(size,2);
		req.addArg(data,data.size());
		m_reqList.append(req);
	}
	triggerNextSend();
	return currentPacketNum-1;
}

int TSBPComms::retrieveBlockFromRam(unsigned short location, unsigned short offset, unsigned short size,bool mark)
{
	Q_UNUSED(offset)
	Q_UNUSED(size)
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = RETRIEVE_BLOCK_IN_RAM;
	req.sequencenumber = currentPacketNum++;
	req.addArg(location,2);
	m_reqList.append(req);
	return currentPacketNum-1;

}

int TSBPComms::retrieveBlockFromFlash(unsigned short location, unsigned short offset, unsigned short size,bool mark)
{
	Q_UNUSED(offset)
	Q_UNUSED(size)
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = RETRIEVE_BLOCK_IN_FLASH;
	req.sequencenumber = currentPacketNum++;
	req.addArg(location,2);
	m_reqList.append(req);
	return currentPacketNum-1;
}

int TSBPComms::burnBlockFromRamToFlash(unsigned short location,unsigned short offset, unsigned short size)
{
	Q_UNUSED(offset)
	Q_UNUSED(size)
	QMutexLocker locker(&reqListMutex);
	RequestClass req;
	req.type = BURN_BLOCK_FROM_RAM_TO_FLASH;
	req.sequencenumber = currentPacketNum++;
	req.addArg(location,2);
	m_reqList.append(req);
	return currentPacketNum-1;
}

void TSBPComms::setInterByteSendDelay(int milliseconds)
{
	Q_UNUSED(milliseconds)
}

void TSBPComms::setlogsDebugEnabled(bool enabled)
{
	Q_UNUSED(enabled)
}
void TSBPComms::packetCounter()
{
	qDebug() << "PPS:" << currentPacketCount;
	currentPacketCount = 0;
}
QByteArray TSBPComms::readPacket(SerialPort *port)
{
//	bool makenextwrite = true;
	//unsigned int packetnum = 0;
	int lenbytesread = 0;
	int lenbyteslefttoread=2;
	QByteArray lenbytes;
	QByteArray newbuf;
	//lenbytesread = port->readBytes(&newbuf,lenbyteslefttoread,1000);
	if (lenbytesread != 2)
	{
		qDebug() << "Error reading initial bytes";
		return QByteArray();
	}
	lenbytes.append(newbuf);
	qDebug() << "Test read. Bytes left:" << port->serialBufferSize();
	unsigned int newlen = (((unsigned char)lenbytes[0]) << 8) + (unsigned char)lenbytes[1];
	qDebug() << "Len:" << newlen;
	unsigned int datalenread =0;
	QByteArray packet;
//	int timeoutcount = 0;
	unsigned int datatoberead = newlen;
	int currentdataread = 0;

	QByteArray newbuf3;
	//currentdataread = port->readBytes(&newbuf3,newlen,1000);
	if (currentdataread <= 0)
	{
		qDebug() << "Error in read" << currentdataread << port->serialBufferSize();
		if (port->serialBufferSize() > 0)
		{
//			port->readBytes(&newbuf3,port->serialBufferSize());
			qDebug() << QString(newbuf3);
		}
		return QByteArray();
	}
	packet.append(newbuf3);

	//if (datalenread == 0)
	//{
		QByteArray newbuf6;
		//int read = port->readBytes(&newbuf6,1); //Read the single code
		//if (read)
		//	qDebug() << "SCode:" << QString::number(newbuf6[0]);
		//else
		//	qDebug() << "Timeout on SCode";
	//}
	qDebug() << "Code:" << QString::number((unsigned char)packet[0],16) << datalenread << newlen;
	QByteArray crcbytes;
	datalenread = 0;
	newlen = 4;
	currentdataread = 0;
	datatoberead=newlen;
	QByteArray newbuf4;
//	currentdataread = port->readBytes(&newbuf4,datatoberead);
	crcbytes.append(newbuf4);
	qDebug() << "CRC Timeout" << currentdataread;

	qDebug() << "Code:" << QString::number((unsigned char)packet[0],16);
	unsigned int newcrc = ((unsigned char)crcbytes[0]) << 24;
	newcrc += ((unsigned char)crcbytes[1]) << 16;
	newcrc += ((unsigned char)crcbytes[2]) << 8;
	newcrc += ((unsigned char)crcbytes[3]);
	if (newcrc != Crc32_ComputeBuf(0,packet.data(),packet.size()))
	{
		qDebug() << "Error, invalid CRC";
		qDebug() << "Packet size:" << packet.size();
		//makenextwrite = true;
		port->flush();
		//continue;
		return QByteArray();
	}
	if ((unsigned char)packet[0] == 0x00)
	{
		 qDebug() << "Packet" << packet.size();
		currentPacketCount++;
		//emit dataLogPayloadReceived(QByteArray(),packet);
		QByteArray newbuf;
	//	port->readBytes(&newbuf,1024,10); //CRC
		return packet.mid(1);
	}
	if (datalenread == 4)
	{
		//makenextwrite = true;
	}
	QByteArray newbuf2;
//	port->readBytes(&newbuf2,1024,10); //CRC
	//qDebug() << "Clearlen:" << clearlen;
	//usleep(10000);
	return QByteArray();
}
void TSBPComms::run()
{
	//unsigned char reqBuf[7];
	//reqBuf[0] = 0;
	//reqBuf[1] = 1;
	//dsreqBuf[0] = 'R';
	//reqBuf[2] = 'A';
	//quint32 crc = Crc32_ComputeBuf(0,reqBuf + 2,1);
	//reqBuf[3] = crc >> 24;
	//reqBuf[4] = crc >> 16;
	//reqBuf[5] = crc >> 8;
	//reqBuf[6] = crc >> 0;

	unsigned int timercounter = 0;
	while (true)
	{
		timercounter++;
		/*if (timercounter > 100)
		{
			timercounter = 0;
			//Once per second, send out a random payload.
			QByteArray payload;
			for (int i=0;i<250;i++)
			{
				payload.append(rand() % 255);
			}
			//emit dataLogPayloadReceived(QByteArray(),payload);
		}*/
		/*reqListMutex.lock();
		for (int i=0;i<m_reqList.size();i++)
		{
			m_privReqList.append(m_reqList[i]);
		}
		m_reqList.clear();
		reqListMutex.unlock();
		for (int i=0;i<m_privReqList.size();i++)
		{
			if (m_privReqList[i].type == GET_INTERFACE_VERSION)
			{
//				msleep(10);
				emit interfaceVersion("TSBP Simulator");
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == SERIAL_CONNECT)
			{

				QString port = m_privReqList[i].args[0].toString();
				//int baud = m_privReqList[i].args[1].toInt();
				int openerror = m_serialPort.openPort(port,115200,false);
				qDebug() << "Opened:" << openerror;
				if (openerror < 0)
				{
					emit disconnected();
					return;
				}
				m_connected = true;
				unsigned char buf[200];
				buf[0] = 'Q';
			        m_serialPort.flush();
			        m_serialPort.writeBytes(buf,1);
//				msleep(1000);
				QByteArray newbuf;
				int count = m_serialPort.readBytes(&newbuf,20,500);
			        qDebug() << "Version size:" << count;
			        QString verstr = "";
			        for (int i=0;i<count;i++)
			        {
					verstr += QString::number(newbuf[i],16) + ",";
			        }
			        qDebug() << verstr;
				qDebug() << "Version:" << newbuf;
				QMap<QString,QString> m_interrogationData;
				m_interrogationData["version"] = newbuf;
				emit interrogationData(m_interrogationData);

				emit connected();
				for (QMap<QString,QList<ConfigBlock> >::const_iterator j=m_memoryMetaData->configMetaData().constBegin(); j!=m_memoryMetaData->configMetaData().constEnd();j++)
				{
					int task = retrieveBlockFromFlash(j.key().toInt(),0,0);
					qDebug() << "Requesting block:" << j.key();
					emit interrogateTaskStart(QString("Flash Location ") + j.key(),task);
					m_interrogateList.append(task);
				}

				//emit interrogationComplete();

				//m_memoryMetaData->configMetaData()
				*/
				/*for (QMap<QString,QList<ConfigBlock> >::const_iterator j=m_memoryMetaData->configMetaData().constBegin(); j!=m_memoryMetaData->configMetaData().constEnd();j++)
				{
					qDebug() << "Getting memory for:" << j.key();
					unsigned short maxoffset = 0;
					for (int k=0;k<j.value().size();k++)
					{
						ConfigBlock b = j.value()[k];
						b.locationId();
						b.offset();
						b.size();
						if (b.offset() + b.size() > maxoffset)
						{
							maxoffset = b.offset() + b.size();
						}
					}
					QByteArray finalbuf;
					if (maxoffset > 255)
					{
						for (int k=0;k<maxoffset/255;k++)
						{
							unsigned char readbuf[13];
							readbuf[0] = 0;
							readbuf[1] = 7;
							readbuf[2]  = 'r';
							readbuf[3] = j.key().toInt() >> 8; //Page
							readbuf[4] = j.key().toInt(); //Page
							readbuf[5] = k; //Offset
							readbuf[6] = 0; //Offset
							readbuf[7] = 0; //Length
							readbuf[8] = 255; //Length
							quint32 crc = Crc32_ComputeBuf(0,readbuf + 2,7);
							readbuf[9] = crc >> 24;
							readbuf[10] = crc >> 16;
							readbuf[11] = crc >> 8;
							readbuf[12] = crc >> 0;
							m_serialPort.writeBytes(readbuf,13);
							QByteArray packet = readPacket(&m_serialPort);
							qDebug() << "Reading:" << "Size:" << 255 << "Offset:" << (k << 8) << "Packet:" << packet.size();
							finalbuf.append(packet);
						}
					}

					unsigned char readbuf[13];
					readbuf[0] = 0;
					readbuf[1] = 7;
					readbuf[2]  = 'r';
					readbuf[3] = j.key().toInt() >> 8; //Page
					readbuf[4] = j.key().toInt(); //Page
					readbuf[5] = maxoffset >> 8; //Offset
					readbuf[6] = 0; //Offset
					readbuf[7] = 0 >> 8; //Length
					readbuf[8] = maxoffset % 255; //Length
					quint32 crc = Crc32_ComputeBuf(0,readbuf + 2,7);
					readbuf[9] = crc >> 24;
					readbuf[10] = crc >> 16;
					readbuf[11] = crc >> 8;
					readbuf[12] = crc >> 0;
					m_serialPort.writeBytes(readbuf,13);

					QByteArray packet = readPacket(&m_serialPort);
					finalbuf.append(packet);
					qDebug() << "Reading:" << "Size:" << maxoffset % 255 << "Offset:" << ((maxoffset >> 8) << 8) << "Packet:" << packet.size();


					//finalbuf Is your final packet for that page.



					j++;
				}*/
/*
			}
			else if (m_privReqList[i].type == GET_FIRMWARE_VERSION)
			{
//				usleep(10000);
				emit firmwareVersion("0.0.1");
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_MAX_PACKET_SIZE)
			{
				//usleep(10000);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_LOCATION_ID_LIST)
			{
				//usleep(10000);
				QList<unsigned short> idlists;*/
				/*for (QMap<QString,QMap<QString,scalarclass> >::const_iterator j = pageMap.constBegin();j!=pageMap.constEnd();j++)
				{
					idlists.append(j.key().toInt());
					for (QMap<QString,scalarclass>::const_iterator j = i.value().constBegin();j!=i.value().constEnd();j++)
					{
						qDebug() << "Page:" << i.key() << "Scalar:" << j.key() << j.value().offset << j.value().scale << j.value().translate;
					}
				}*//*
				if (m_interrogateInProgress)
				{

				}
				emit locationIdList(idlists);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_DECODER_NAME)
			{
				//usleep(10000);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_FIRMWARE_BUILD_DATE)
			{
				//usleep(10000);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_COMPILER_VERSION)
			{
				//usleep(10000);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_OPERATING_SYSTEM)
			{
				//usleep(10000);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == GET_LOCATION_ID_INFO)
			{
				//usleep(10000);
				unsigned short locid = m_privReqList[i].args[0].toInt();
				//void locationIdInfo(unsigned short locationid,MemoryLocationInfo info);
				MemoryLocationInfo info;
				info.isRam = false;
				info.isFlash = true;
				info.locationid = locid;
				info.size = 1024;
				info.type = DATA_CONFIG;
				emit locationIdInfo(locid,info);
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == UPDATE_BLOCK_IN_FLASH)
			{

				unsigned short locid = m_privReqList[i].args[0].toInt();
				unsigned short offset = m_privReqList[i].args[1].toInt();
				unsigned short size = m_privReqList[i].args[2].toInt();
				qDebug() << "Writing to page" << locid << "Of offset" << offset << "Size" << size;
				QByteArray bytes = m_privReqList[i].args[3].toByteArray();
				qDebug() << "True Size:" << bytes.size();
				QByteArray req;
				req.append('w');
				req.append((char)0);
				req.append(locid);
				req.append((char)((offset >> 8) & 0xFF));
				req.append(offset & 0xFF);
				req.append((char)((size >> 8) & 0xFF));
				req.append(size & 0xFF);
				req.append(bytes);
				for (int j=0;j<req.size();j++)
				{
					m_serialPort.writeBytes(req.mid(j,1));
					//msleep(1);
				}
				req.clear();
				//msleep(100);
			}
			else if (m_privReqList[i].type == BURN_BLOCK_FROM_RAM_TO_FLASH)
			{
				unsigned short page = m_privReqList[i].args[0].toInt();
				QByteArray req;
				req.append('b');
				req.append((char)0);
				req.append(page);
				for (int j=0;j<req.size();j++)
				{
					m_serialPort.writeBytes(req.mid(j,1));
					//msleep(1);
				}
				//msleep(500);
			}
			else if (m_privReqList[i].type == RETRIEVE_BLOCK_IN_RAM)
			{
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}
			else if (m_privReqList[i].type == RETRIEVE_BLOCK_IN_FLASH)
			{
				int current = -1;
				//void flashBlockRetrieved(unsigned short locationid,QByteArray header,QByteArray payload);
				//Header can be mepty, payload should be the entire block of memory.
				unsigned short locid = m_privReqList[i].args[0].toInt();
				//if (//m_memoryMetaData->configMetaData())
				if (m_memoryMetaData->hasConfigMetaData(QString::number(locid)))
				{
					QList<ConfigBlock> blocklist = m_memoryMetaData->getConfigMetaData(QString::number(locid));
					qDebug() << "Getting memory for:" << QString::number(locid);
					unsigned short maxoffset = 0;
					for (int k=0;k<blocklist.size();k++)
					{
						ConfigBlock b = blocklist[k];
						b.locationId();
						b.offset();
						b.size();
						if (b.offset() + b.elementSize() > maxoffset)
						{
							maxoffset = b.offset() + b.elementSize();
						}
					}
					qDebug() << "Expected max:" << maxoffset;
					qDebug() << "Actual Max:" << m_pageSizeMap[locid];
					maxoffset = m_pageSizeMap[locid];
					QByteArray finalbuf;
					unsigned short curroffset = 0;
					int tries = 0;
					bool failure = false;
					if (maxoffset > 255)
					{
						while (maxoffset - curroffset > 255 && tries < 5)
						{*/
							/*unsigned char readbuf[13];
							readbuf[0] = 0;
							readbuf[1] = 7;
							readbuf[2]  = 'r';
							readbuf[3] = locid >> 8; //Page
							readbuf[4] = locid; //Page
							readbuf[5] = curroffset >> 8; //Offset
							readbuf[6] = curroffset; //Offset
							readbuf[7] = 0; //Length
							readbuf[8] = 255; //Length
							quint32 crc = Crc32_ComputeBuf(0,readbuf + 2,7);
							readbuf[9] = crc >> 24;
							readbuf[10] = crc >> 16;
							readbuf[11] = crc >> 8;
							readbuf[12] = crc >> 0;*//*
							//m_serialPort.writeBytes(readbuf,13);
							unsigned char readbuf[13];
							readbuf[0] = 'r';
							readbuf[1] = 0;
							readbuf[2] = locid;
							//msleep(1000);
							m_serialPort.writeBytes(readbuf,3);
							if (current != locid)
							{
//								msleep(400);
								current = locid;
							}
							readbuf[0] = curroffset >> 8;
							readbuf[1] = curroffset;
							readbuf[2] = 0;
							readbuf[3] = 255;
							m_serialPort.writeBytes(readbuf,4);
							//msleep(2000);
							//QByteArray packet = readPacket(&m_serialPort);
							QByteArray packet;
							int rcvd = m_serialPort.readBytes(&packet,255,500);
							qDebug() << "Reading:" << "Size:" << 255 << "Offset:" <<  curroffset << "Packet:" << packet.size() << "Got" << rcvd;
							if (rcvd == 255)
							{
								tries = 0;
							}
							else
							{
								tries++;
							}
							curroffset += packet.size();
							finalbuf.append(packet);
						}
						if (maxoffset - curroffset > 255)
						{
							failure = true;
						}
						qDebug() << "Done with major offset";
					}
					//emit commandSuccessful(m_privReqList[i].sequencenumber);

					/*unsigned char readbuf[13];
					readbuf[0] = 0;
					readbuf[1] = 7;
					readbuf[2]  = 'r';
					readbuf[3] = locid >> 8; //Page
					readbuf[4] = locid; //Page
					readbuf[5] = curroffset >> 8; //Offset
					readbuf[6] = curroffset; //Offset
					readbuf[7] = 0; //Length
					readbuf[8] = maxoffset - curroffset; //Length
					quint32 crc = Crc32_ComputeBuf(0,readbuf + 2,7);
					readbuf[9] = crc >> 24;
					readbuf[10] = crc >> 16;
					readbuf[11] = crc >> 8;
					readbuf[12] = crc >> 0;
					m_serialPort.writeBytes(readbuf,13);*//*
					if (!failure)
					{
						unsigned char readbuf[13];
						readbuf[0] = 'r';
						readbuf[1] = 0;
						readbuf[2] = locid;
						m_serialPort.writeBytes(readbuf,3);
						if (current != locid)
						{
//							msleep(400);
							current = locid;
						}
						readbuf[0] = curroffset >> 8;
						readbuf[1] = curroffset;
						readbuf[2] = 0;
						readbuf[3] = maxoffset - curroffset;
						m_serialPort.writeBytes(readbuf,4);

						//QByteArray packet = readPacket(&m_serialPort);
						QByteArray packet;
						int rcvd = m_serialPort.readBytes(&packet,maxoffset - curroffset,500);
						finalbuf.append(packet);
						qDebug() << "Reading:" << "Size:" << maxoffset - curroffset << "Offset:" << curroffset << "Packet:" << packet.size() << "Got" << rcvd;


						//finalbuf Is your final packet for that page.
						qDebug() << "Packet:" << locid << "retrieved. Size:" << finalbuf.size() << "expected:" << maxoffset;;
						emit flashBlockRetrieved(locid,QByteArray(),finalbuf);

						if (m_pageTo2DTableList.contains(locid))
						{
							for (int i=0;i<m_pageTo2DTableList.value(locid).size();i++)
							{
								if (m_pageTo2DTableList.value(locid)[i]->axisPage() == locid)
								{
									m_pageTo2DTableList.value(locid)[i]->setAxis(finalbuf);
								}
								else
								{
									qDebug() << "Bad";
								}
								if (m_pageTo2DTableList.value(locid)[i]->valuePage() == locid)
								{
									m_pageTo2DTableList.value(locid)[i]->setValues(finalbuf);
								}
								else
								{
									qDebug() << "Bad";
								}
							}
						}
						if (m_pageTo3DTableList.contains(locid))
						{
							for (int i=0;i<m_pageTo3DTableList.value(locid).size();i++)
							{
								if (m_pageTo3DTableList.value(locid)[i]->xAxisPage() == locid)
								{
									m_pageTo3DTableList.value(locid)[i]->setXAxis(finalbuf);
								}
								if (m_pageTo3DTableList.value(locid)[i]->yAxisPage() == locid)
								{
									m_pageTo3DTableList.value(locid)[i]->setYAxis(finalbuf);
								}
								if (m_pageTo3DTableList.value(locid)[i]->valuePage() == locid)
								{
									m_pageTo3DTableList.value(locid)[i]->setValues(finalbuf);
								}
								else
								{
									qDebug() << "Bad";
								}
							}
						}
						if (m_interrogateList.contains(m_privReqList[i].sequencenumber))
						{
							emit interrogateTaskSucceed(m_privReqList[i].sequencenumber);
							m_interrogateList.removeOne(m_privReqList[i].sequencenumber);
							if (m_interrogateList.size() == 0)
							{
								emit interrogationComplete();
							}
						}
						for (int j=0;j<m_configNameList.size();j++)
						{
							if (m_configDataMap[m_configNameList[j]]->locationId() == locid)
							{
								//We're on the current page
								m_configDataMap[m_configNameList[j]]->setData(finalbuf);
							}
						}
					}
					else
					{
						emit commandFailed(m_privReqList[i].sequencenumber,0xFF00FF);
					}
				}
				emit commandSuccessful(m_privReqList[i].sequencenumber);
			}

		}
		m_privReqList.clear();
		//Before we sleep, lets send and recieve.

		if (m_connected)
		{
			//qDebug() << "Writing:";// << port.writeBytes(buf,7);
			m_serialPort.writeBytes(QByteArray().append('A'));
		//usleep(13000);
		//QByteArray reply = readPacket(&m_serialPort);
		QByteArray reply;
		int replybytes = m_serialPort.readBytes(&reply,169,5000); //MS2 returns 169
		//m_serialPort.readBytes(&reply,19,5000);
		//m_serialPort.readUntilEmpty();
		if (reply.size() > 0)
		{
			emit dataLogPayloadReceived(reply);
			//qDebug() << "Reply:" << reply.size();
		}
		}
		if (!m_threadRun)
		{
			m_serialPort.closePort();
			return;
		}

//		usleep(5000); //Sleep to avoid CPU spinning
*/
	}
	/*
	emit connected();
	SerialPort port;
	int openerror = port.openPort("/dev/ttyACM0",115200,false);
	qDebug() << "Opened:" << openerror;
	if (openerror < 0)
	{
		return;
	}

	unsigned char buf[1024];

	bool makenextwrite = true;
	int looptime = 0;
	unsigned int packetnum = 0;

	buf[0] = 'Q';
	port.flush();
	port.writeBytes(buf,1);
	usleep(1000000);
	QByteArray newbuf;
	int count = port.readBytes(&newbuf,200);
	qDebug() << "Version size:" << count;
	QString verstr = "";
	for (int i=0;i<count;i++)
	{
		verstr += QString::number(buf[i],16) + ",";
	}
	qDebug() << verstr;
	qDebug() << "Version:" << QString::fromAscii((const char*)buf+count-20,19);

	buf[0] = 0;
	buf[1] = 7;
	buf[2]  = 'r';
	buf[3] = 0; //Page
	buf[4] = 1; //Page
	buf[5] = 0; //Offset
	buf[6] = 0; //Offset
	buf[7] = 0; //Length
	buf[8] = 0xFF; //Length

	while (true)
	{
		if (makenextwrite)
		{
			buf[0] = 0;
			buf[1] = 1;
			buf[2] = 'A';
			quint32 crc = Crc32_ComputeBuf(0,buf + 2,1);
			buf[3] = crc >> 24;
			buf[4] = crc >> 16;
			buf[5] = crc >> 8;
			buf[6] = crc >> 0;
			//qDebug() << "Writing:" << port.writeBytes(buf,7);
			port.writeBytes(buf,7);
			//usleep(13000);
			makenextwrite = true;
		}
		QByteArray reply = readPacket(&port);
		if (reply.size() > 0)
		{
			emit dataLogPayloadReceived(QByteArray(),reply);
		}
		int stopper = 1;
	}*/
}
