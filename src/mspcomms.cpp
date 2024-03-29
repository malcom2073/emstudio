/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "mspcomms.h"
#include "tsbpdatapacketdecoder.h"
#include <QSerialPort>
#include <QDebug>
#include <QMap>
#include <QStringList>
#include "tsbptable2ddata.h"
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QRegularExpression>
#include <QThread>
#include <QFileDialog>
QString convertString(QString input)
{
    QRegularExpression myregex("\\\\[xX][0-9a-fA-F]+");
    QRegularExpressionMatch match = myregex.match(input);
    if (match.hasMatch())
    {
        QString matched = match.captured(0);
        QString orig = matched;
        orig.detach(); // Because QString::replace modifies the origonal string.
        return input.replace(input.indexOf(orig),orig.length(),QByteArray::fromHex(matched.replace("\\x","").toLatin1()));
    }
    return input;
}
QStringList splitValueLine(QString line)
{
    QStringList retval;
    if (line.contains("{"))
    {
        //Splits a line with commas, respecting "{}" characters.
        foreach (QString str,line.split(QRegularExpression(",(?=(?:[^\\{]*\\{[^\\}]*\\})*[^\\}]*$)")))
        {
            retval.append(str.trimmed());
        }
        qDebug() <<  retval;
    }
    else // No {}, so split like normal.
    {
        foreach (QString str,line.split(","))
        {
            retval.append(str.trimmed());
        }
    }
    return retval;
}

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
TableConfigData *MSPComms::tableConfigFromIniLine(QString name,QStringList linevalsplit)
{
    TableConfigData *data = new TableConfigData();
    connect(data,&TableConfigData::saveSignal,this,&MSPComms::memorySaveSlot);

    data->setBigEndian(m_bigEndian);
    data->setType(ConfigData::ARRAY);
    data->setName(name);

    QString format = linevalsplit[3].trimmed();
    data->setXSize(format.mid(1,format.size()-2).split("x")[0].toInt());
    data->setYSize(format.mid(1,format.size()-2).split("x")[1].toInt());
    data->setOffset(linevalsplit[2].trimmed().toInt());
    data->setDisplayDecimals(linevalsplit[9].toInt());

    QList<QPair<QString,double> > calclist;
    if (linevalsplit[5].trimmed().startsWith("."))
    {
        calclist.append(QPair<QString,double>("mult",QString("0").append(linevalsplit[5].trimmed()).toFloat()));
    }
    else
    {
        calclist.append(QPair<QString,double>("mult",linevalsplit[5].trimmed().toFloat()));
    }
    if (linevalsplit[6].trimmed().startsWith("."))
    {
        calclist.append(QPair<QString,double>("add",QString("0").append(linevalsplit[6].trimmed()).toFloat()));
    }
    else
    {
        calclist.append(QPair<QString,double>("add",linevalsplit[6].trimmed().toFloat()));
    }
    data->setCalc(calclist);


    if (linevalsplit[1].trimmed() == "S16")
    {
        data->setElementSize(2);
        data->setElementType(ConfigData::SIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U08")
    {
        data->setElementSize(1);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U16")
    {
        data->setElementSize(2);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "S32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::SIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "F32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::FLOAT_ELEMENT);
    }
    return data;



}
ArrayConfigData *MSPComms::arrayConfigFromIniLine(QString name,QStringList linevalsplit)
{
    ArrayConfigData *data = new ArrayConfigData();
    data->setBigEndian(m_bigEndian);
    connect(data,&ArrayConfigData::saveSignal,this,&MSPComms::memorySaveSlot);
    data->setName(name);
    data->setType(ConfigData::ARRAY);
    data->setDisplayDecimals(linevalsplit[9].toInt());

    data->setOffset(linevalsplit[2].trimmed().toInt());
    QString format = linevalsplit[3].trimmed();
    data->setSize(format.mid(1,format.size()-2).trimmed().toInt());

    QList<QPair<QString,double> > calclist;
    if (linevalsplit[5].trimmed().startsWith("."))
    {
        calclist.append(QPair<QString,double>("mult",QString("0").append(linevalsplit[5].trimmed()).toFloat()));
    }
    else
    {
        calclist.append(QPair<QString,double>("mult",linevalsplit[5].trimmed().toFloat()));
    }
    if (linevalsplit[6].trimmed().startsWith("."))
    {
        calclist.append(QPair<QString,double>("add",QString("0").append(linevalsplit[6].trimmed()).toFloat()));
    }
    else
    {
        calclist.append(QPair<QString,double>("add",linevalsplit[6].trimmed().toFloat()));
    }
    data->setCalc(calclist);

    if (linevalsplit[1].trimmed() == "S16")
    {
        data->setElementSize(2);
        data->setElementType(ConfigData::SIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U08")
    {
        data->setElementSize(1);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U16")
    {
        data->setElementSize(2);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "U32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::UNSIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "S32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::SIGNED_ELEMENT);
    }
    else if (linevalsplit[1].trimmed() == "F32")
    {
        data->setElementSize(4);
        data->setElementType(ConfigData::FLOAT_ELEMENT);
    }

    return data;
}
void MSPComms::loadIniFile(QFile *inifile)
{
    inifile->open(QIODevice::ReadOnly);
    QString inistring = inifile->readAll();
    inifile->close();
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
    int nPages = 0;


    foreach (QString line,inilist)
    {
        line = line.trimmed();
        if (line.startsWith(";"))
        {
            continue;
        }
        else if (line == "")
        {
            continue;
        }
        if (line.contains(";"))
        {
            line = line.mid(0,line.indexOf(";")-1);
        }
        if (line.startsWith("[MegaTune]"))
        {
            section = 8;
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
        if (section == 8)
        {
            if (line.contains("="))
            {
            QStringList linesplit = line.split("=");

            QString cmd = linesplit[0].trimmed();
            QString args = linesplit[1].trimmed();
            if (cmd == "queryCommand")
            {

                m_commandStructure.queryCommand = args.replace("\"","");
            }
            else if (cmd == "versionInfo")
            {
                m_commandStructure.versionInfo = args.replace("\"","");;
            }
            }
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
                        else
                        {
                            //No title, just a variable
                            currentDialogItem.variable = currdialogitemsplit[0].replace("\"","").trimmed();
                            currentDialogItem.title = currdialogitemsplit[0].replace("\"","").trimmed();
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
                        DialogField field;
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
                    }
                    else if (linesplit[0].trimmed() == "panel")
                    {
                        //Sub dialog window
                        QStringList panelsplit = splitline(linesplit[1]);
                        if (panelsplit.size() >= 2)
                        {
                        QString panelname = panelsplit[0].trimmed();
                        QString panelloc = panelsplit[1].trimmed();
                        if (panelloc == "West" || panelloc == "Center" || panelloc == "East")
                        {
                        currentDialogItem.panelList.append(QPair<QString,QString>(panelloc,panelname));
                        }
                        else
                        {
                            //Conditional panel, not a location
                            currentDialogItem.panelList.append(QPair<QString,QString>(panelloc,panelname));
                        }
                        }
                        else
                        {
                        QString panelname = panelsplit[0].trimmed();
                        currentDialogItem.panelList.append(QPair<QString,QString>("noloc",panelname));
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
                if (linesplit[0].trimmed() == "ochGetCommand")
                {
                    m_commandStructure.ochGetCommand = linesplit[1].trimmed().replace("\"","");
                }
                if (linesplit[0].trimmed() == "ochBlockSize")
                {
                    m_commandStructure.ochBlockSize = linesplit[1].trimmed().toInt();
                }
                QStringList linevalsplit = linesplit[1].split(",");
                if (linevalsplit.size() > 5)
                {
                    if (linevalsplit[0].trimmed() == "scalar")
                    {
                        scalarclass scalar;
                        bool isfloat = false;
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

                        if (linevalsplit[1].trimmed() == "S08")
                        {
                            scalar.size = 1;
                            scalar.signedval = true;
                        }
                        else if (linevalsplit[1].trimmed() == "S16")
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
                        else if (linevalsplit[1].trimmed() == "F32")
                        {
                            scalar.size = 4;
                            scalar.signedval = true;
                            isfloat = true;
                        }
                        scalar.unit = linevalsplit[3].trimmed().mid(1,linevalsplit[3].trimmed().length()-2);
                        //dataLogList.append(scalar);
                        DataField f(linesplit[0].trimmed(),"",scalar.offset,scalar.size,scalar.scale,scalar.translate,0,0,scalar.signedval,isfloat);
                        f.setIsBig(m_bigEndian);
                        dataLogMap.append(f);
                        //dataLogMap[linesplit[0].trimmed()] = scalar;
                    }
                }
            }
        }
        if (section == 1)
        {
            if (line.startsWith("nPages"))
            {
                //Number of pages
                if (line.contains("="))
                {
                    nPages = line.split("=")[1].trimmed().toInt();
                }
                for (int i=0;i<nPages;i++)
                {
                    m_pageInfoList.append(PageInfoStruct());
                }
            }
            else if (line.startsWith("blockingFactor"))
            {
                m_commandStructure.blockingFactor = line.split("=")[1].trimmed().toInt();
            }
            else if (line.startsWith("endianness"))
            {
                if (line.split("=")[1].trimmed().contains("big"))
                {
                    m_bigEndian = true;
                }
                else
                {
                    m_bigEndian = false;
                }
            }
            else if (line.startsWith("page=") || line.startsWith("page "))
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
                    pagenum = pagenum.mid(0,pagenum.indexOf(";")).replace("\"","").trimmed();
                    qDebug() << "New pagenum" << pagenum;

                    pagenumint = pagenum.toInt()-1;
                    pagenum = QString::number(pagenumint);
                    int pagesize = pageSize[pagenumint];
                    QList<ConfigBlock> blocklist;
                }
                else
                {
                    qDebug() << "Page iwthout" << line;
                }
                //qDebug() << "Page Num:" << pagenum;
            }
            else if (!line.trimmed().startsWith(";"))
            {
                if (line.trimmed().startsWith("pageIdentifier"))
                {
                    //Page identifiers, these are important!
                    QString idents = line.trimmed().split("=")[1];
                    QStringList identsplit = idents.split(",");
                    for (int j=0;j<identsplit.size();j++)
                    {
                        pageIdent.append(identsplit[j].trimmed());
                        m_pageInfoList[j].pageIdentifier = identsplit[j].trimmed().replace("\"","");
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
                        m_pageInfoList[j].pageSize = pageint;
                        m_pageBufferMap[j] = QByteArray();
                        m_pageBufferFilledMap[j] = 0;
                        m_pageBufferMap[j].resize(pageint);
                    }
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
                else if (line.startsWith("pageReadCommand"))
                {
                    QString pagecommands = line.trimmed().split("=")[1];
                    QStringList commandsplit = pagecommands.split(",");
                    for (int j=0;j<commandsplit.size();j++)
                    {
                        m_pageInfoList[j].pageReadCommand = convertString(commandsplit[j].trimmed().replace("\"",""));
                    }
                }
                else if (line.startsWith("pageValueWrite"))
                {
                    QString pagecommands = line.trimmed().split("=")[1];
                    QStringList commandsplit = pagecommands.split(",");
                    for (int j=0;j<commandsplit.size();j++)
                    {
                        m_pageInfoList[j].pageValueCommand = commandsplit[j].trimmed().replace("\"","");
                    }
                }
                else if (line.startsWith("pageChunkWrite"))
                {
                    QString pagecommands = line.trimmed().split("=")[1];
                    QStringList commandsplit = pagecommands.split(",");
                    for (int j=0;j<commandsplit.size();j++)
                    {
                        m_pageInfoList[j].pageChunkCommand = commandsplit[j].trimmed().replace("\"","");
                    }
                }
                else if (line.startsWith("crc32CheckCommand"))
                {
                    QString pagecommands = line.trimmed().split("=")[1];
                    QStringList commandsplit = pagecommands.split(",");
                    for (int j=0;j<commandsplit.size();j++)
                    {
                        m_pageInfoList[j].crc32CheckCommand = commandsplit[j].trimmed().replace("\"","");
                    }
                }
                QStringList linesplit = line.split("=");
                if (linesplit.size() > 1)
                {
                    //QStringList linevalsplit = linesplit[1].split(",");
                    QStringList linevalsplit = splitValueLine(linesplit[1]);
                    if (linevalsplit.size() > 5)
                    {
                        //qDebug() << linesplit[1];
                        if (linevalsplit[0].trimmed() == "scalar")
                        {
                            //Scalar
                            ScalarConfigData *data = new ScalarConfigData();
                            data->setBigEndian(m_bigEndian);
                            connect(data,&ScalarConfigData::saveSignal,this,&MSPComms::memorySaveSlot);
                            data->setOffset(linevalsplit[2].trimmed().toInt());
                            data->setType(ConfigData::VALUE);
                            QList<QPair<QString,double> > calclist;
                            if (linevalsplit[4].trimmed().startsWith("."))
                            {
                                calclist.append(QPair<QString,double>("mult",QString("0").append(linevalsplit[4].trimmed()).toFloat()));
                            }
                            else
                            {
                                calclist.append(QPair<QString,double>("mult",linevalsplit[4].trimmed().toFloat()));
                            }
                            if (linevalsplit[5].trimmed().startsWith("."))
                            {
                                calclist.append(QPair<QString,double>("add",QString("0").append(linevalsplit[5].trimmed()).toFloat()));
                            }
                            else
                            {
                                calclist.append(QPair<QString,double>("add",linevalsplit[5].trimmed().toFloat()));
                            }
                            data->setCalc(calclist);
                            if (linevalsplit[1].trimmed() == "S08")
                            {
                                data->setElementSize(1);
                                data->setElementType(ConfigData::SIGNED_ELEMENT);

                            }
                            else if (linevalsplit[1].trimmed() == "S16")
                            {
                                data->setElementSize(2);
                                data->setElementType(ConfigData::SIGNED_ELEMENT);

                            }
                            else if (linevalsplit[1].trimmed() == "U08")
                            {
                                data->setElementSize(1);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "U16")
                            {
                                data->setElementSize(2);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "U32")
                            {
                                data->setElementSize(4);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "S32")
                            {
                                data->setElementType(ConfigData::SIGNED_ELEMENT);
                                data->setElementSize(4);
                            }
                            else if (linevalsplit[1].trimmed() == "F32")
                            {
                                data->setElementSize(4);
                                data->setElementType(ConfigData::FLOAT_ELEMENT);
                                data->setType(ConfigData::FLOAT);
                            }

                            data->setSize(1);
                            data->setName(linesplit[0].trimmed());
                            data->setPage(pagenum.toInt());

                            m_scalarDataMap[linesplit[0].trimmed()] = data;
                            m_configNameList.append(linesplit[0].trimmed());
                        } //if (linevalsplit[0].trimmed() == "scalar")
                        else if (linevalsplit[0].trimmed() == "bits")
                        {
                            unsigned int offset = linevalsplit[2].trimmed().toInt();
                            BitConfigData *data;
                            if (m_bitDataLocationMap.contains(offset))
                            {
                                data = m_bitDataLocationMap[offset];
                            }
                            else
                            {
                                data = new BitConfigData();
                                data->setBigEndian(m_bigEndian);
                                data->setPage(pagenum.toInt());
                                connect(data,&BitConfigData::saveSignal,this,&MSPComms::memorySaveSlot);
                                m_bitDataLocationMap.insert(offset,data);
                            }
                            QString bits = linevalsplit[3].trimmed().replace("[","").replace("]","");
                            unsigned int min = bits.split(":")[0].toInt();
                            unsigned int max = bits.split(":")[1].toInt();
                            QList<QString> bitarray;
                            for (int i=4;i<linevalsplit.size();i++)
                            {
                                bitarray.append(linevalsplit[i].trimmed().replace("\"",""));
                            }
                            data->addBitField(linesplit[0].trimmed(),min,max,bitarray);

                            data->setType(ConfigData::ENUM);
                            data->setOffset(offset);
                            if (linevalsplit[1].trimmed() == "S08")
                            {
                                data->setElementSize(1);
                                data->setSize(1);
                                data->setElementType(ConfigData::SIGNED_ELEMENT);

                            }
                            else if (linevalsplit[1].trimmed() == "S16")
                            {
                                data->setElementSize(2);
                                data->setSize(2);
                                data->setElementType(ConfigData::SIGNED_ELEMENT);

                            }
                            else if (linevalsplit[1].trimmed() == "U08")
                            {
                                data->setElementSize(1);
                                data->setSize(1);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "U16")
                            {
                                data->setElementSize(2);
                                data->setSize(2);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "U32")
                            {
                                data->setElementSize(4);
                                data->setSize(4);
                                data->setElementType(ConfigData::UNSIGNED_ELEMENT);
                            }
                            else if (linevalsplit[1].trimmed() == "S32")
                            {
                                data->setElementType(ConfigData::SIGNED_ELEMENT);
                                data->setElementSize(4);
                                data->setSize(4);
                            }
                            else if (linevalsplit[1].trimmed() == "F32")
                            {
                                data->setElementSize(4);
                                data->setSize(4);
                                data->setElementType(ConfigData::FLOAT_ELEMENT);
                                data->setType(ConfigData::FLOAT);
                            }
                            data->setName(linesplit[0].trimmed());
                            m_bitDataNameMap[linesplit[0].trimmed()] = data;

                            m_configNameList.append(linesplit[0].trimmed());
                        }
                        else if (linevalsplit[0].trimmed() == "array")
                        {
                            QString format = linevalsplit[3].trimmed();
                            if (format.contains("x"))
                            {
                                TableConfigData *data = tableConfigFromIniLine(linesplit[0].trimmed(),linevalsplit);
                                data->setPage(pagenum.toInt());
                                m_tableDataMap.insert(linesplit[0].trimmed(),data);
                            }
                            else
                            {
                                ArrayConfigData *data = arrayConfigFromIniLine(linesplit[0].trimmed(),linevalsplit);
                                data->setPage(pagenum.toInt());
                                m_arrayDataMap.insert(linesplit[0].trimmed(),data);
                            }
                            m_configNameList.append(linesplit[0].trimmed());
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
                        if (linesplit[1].contains(","))
                        {
                            tableMap[currenttablevar]->xbin = linesplit[1].split(",")[0].trimmed();
                        }
                        else
                        {
                            tableMap[currenttablevar]->xbin = linesplit[1].trimmed();
                        }
                    }
                    else if (linesplit[0].trimmed() == "yBins")
                    {
                        if (linesplit[1].contains(","))
                        {
                            tableMap[currenttablevar]->ybin = linesplit[1].split(",")[0].trimmed();
                        }
                        else
                        {
                            tableMap[currenttablevar]->ybin = linesplit[1].trimmed();
                        }
                    }
                    else if (linesplit[0].trimmed() == "zBins")
                    {
                        if (linesplit[1].contains(","))
                        {
                            tableMap[currenttablevar]->zbin = linesplit[1].split(",")[0].trimmed();
                        }
                        else
                        {
                            tableMap[currenttablevar]->zbin = linesplit[1].trimmed();
                        }
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
                        QString xbin = linesplit[1].trimmed();
                        if (xbin.contains(","))
                        {
                            xbin = xbin.split(",")[0].trimmed();
                        }
                        curveMap[currentcurvevar]->xbin = xbin;
                    }
                    else if (linesplit[0].trimmed() == "yBins")
                    {
                        QString ybin = linesplit[1].trimmed();
                        if (ybin.contains(","))
                        {
                            ybin = ybin.split(",")[0].trimmed();
                        }
                        curveMap[currentcurvevar]->ybin = ybin;
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




    QMap<QString,QList<ConfigBlock> > configmap;
    for (QMap<QString,QMap<QString,scalarclass> >::const_iterator i = pageMap.constBegin();i!=pageMap.constEnd();i++)
    {
        int pagenum = i.key().toInt();
        qDebug() << "Page:" << pagenum;
        int pagesize = pageSize[pagenum];
        QList<ConfigBlock> blocklist;
        m_pageSizeList.append(pagesize);
        for (QMap<QString,scalarclass>::const_iterator j = i.value().constBegin();j!=i.value().constEnd();j++)
        {
            TSBPConfigData *data = new TSBPConfigData();
            data->setElementSize(j.value().size);
            data->setSize(1);
            data->setName(j.key());
            data->setOffset(j.value().offset);
            data->setLocationId(pagenum);
            data->setType(ConfigData::VALUE);
            QList<QPair<QString,double> > calclist;
            calclist.append(QPair<QString,double>("add",j.value().translate));
            calclist.append(QPair<QString,double>("mult",j.value().scale));
            data->setCalc(calclist);
            m_configNameList.append(j.key());


        }
        configmap[QString::number(pagenum)] = blocklist;
    }
    //m_dsataDecoder->setDataDefinitions(dataLogMap);
    m_memoryMetaData->passConfigData(configmap);
    m_dataDecoder->setDataDefinitions(dataLogMap);

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
    m_iniFileLoaded = true;
}
MSPComms::MSPComms(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QMap<QString,QString> >("QMap<QString,QString>");
    m_connected = false;
    m_bigEndian = false;
    m_iniFileLoaded = false;
    m_dataDecoder = new TSBPDataPacketDecoder();
    connect(this,SIGNAL(dataLogPayloadReceived(QByteArray)),m_dataDecoder,SLOT(decodePayload(QByteArray)));
    connect(m_dataDecoder,SIGNAL(payloadDecoded(QVariantMap)),this,SIGNAL(dataLogPayloadDecoded(QVariantMap)));
    m_memoryMetaData = new TSBPMemoryMetaData();
    qRegisterMetaType<QList<unsigned short> >("QList<unsigned short>");
    currentPacketNum=1;
    m_interrogateInProgress = false;
    m_interrogateComplete = false;
    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(packetCounter()));
    timer->start(10000);

    m_threadRun = true;
    m_waitingForPacketResponse = false;
    savePageTimer = new QTimer(this);
    connect(savePageTimer,SIGNAL(timeout()),this,SLOT(savePageTimerTick()));

    connect(&m_fileDownloader,&FileDownloader::fileDownloaded,this,&MSPComms::fileDownloaded);
    connect(&m_fileDownloader,&FileDownloader::fileDownloadFailed,this,&MSPComms::fileDownloadFailed);


}
int MSPComms::sendBurn()
{
//    QMutexLocker locker(&reqListMutex);
    RequestClass req;
    req.type = BURN_BLOCK_FROM_RAM_TO_FLASH;
    req.sequencenumber = currentPacketNum++;
    m_reqList.append(req);
    triggerNextSend();
    return currentPacketNum-1;
}
void MSPComms::memorySaveSlot()
{
    //Sender will be a ConfigData.
    ConfigData *data = qobject_cast<ConfigData*>(sender());
    if (!data)
    {
        qDebug() << "memorySaveSlot called with invalid configdata!!!";
        return;
    }
    qDebug() << "Saving offset:" << data->offset() << "Size:" << data->size() << "Bytes:" << data->getBytes().toHex();
    updateBlockInFlash(0,data->offset(),data->size(),data->getBytes());
    sendBurn();
}
void MSPComms::getConsoleTextTimerTimeout()
{
    RequestClass req;
    req.type = GET_CONSOLE;
    req.sequencenumber = currentPacketNum++;
    m_reqList.append(req);
    triggerNextSend();
    //sendPacket(req);
}
Table* MSPComms::getTableFromName(QString name)
{
    if (tableMap.contains(name))
    {
        return tableMap[name];
    }
    return 0;
}
Curve* MSPComms::getCurveFromName(QString name)
{
    if (curveMap.contains(name))
    {
        return curveMap[name];
    }
    return 0;
}
QStringList MSPComms::splitline(QString csvline)
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

void MSPComms::saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size)
{
    updateBlockInFlash(locationid,offset,size,data);
    if (!m_dirtyPageList.contains(locationid))
    {
        m_dirtyPageList.append(locationid);
        emit memoryDirty();
    }

}
void MSPComms::pageBurnRequested(unsigned int page)
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
void MSPComms::savePageTimerTick()
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

int MSPComms::startBenchTest(unsigned char eventspercycle, unsigned short numcycles, unsigned short ticksperevent, QVariantList pineventmask, QVariantList pinmode)
{
    Q_UNUSED(eventspercycle)
    Q_UNUSED(numcycles)
    Q_UNUSED(ticksperevent)
    Q_UNUSED(pineventmask)
    Q_UNUSED(pinmode)
    return -1; //Not supported
}

int MSPComms::stopBenchTest()
{
    return -1; //Not supported

}

int MSPComms::bumpBenchTest(unsigned char cyclenum)
{
    Q_UNUSED(cyclenum)
    return -1; //Not supported

}

ConfigData* MSPComms::getConfigData(QString name)
{
    if (m_configDataMap.contains(name))
    {
        return m_configDataMap[name];
    }
    return 0;
}
ArrayConfigData *MSPComms::getArrayConfigData(QString name)
{
    if (m_arrayDataMap.contains(name))
    {
        return m_arrayDataMap[name];
    }
    return 0;
}
TableConfigData *MSPComms::getTableConfigData(QString name)
{
    if (m_tableDataMap.contains(name))
    {
        return m_tableDataMap[name];
    }
    return 0;
}
ScalarConfigData *MSPComms::getScalarConfigData(QString name)
{
    if (m_scalarDataMap.contains(name))
    {
        return m_scalarDataMap[name];
    }
    return 0;
}
BitConfigData *MSPComms::getBitConfigData(QString name)
{
    if (m_bitDataNameMap.contains(name))
    {
        return m_bitDataNameMap[name];
    }
    return 0;
}
QList<QString> MSPComms::getConfigList()
{
    return m_configNameList;
}

void MSPComms::writeAllRamToRam()
{

}
TSBPTable2DData* MSPComms::get2DTableData(QString varname)
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

TSBPTable2DData* MSPComms::get2DTableData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return 0;
}

TSBPTable3DData* MSPComms::get3DTableData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return 0;
}
TSBPTable3DData* MSPComms::get3DTableData(QString locationname)
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

RawData* MSPComms::getRawData(unsigned short locationid)
{
    Q_UNUSED(locationid)
    return 0;
}
void MSPComms::startInterrogation()
{
    if (!m_interrogateInProgress)
    {
        //m_interrogatePacketList.clear();
        m_interrogateInProgress = true;
        interrogateTaskStart("Ecu Info",getFirmwareVersion());
        QByteArray pagereq;
        pagereq.append("R");
    }
}

void MSPComms::stop()
{
    m_threadRun = false;
}

void MSPComms::setLogsEnabled(bool enabled)
{
    Q_UNUSED(enabled)
    //Q_UNUSED(locationid)
}
TSBPMemoryMetaData *MSPComms::getMetaParser()
{
    return m_memoryMetaData;
}

TSBPDataPacketDecoder *MSPComms::getDecoder()
{
    return m_dataDecoder;
}

TSBPTable3DData *MSPComms::getNew3DTableData()
{
    return 0;
}

TSBPTable2DData *MSPComms::getNew2DTableData()
{
    return 0;
}

void MSPComms::setLogDirectory(QString dir)
{
    Q_UNUSED(dir)
}
int MSPComms::enableDatalogStream()
{
    return 0;
}

int MSPComms::disableDatalogStream()
{
    return 0;
}

void MSPComms::setPort(QString portname)
{
    m_portName = portname;
}

void MSPComms::setBaud(int baudrate)
{
    Q_UNUSED(baudrate)
}

void MSPComms::setLogFileName(QString filename)
{
    Q_UNUSED(filename)
}


void MSPComms::sendPacket(RequestClass req)
{
    QThread::msleep(10);
    //qDebug() << "sendPacket called";

    m_waitingForPacketResponse = true;
    if (req.type == RETRIEVE_PAGE)
    {
        //Retrieve page!
        QByteArray pagereqstr = req.args.at(0).toByteArray(); //Page Name
        qDebug() << "Sending Page request:" << pagereqstr.toHex();
        qDebug() << "Sending Page request:" << pagereqstr;
        m_currentRequest = req;
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
        qDebug() << "Writing page req packet";
        if (m_connectionType == TCPSOCKET)
        {
            m_tcpPort->write(packet);
        }
        else
        {
            m_serialPort->write(packet);
        }
    }
    else if (req.type == GET_FIRMWARE_VERSION)
    {
        qDebug() << "Getting firmware version";
        m_currentRequest = req;
        QByteArray req = QByteArray("S"); // Hardcoded since it's in the spec PDF
        uint32_t crc = Crc32_ComputeBuf(0,req.data(),req.size());
        QByteArray packet;
        packet.append((char)0x0);
        packet.append((char)0x1);
        packet.append(req);
        packet.append(crc >> 24);
        packet.append(crc >> 16);
        packet.append(crc >> 8);
        packet.append(crc >> 0);
        qDebug() << "Writing firmware version packet";
        qDebug() << packet.toHex();
        if (m_connectionType == TCPSOCKET)
        {
            m_tcpPort->write(packet);
        }
        else
        {
            m_serialPort->write(packet);
        }
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
        //reqdata.append(static_cast<char>(0x0));
        //reqdata.append(static_cast<char>(0x0));
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
        //m_serialPort->requestPacket(packet,0);
        qDebug() << "Writing update block req packet";
        if (m_connectionType == TCPSOCKET)
        {
            m_tcpPort->write(packet);
        }
        else
        {
            m_serialPort->write(packet);
        }


    }
    else if (req.type == BURN_BLOCK_FROM_RAM_TO_FLASH)
    {
        m_currentRequest = req;
        QByteArray req = "B";
        uint32_t crc = Crc32_ComputeBuf(0,req.data(),req.size());
        QByteArray packet;
        packet.append((char)0x0);
        packet.append((char)0x1);
        packet.append(req);
        packet.append(crc >> 24);
        packet.append(crc >> 16);
        packet.append(crc >> 8);
        packet.append(crc >> 0);
        qDebug() << "Writing firmware version packet";
        //m_serialPort->requestPacket(packet,0);
        if (m_connectionType == TCPSOCKET)
        {
            m_tcpPort->write(packet);
        }
        else
        {
            m_serialPort->write(packet);
        }

    }
    else if (req.type == GET_DATA)
    {

        m_currentRequest = req;
        //m_serialPort->requestPacket(QByteArray("H"),0);

        QByteArray reqbytes = m_commandStructure.ochGetCommand.toLatin1();
        if (reqbytes.contains("%2o") && reqbytes.contains("%2c"))
        {
        int offset = req.args.at(0).toInt(); //Size
        int size = req.args.at(1).toInt(); //Size
        //QByteArray data = req.args.at(3).toByteArray();
        //qDebug() << "Sending update block:" << data.toHex() << offset << size;
        //m_serialPort->requestPacket(pagename.toLocal8Bit(),0);
        //m_serialPort->requestPacket(QString("V").toLocal8Bit(),size);

        /*if (m_bigEndian)
        {
            offset.append((char)((i >> 8) & 0xFF));
            offset.append((char)i & 0xFF);
        }
        else
        {
            offset.append((char)i & 0xFF);
            offset.append((char)((i >> 8) & 0xFF));
        }*/


        QByteArray offsetbytes;
        offsetbytes.append(static_cast<char>(offset & 0xFF));
        offsetbytes.append(static_cast<char>((offset >> 8) & 0xFF));
        QByteArray sizebytes;
        sizebytes.append(static_cast<char>(size & 0xFF));
        sizebytes.append(static_cast<char>((size >> 8) & 0xFF));
        reqbytes = reqbytes.replace("%2o",offsetbytes).replace("%2c",sizebytes);
        //qDebug() << "O:" << offset << "S:" << size;
        }
        m_currentRequest = req;

        //req.append((char)0x0);
        //req.append((char)0x0);
        //req.append((char)0xF4);
        //req.append((char)0x1);
        uint32_t crc = Crc32_ComputeBuf(0,reqbytes.data(),reqbytes.size());
        QByteArray packet;
        packet.append((char)0x00);
        packet.append(reqbytes.length());
        packet.append(reqbytes);
        packet.append(crc >> 24);
        packet.append(crc >> 16);
        packet.append(crc >> 8);
        packet.append(crc >> 0);
        //qDebug() << "Writing get data packet";
        //m_serialPort->requestPacket(packet,0);
        //qDebug() << "Getting data frame" << packet.toHex();
        m_serialPort->write(packet);
    }
    else if (req.type == GET_CONSOLE)
    {
        m_currentRequest = req;
        //m_serialPort->requestPacket(QByteArray("H"),0);
        QByteArray req = "G";
        //req.append((char)0x0);
        //req.append((char)0x0);
        //req.append((char)0xF4);
        //req.append((char)0x1);
        uint32_t crc = Crc32_ComputeBuf(0,req.data(),req.size());
        QByteArray packet;
        packet.append((char)0x00);
        packet.append(0x01);
        packet.append(req);
        packet.append(crc >> 24);
        packet.append(crc >> 16);
        packet.append(crc >> 8);
        packet.append(crc >> 0);
        //qDebug() << "Writing get data packet";
        //m_serialPort->requestPacket(packet,0);
        m_serialPort->write(packet);
    }
    else
    {
        qDebug() << "Unknown request type!";
    }
}



int MSPComms::getInterfaceVersion()
{
    qDebug() << "getInterfaceVersion";
    QMutexLocker locker(&reqListMutex);
    RequestClass req;
    req.type = GET_INTERFACE_VERSION;
    req.sequencenumber = currentPacketNum++;
    m_reqList.append(req);
    return currentPacketNum-1;
}

int MSPComms::getFirmwareVersion()
{
    //QMutexLocker locker(&reqListMutex);
    RequestClass req;
    req.type = GET_FIRMWARE_VERSION;
    req.sequencenumber = currentPacketNum++;
    m_reqList.append(req);
    triggerNextSend();
    return currentPacketNum-1;
}


int MSPComms::echoPacket(QByteArray packet)
{
    Q_UNUSED(packet)
    return -1;
}

int MSPComms::getLocationIdList(unsigned char listtype, unsigned short listmask)
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

int MSPComms::softReset()
{
    return -1;
}

int MSPComms::hardReset()
{
    return -1;
}

bool MSPComms::sendSimplePacket(unsigned short payload)
{
    Q_UNUSED(payload)
    return false;
}

void MSPComms::connectSerial(bool isserial,QString port,int baud,QString inifile)
{
    m_iniFile = inifile;
//    QMutexLocker locker(&reqListMutex);
    RequestClass req;
    req.type = SERIAL_CONNECT;
    req.addArg(port);
    req.addArg(baud);
    //m_reqList.append(req);

    m_state = 1;

    if (!isserial)
    {
    m_connectionType = TCPSOCKET;

    m_tcpPort = new QTcpSocket(this);
    connect(m_tcpPort,&QTcpSocket::readyRead,this,&MSPComms::handleReadyRead);
    m_tcpPort->connectToHost("localhost",29001);
    m_tcpPort->waitForConnected();
    serialPortConnected();
    return;
    }
    m_connectionType = SERIAL;


    m_serialPort = new QSerialPort();

    connect(m_serialPort, &QSerialPort::readyRead, this, &MSPComms::handleReadyRead);
    connect(m_serialPort,SIGNAL(bytesReady(QByteArray)),this,SLOT(parseBuffer(QByteArray)),Qt::QueuedConnection);
    connect(m_serialPort,SIGNAL(connected()),this,SLOT(serialPortConnected()),Qt::QueuedConnection);

    m_serialPort->setBaudRate(baud);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort->setPortName(port);
    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        qDebug() << "Error opening serialport";
        return;
    }
    m_serialPort->setTextModeEnabled(false);
    m_serialPort->setBreakEnabled(false);
    m_serialPort->setDataTerminalReady(false);
    QThread::msleep(500);
    m_serialPort->setDataTerminalReady(true);
    QThread::msleep(500);
    serialPortConnected();

}
void MSPComms::serialPortConnected()
{
    m_state = 2;
    emit connected();
}
int MSPComms::requestPage(QByteArray pagereqstring,int length,int pageid)
{
    for (int i=0;i<length;i+=255)
    {
        QByteArray nextreq = pagereqstring;
        QByteArray canid;
        canid.append((char)0x0);
        //canid.append((char)0x0);
        QByteArray size;
        if (length-i >= 255)
        {
            if (m_bigEndian)
            {
                size.append((char)0x0);
                size.append((char)0xFF);
            }
            else
            {
                size.append((char)0xFF);
                size.append((char)0x0);
            }
        }
        else
        {
            if (m_bigEndian)
            {
                size.append((char)0x0);
                size.append((char)((length-i) & 0xFF));
            }
            else
            {
                size.append((char)((length-i) & 0xFF));
                size.append((char)0x0);
            }
        }
        QByteArray offset;
        if (m_bigEndian)
        {
            offset.append((char)((i >> 8) & 0xFF));
            offset.append((char)i & 0xFF);
        }
        else
        {
            offset.append((char)i & 0xFF);
            offset.append((char)((i >> 8) & 0xFF));
        }

        qDebug() << "PageReqString:" << nextreq.toHex();
        if (nextreq.contains("\\$tsCanId"))
        {
            nextreq = nextreq.replace("\\$tsCanId",canid);
        }
        if (nextreq.contains("%2i"))
        {
            QString ident = convertString(m_pageInfoList[pageid].pageIdentifier).replace("\\$tsCanId",canid);
            nextreq = nextreq.replace("%2i",ident.toLatin1());
        }
        qDebug() << "PageReqString:" << nextreq.toHex();
        nextreq = nextreq.replace("%2o",offset).replace("%2c",size);
        qDebug() << "PageReqString:" << nextreq.toHex();
        RequestClass req;
        req.type = RETRIEVE_PAGE;
        req.sequencenumber = currentPacketNum++;
        emit interrogateTaskStart(QString("Location ID ") + QString::number(i),currentPacketNum-1);
        req.addArg(nextreq);
        req.addArg(i);
        req.addArg(((length - i >= 255) ? 255 : length-i));
        req.addArg(pageid);
        //i += 255;
        m_reqList.append(req);
    }
    triggerNextSend();
    return currentPacketNum-1;
}
void MSPComms::getDataReq()
{
    for (int i=0;i<m_commandStructure.ochBlockSize;i+=m_commandStructure.blockingFactor)
    {
        RequestClass req;
        req.type = GET_DATA;
        req.addArg(i);
        req.addArg(((m_commandStructure.ochBlockSize-i >= m_commandStructure.blockingFactor) ? m_commandStructure.blockingFactor : m_commandStructure.ochBlockSize-i ));
        req.sequencenumber = currentPacketNum++;
        m_reqList.append(req);

    }
    triggerNextSend();
}
void MSPComms::triggerNextSend()
{
    if (!m_waitingForPacketResponse)
    {
        if (m_reqList.size() > 0)
        {
            //QLOG_DEBUG() << "Sending next packet:" << "0x" + QString::number(m_reqList.at(0).type,16) << "Packets left:" << m_reqList.size();
            RequestClass req = m_reqList.at(0);
            m_reqList.removeAt(0);
            sendPacket(req);
        }
        else
        {
            if (m_interrogateComplete)
            {
                getDataReq();
            }
            //No requests, create a datalog request
        }
    }
    else
    {
        //qDebug() << "triggerNextSend while we have a message in waiting: " << m_currentRequest.type;
    }
}

void MSPComms::handleReadyRead()
{
    QByteArray buffer;
    if (m_connectionType == TCPSOCKET)
    {
        buffer = m_tcpPort->readAll();
    }
    else
    {

        buffer = m_serialPort->readAll();
    }
    m_serialPortBuffer.append(buffer);
    qDebug() << "Incoming data from serial port:" << buffer.size() << "Total:" << m_serialPortBuffer.size();
    bool resend = false;
    while (true)
    {
        if (m_serialPortBuffer.length() < 3)
        {
            //Not enough data to process
            //qDebug() << "Leftover data in buffer: " << m_serialPortBuffer.length();
            break;
        }
        int bufsize = ((static_cast<unsigned int>(m_serialPortBuffer[0]) & 0xFF) << 8) + (static_cast<unsigned int>(m_serialPortBuffer[1]) & 0xFF);
        if (m_serialPortBuffer.length() < (bufsize+6))
        {
            //not enough data to process a full packet
            //qDebug() << "Packet size reads: " << bufsize+6 << "But we've only got" << m_serialPortBuffer.length();
            break;
        }
        //qDebug() << "Packet size reads: " << bufsize << "and we've only got" << m_serialPortBuffer.length();
        //bufsize is the actual payload size. It doens't include the size or CRC, but it does include the flag it seems?
        QByteArray packettoparse = m_serialPortBuffer.mid(0,bufsize+6);
        //m_serialPortBuffer = m_serialPortBuffer.remove(0,bufsize);
        m_serialPortBuffer.remove(0,bufsize+6);
        if (((unsigned char)packettoparse[2]) == 0x80)
        {
            qDebug() << "Buffer underrun, resend command";
            //sendPacket(m_currentRequest);
            resend = true;
            continue;
        }
        if (((unsigned char)packettoparse[2]) == 0x83)
        {
            qDebug() << "unrecognized command, resend command";
            qDebug() << "Type:" << m_currentRequest.type;
            qDebug() << "Args:" << m_currentRequest.args;
            //sendPacket(m_currentRequest);
            //resend = true;
            continue;
        }
        parseBuffer(packettoparse);
    }
    if (resend)
    {
        sendPacket(m_currentRequest);
    }
    else
    {
        triggerNextSend();
    }
}
void MSPComms::fileDownloaded(QFile *file)
{
    loadIniFile(file);
    delete file; // Have to do this here, noone else owns the pointer.
    startInterrogation();

}
void MSPComms::fileDownloadFailed()
{
    if (QMessageBox::question(0,"Error","Unable to download ini file automatically. Do you wish to manually select it?") == QMessageBox::Yes)
    {
        QString filenamestr = QFileDialog::getOpenFileName(0,"Find INI file");
        if (filenamestr.isEmpty())
        {
            return;
        }
        QFile *file = new QFile(filenamestr);
        loadIniFile(file);
        delete file;
        startInterrogation();
    }
}
void MSPComms::parseBuffer(QByteArray data)
{
    //Ignore data, everything is in m_serialPortBuffer
    Q_UNUSED(data)
    //qDebug() << "Bytes available:" << data.size();
    //qDebug() << data.toHex();
    //qDebug() << QString(data);


    /*if (packettoparse[0] != 0x0)
    {
        qDebug() << "Packet error: " << packettoparse[0];
        return;
    }*/
    if ((unsigned int)data[2] != 0 && (unsigned int)data[2] != 1)
    {
        qDebug() << "Packet error:" << (unsigned int)data[2];
    }
    data = data.mid(3,data.length()-7);
    if (m_waitingForPacketResponse == true)
    {
        if (m_currentRequest.type == GET_FIRMWARE_VERSION)
        {
            QString versionstr(data);
            emit interrogateTaskSucceed(m_currentRequest.sequencenumber);
            qDebug() << "Got firmware version:" << versionstr;
            //"rusEFI 2022.06.25.all.2759818183\u0000
            m_firmwareVersion = versionstr;
            if (versionstr.contains("rusEFI"))
            {
                QStringList versionstrlist = versionstr.mid(0,versionstr.length()-1).replace("\"","").split(" ");
                QString path = "https://rusefi.com/online/ini/rusefi/" + versionstrlist[1].replace(".","/") + ".ini";
                if (m_iniFile != "" && !m_iniFileLoaded)
                {
                    QStringList inifilesplit = m_iniFile.split("/");
                    QStringList versionsplit = versionstrlist[1].split("/");
                    QString filename = inifilesplit[inifilesplit.length()-1];
                    if (filename.trimmed() != (versionsplit[versionsplit.length()-1] + ".ini"))
                    {
                        if (QMessageBox::question(0,"Wrong INI File","INI File provided is " + filename + " however firmware needs " + versionsplit[versionsplit.length()-1] + ".ini. Would you like to automatically download the appropraite file?") == QMessageBox::Yes)
                        {
                            m_interrogateInProgress = false;
                            m_fileDownloader.downloadFile(path,"");
                        }
                    }
                    else
                    {
                        QFile *file = new QFile(m_iniFile);
                        loadIniFile(file);
                        delete file;
                        m_interrogateInProgress = false;
                        startInterrogation();
                    }
                }
                else if (!m_iniFileLoaded)
                {
                    m_interrogateInProgress = false;
                    m_fileDownloader.downloadFile(path,"");
                }
                else if (m_iniFileLoaded)
                {
                    for (int i=0;i<m_pageInfoList.size();i++)
                    {
                        interrogateTaskStart("Location ID " + QString::number(i),requestPage(m_pageInfoList[i].pageReadCommand.toLatin1(),m_pageInfoList[i].pageSize,i));
                    }
                }
            }
            else if (versionstr.contains("Speeduino"))
            {
                emit interrogateTaskSucceed(m_currentRequest.sequencenumber);
                if (m_iniFile != "" && !m_iniFileLoaded)
                {
                    QFile *file = new QFile(m_iniFile);
                    loadIniFile(file);
                    delete file;
                    m_interrogateInProgress = false;
                    startInterrogation();
                }
                else if (!m_iniFileLoaded)
                {
                    m_interrogateInProgress = false;
                    QString fwversion = versionstr.split(" ")[1];
                    m_fileDownloader.downloadFile(QString("https://speeduino.com/fw/") + fwversion + ".ini","");
                }
                else if (m_iniFileLoaded)
                {
                    for (int i=0;i<m_pageInfoList.size();i++)
                    {
                        interrogateTaskStart("Location ID " + QString::number(i),requestPage(m_pageInfoList[i].pageReadCommand.toLatin1(),m_pageInfoList[i].pageSize,i));
                    }
                }
            }
            else
            {
                if (m_iniFile != "" && !m_iniFileLoaded)
                {
                    QFile *file = new QFile(m_iniFile);
                    loadIniFile(file);
                    delete file;
                    m_interrogateInProgress = false;
                    startInterrogation();
                }
                else if (!m_iniFileLoaded)
                {
                    //We've got no ini file!
                    if (QMessageBox::information(0,"Error","No INI file provided to match firmware: " + versionstr))
                    {
                        return;
                    }
                }
                else if (m_iniFileLoaded)
                {
                    for (int i=0;i<m_pageInfoList.size();i++)
                    {
                        interrogateTaskStart("Location ID " + QString::number(i),requestPage(m_pageInfoList[i].pageReadCommand.toLatin1(),m_pageInfoList[i].pageSize,i));
                    }
                }
            }
        }
        else if (m_currentRequest.type == GET_DATA)
        {
            QByteArray realtimedata = data;
            //qDebug() << "Data:" << realtimedata;
            int offset = m_currentRequest.args.at(0).toInt(); //Size
            int size = m_currentRequest.args.at(1).toInt(); //Size
            m_dataReqBuffer.append(realtimedata);
            if (offset + size >= m_commandStructure.ochBlockSize)
            {
                //emit dataLogPayloadReceived(m_dataReqBuffer,QByteArray());
                emit dataLogPayloadReceived(m_dataReqBuffer);
                m_dataReqBuffer = QByteArray();
            }
            currentPacketCount++;
        }
        else if (m_currentRequest.type == GET_CONSOLE)
        {

            QByteArray realtimedata = data;
            //qDebug() << "Data:" << realtimedata;
            emit consoleText(realtimedata);

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
            int pageid = m_currentRequest.args.at(3).toInt(); //Size
            qDebug() << "Got page:" << pagereqstr << "size" << size << "offset" << offset << "actualsize" << data.size();
            //Now we have selected the page, submit a request for its data.
            m_pageBufferMap[pageid].replace(offset,size,data);
            m_pageBufferFilledMap[pageid] += size;
            qDebug() << "Size of page" << pageid << "is now:" << m_pageBufferFilledMap[pageid] << ":" << m_pageBufferMap[pageid].size();
            //m_pageBuffer.append(data);
            emit interrogateTaskSucceed(m_currentRequest.sequencenumber);
            if (m_reqList.size() == 0)
            {
                for (QMap<int,QByteArray>::const_iterator page=m_pageBufferMap.constBegin();page!=m_pageBufferMap.constEnd();page++)
                {
                    for (QMap<QString,TableConfigData*>::const_iterator i=m_tableDataMap.constBegin();i!=m_tableDataMap.constEnd();i++)
                    {
                        if (i.value()->page() == page.key())
                        {
                            if (i.value()->name() == "veTable1")
                            {
                                qDebug() << "VETABLE:";
                                qDebug() << page.value();
                            }
                            i.value()->setData(page.value());
                        }
                    }
                    for (QMap<QString,ScalarConfigData*>::const_iterator i=m_scalarDataMap.constBegin();i!=m_scalarDataMap.constEnd();i++)
                    {
                        if (i.value()->page() == page.key())
                        {
                            i.value()->setData(page.value());
                        }

                    }
                    for (QMap<QString,BitConfigData*>::const_iterator i=m_bitDataNameMap.constBegin();i!=m_bitDataNameMap.constEnd();i++)
                    {
                        if (i.value()->page() == page.key())
                        {
                            i.value()->setData(page.value());
                        }
                    }


                    for (QMap<QString,ArrayConfigData*>::const_iterator i=m_arrayDataMap.constBegin();i!=m_arrayDataMap.constEnd();i++)
                    {
                        if (i.value()->page() == page.key())
                        {
                            i.value()->setData(page.value());
                        }
                    }
                }
                if (m_firmwareVersion.contains("rusEFI"))
                {
                    QTimer *timer = new QTimer();
                    connect(timer,SIGNAL(timeout()),this,SLOT(getConsoleTextTimerTimeout()));
                    timer->start(1000);
                }
                m_interrogateComplete = true;
                m_interrogateInProgress = false;
                emit interrogationComplete();
            }
            QString pagenum = m_currentRequest.args.at(0).toString();
            pagenum = pagenum.mid(1);
            int i = 0;
            while(i < pagenum.length() && pagenum[i]=='0'){
             i++;
            }

            pagenum.remove(0,i);
            //int locid = pagenum[0].toLatin1();

        }
        m_waitingForPacketResponse = false;
    }
    else
    {
        qDebug() << "Incoming data, but not waiting for a response right now!";
    }

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

void MSPComms::disconnectSerial()
{

}

void MSPComms::loadLog(QString filename)
{
    Q_UNUSED(filename)
}

void MSPComms::playLog()
{

}

//virtual void populateDataFields()=0;
QByteArray MSPComms::generatePacket(QByteArray header,QByteArray payload)
{
    Q_UNUSED(header)
    Q_UNUSED(payload);
    return QByteArray();
}

int MSPComms::updateBlockInRam(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)
{
    Q_UNUSED(location)
    Q_UNUSED(offset)
    Q_UNUSED(size)
    Q_UNUSED(data)
    return -1;
}

int MSPComms::updateBlockInFlash(unsigned short location,unsigned short offset, unsigned short size,QByteArray data)
{
    //QMutexLocker locker(&reqListMutex);
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

int MSPComms::retrieveBlockFromRam(unsigned short location, unsigned short offset, unsigned short size,bool mark)
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

int MSPComms::retrieveBlockFromFlash(unsigned short location, unsigned short offset, unsigned short size,bool mark)
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

int MSPComms::burnBlockFromRamToFlash(unsigned short location,unsigned short offset, unsigned short size)
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

void MSPComms::setInterByteSendDelay(int milliseconds)
{
    Q_UNUSED(milliseconds)
}

void MSPComms::setlogsDebugEnabled(bool enabled)
{
    Q_UNUSED(enabled)
}
void MSPComms::packetCounter()
{
    qDebug() << "PPS:" << currentPacketCount;
    currentPacketCount = 0;
}
QByteArray MSPComms::readPacket(QSerialPort *port)
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
    //qDebug() << "Test read. Bytes left:" << port->serialBufferSize();
    unsigned int newlen = (((unsigned char)lenbytes[0]) << 8) + (unsigned char)lenbytes[1];
    qDebug() << "Len:" << newlen;
    unsigned int datalenread =0;
    QByteArray packet;
//	int timeoutcount = 0;
    unsigned int datatoberead = newlen;
    int currentdataread = 0;

    QByteArray newbuf3;
    newbuf3 = port->read(1000);
    currentdataread = newbuf3.length();
    //currentdataread = port->readBytes(&newbuf3,newlen,1000);
    if (currentdataread <= 0)
    {
        return QByteArray();
    }
    packet.append(newbuf3);


    QByteArray newbuf6;

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
void MSPComms::run()
{
    unsigned int timercounter = 0;
    while (true)
    {
        timercounter++;
        return;
    }
}
