/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "tableview2d.h"
#include <QMessageBox>
#include <QFileDialog>
#include "tablewidgetdelegate.h"
#include <QPair>
//#include "QsLog.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QMimeData>

//#include "freeems/fetable2ddata.h"
TableView2D::TableView2D(QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(parent)
    //m_isSignedData = isSigned;
    m_isFlashOnly = false;
    ui.setupUi(this);
    metaDataValid = false;
    //tableData=0;
    //ui.tableWidget->setColumnCount(1);
    ui.tableWidget->setRowCount(2);
    ui.tableWidget->horizontalHeader()->hide();
    ui.tableWidget->verticalHeader()->hide();
    //ui.tableWidget->setColumnWidth(0,100);
    connect(ui.savePushButton,SIGNAL(clicked()),this,SLOT(saveClicked()));
    connect(ui.loadFlashPushButton,SIGNAL(clicked()),this,SLOT(loadFlashClicked()));
    connect(ui.loadRamPushButton,SIGNAL(clicked()),this,SLOT(loadRamClicked()));
    connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
    connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
    connect(ui.exportPushButton,SIGNAL(clicked()),this,SLOT(exportClicked()));
    connect(ui.importPushButton,SIGNAL(clicked()),this,SLOT(importClicked()));
    connect(ui.tableWidget,SIGNAL(hotKeyPressed(int,Qt::KeyboardModifiers)),this,SLOT(hotKeyPressed(int,Qt::KeyboardModifiers)));
    ui.tableWidget->addHotkey(Qt::Key_Plus,Qt::ShiftModifier);
    ui.tableWidget->addHotkey(Qt::Key_Minus,Qt::NoModifier);
    ui.tableWidget->addHotkey(Qt::Key_Underscore,Qt::ShiftModifier);
    ui.tableWidget->addHotkey(Qt::Key_Equal,Qt::NoModifier);
    ui.tableWidget->setItemDelegate(new TableWidgetDelegate());

    /*ui.plot->setInteraction(QCP::iRangeDrag, false);
    ui.plot->setInteraction(QCP::iRangeZoom, false);
    ui.plot->plotLayout()->clear();
    m_wideAxisRect = new QCPAxisRect(ui.plot);
    m_wideAxisRect->setupFullAxesBox(true);
    m_wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(false);
    //m_wideAxisRect->removeAxis(m_wideAxisRect->axis(QCPAxis::atLeft,0));
    //m_wideAxisRect->removeAxis(m_wideAxisRect->axis(QCPAxis::atBottom,0));
    ui.plot->plotLayout()->addElement(0, 0, m_wideAxisRect);
    ui.plot->addGraph(m_wideAxisRect->axis(QCPAxis::atBottom), m_wideAxisRect->axis(QCPAxis::atLeft,0));*/



    //curve->setData()
    /*if (!isram)
    {
        //Is only flash
        ui.loadRamPushButton->setVisible(false);
    }
    else if (!isflash)
    {
        //Is only ram
        ui.loadFlashPushButton->setVisible(false);
        ui.savePushButton->setVisible(false);
    }
    else
    {
        //Is both ram and flash
    }*/
    connect(ui.tracingCheckBox,SIGNAL(stateChanged(int)),this,SLOT(tracingCheckBoxStateChanged(int)));
}
void TableView2D::hotKeyPressed(int key,Qt::KeyboardModifiers modifier)
{
    if (key == Qt::Key_Plus || key == Qt::Key_Equal)
    {
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            return;
        }
        QList<QPair<QPair<int,int>,double> > vallist;
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            QString text = ui.tableWidget->selectedItems()[i]->text();
            double textd = text.toDouble();
            double maxval = 0;
            //int dp = 0;
            if (ui.tableWidget->selectedItems()[i]->row() == 0)
            {
               // maxval = tableData->maxCalcedXAxis();
            //	dp = m_metaData.xDp;
            }
            else /*if (ui.tableWidget->selectedItems()[0]->row() == 1)*/
            {
               // maxval = tableData->maxCalcedYAxis();
            //	dp = m_metaData.yDp;
            }

            if (modifier & Qt::ShiftModifier)
            {
                textd += maxval / 10.0;
            }
            else
            {
                textd += maxval / 100.0;
            }
            if (textd > maxval)
            {
                ///TODO: Talk to fredcooke about DP, and why it makes it impossible
                //to set any short value to exactly 65535. This is probably unintentional,
                //but certainly not good.
                textd = maxval;
            }
            else
            {
                int stopper = 1;
                stopper++;
            }
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(ui.tableWidget->selectedItems()[i]->row(),ui.tableWidget->selectedItems()[i]->column());
            val.second = textd;
            vallist.append(val);
        }
        setRange(vallist);



        //ui.tableWidget->selectedItems()[0]->setText(QString::number(textd,'f',dp));
        //setValue(ui.tableWidget->selectedItems()[0]->row(),ui.tableWidget->selectedItems()[0]->column(),textd);
    }
    else if (key == Qt::Key_Minus || key == Qt::Key_Underscore)
    {
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            return;
        }
        QList<QPair<QPair<int,int>,double> > vallist;
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            QString text = ui.tableWidget->selectedItems()[i]->text();
            double textd = text.toDouble();
            double minval = 0;
            double maxval = 0;
            //int dp = 0;
            if (ui.tableWidget->selectedItems()[i]->row() == 0)
            {
                //minval = tableData->minCalcedXAxis();
                //maxval = tableData->maxCalcedXAxis();
            //	dp = m_metaData.xDp;
            }
            else if (ui.tableWidget->selectedItems()[i]->row() == 1)
            {
                //minval = tableData->minCalcedYAxis();
                //maxval = tableData->maxCalcedYAxis();
            //	dp = m_metaData.yDp;
            }
            if (modifier & Qt::ShiftModifier)
            {
                textd -= maxval / 10.0;
            }
            else
            {
                textd -= maxval / 100.0;
            }
            if (textd < minval)
            {
                textd = minval;
            }
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(ui.tableWidget->selectedItems()[i]->row(),ui.tableWidget->selectedItems()[i]->column());
            val.second = textd;
            vallist.append(val);
        }
        setRange(vallist);
    }
}
void TableView2D::setRange(QList<QPair<QPair<int,int>,double> > data)
{
    ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
    ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
    QMap<int,QMap<int,QString> > tmpvaluemap;
    bool valid = true;
    QList<QString> invalidreasons;
    for (int i=0;i<data.size();i++)
    {
        if (!tmpvaluemap.contains(data[i].first.first))
        {
            tmpvaluemap[data[i].first.first] = QMap<int,QString>();
        }
        tmpvaluemap[data[i].first.first][data[i].first.second] = ui.tableWidget->item(data[i].first.first,data[i].first.second)->text();
        QString formatstr = formatNumber(data[i].second,2);
        QString verifystr = verifyValue(data[i].first.first,data[i].first.second,formatstr);
        if (verifystr != "GOOD")
        {
            invalidreasons.append(verifystr);
            valid = false;
        }
        ui.tableWidget->item(data[i].first.first,data[i].first.second)->setText(formatstr);
    }
    //Write the values, and re-enable the signals.
    if (valid)
    {
        writeTable(true);
        if (data.size() == 1)
        {
            updateTable();
            //reColorTable(data[0].first.first,data[0].first.second);
        }
        else
        {
            updateTable();
            //reColorTable(-1,-1);
        }
    }
    else
    {
        for (QMap<int,QMap<int,QString> >::const_iterator i=tmpvaluemap.constBegin();i!=tmpvaluemap.constEnd();i++)
        {
            for (QMap<int,QString>::const_iterator j=i.value().constBegin();j!=i.value().constEnd();j++)
            {
                ui.tableWidget->item(i.key(),j.key())->setText(j.value());
            }
        }
        QString errorstr = "";
        foreach(QString reason,invalidreasons)
        {
            errorstr += reason + ",";
        }
        QMessageBox::information(0,"Error",errorstr);
    }
    connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
    connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
}
QString TableView2D::verifyValue(int row,int column,QString item)
{
    Q_UNUSED(column)
    double tempValue = item.toDouble();

    if (row == 0)
    {
        /*if (tempValue > tableData->maxCalcedXAxis())
        {
            return "Axis value entered too large!";
        }
        else if (tempValue < tableData->minCalcedXAxis())
        {
            return "Axis value entered too small";
        }*/
    }
    else
    {/*
        if (tempValue > tableData->maxCalcedYAxis())
        {
            return "Axis value too large";
        }
        else if (tempValue < tableData->minCalcedYAxis())
        {
            return  "Axis value too small";
        }*/
    }
    return "GOOD";
}
QString TableView2D::formatNumber(double num,int prec)
{
    if (metaDataValid)
    {
        return QString::number(num,'f',prec);
    }
    else
    {
        return QString::number(num);
    }
}
bool TableView2D::setData(QString name,ArrayConfigData *x,ArrayConfigData *y)
{
    m_xAxis = x;
    m_yAxis = y;
    updateTable();
    update();
    return true;
}
void TableView2D::setValue(int row, int column,double value)
{
    //QLOG_DEBUG() << "Set value:" << row << column << value;
    if (row == -1 || column == -1)
    {
        //QLOG_DEBUG() << "Negative array index! Should be unreachable code! FIXME!";
        return;
    }
    if (row >= ui.tableWidget->rowCount() || column >= ui.tableWidget->columnCount())
    {
        //QLOG_DEBUG() << "Larger than life, should be unreachable code! FIXME!";
        return;
    }
    //bool conversionOk = false;
    //double tempValue=ui.tableWidget->item(row,column)->text().toDouble(&conversionOk);
    double tempValue=value;
    double oldValue = tempValue;
    /*if (!conversionOk)
    {
        QMessageBox::information(0,"Error","Value entered is not a number!");
        setSilentValue(row,column,QString::number(currentvalue,'f',2));
        //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
        return;
    }*/
    //QLOG_DEBUG() << "New Value:" << tempValue;

    //This is to make sure we round the value properly. So value displayed == value sent.
    //New value has been accepted. Let's write it.
    //unsigned short newval = ui.tableWidget->item(row,column)->text().toInt();
    //currentvalue = newval;
    /*
    if (row == 0)
    {
        setSilentValue(row,column,QString::number(tempValue,'f',2));
        //tempValue = ui.tableWidget->item(row,column)->text().toDouble(&conversionOk);
        if (tempValue > tableData->maxCalcedXAxis())
        {
            QMessageBox::information(0,"Error",QString("Value entered too large! Value range " + QString::number(tableData->minCalcedXAxis()) + "-" + QString::number(tableData->maxCalcedXAxis()) + ". Entered value:") + ui.tableWidget->item(row,column)->text());
            setSilentValue(row,column,QString::number(currentvalue,'f',2));
            //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
            return;
        }
        else if (tempValue < tableData->minCalcedXAxis())
        {
            QMessageBox::information(0,"Error",QString("Value entered too small! Value range " + QString::number(tableData->minCalcedXAxis()) + "-" + QString::number(tableData->maxCalcedXAxis()) + ". Entered value:") + ui.tableWidget->item(row,column)->text());
            setSilentValue(row,column,QString::number(currentvalue,'f'2));
            //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
            return;
        }

        currentvalue = oldValue;
        //tableData->setCell(0,column,currentvalue);
    }
    else if (row == 1)
    {
        setSilentValue(row,column,QString::number(tempValue,'f',2));
        if (tempValue > tableData->maxCalcedYAxis())
        {
            QMessageBox::information(0,"Error",QString("Value entered too large! Value range " + QString::number(tableData->minCalcedYAxis()) + "-" + QString::number(tableData->maxCalcedYAxis()) + ". Entered value:") + ui.tableWidget->item(row,column)->text());
            setSilentValue(row,column,QString::number(currentvalue,'f',2));
            //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
            return;
        }
        else if (tempValue < tableData->minCalcedYAxis())
        {
            QMessageBox::information(0,"Error",QString("Value entered too small! Value range " + QString::number(tableData->minCalcedYAxis()) + "-" + QString::number(tableData->maxCalcedYAxis()) + ". Entered value:") + ui.tableWidget->item(row,column)->text());
            setSilentValue(row,column,QString::number(currentvalue,'f',2));
            //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
            return;
        }
        //tableData->setXAxis(column,currentvalue);
        currentvalue = oldValue;
        //samples.replace(column,QPointF(samples.at(column).x(),ui.tableWidget->item(row,column)->text().toInt()));
    }
    //New value has been accepted. Let's write it.
    tableData->setCell(row,column,oldValue); //This will emit saveSingleData
    updateTable();
    //ui.tableWidget->resizeColumnsToContents();
    resizeColumnWidths();*/
}

void TableView2D::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_C && event->modifiers() & Qt::ControlModifier)
    {
        //Copy
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            return;
        }
        QByteArray itembytes;
        //int row = ui.tableWidget->selectedItems()[0]->row();
        QMap<int,QMap<int,QString> > tablemap;
        int maxrow = 0;
        int maxcolumn = 0;
        int minrow = 255;
        int mincolumn = 255;
        foreach(QTableWidgetItem* item,ui.tableWidget->selectedItems())
        {
            tablemap[item->row()][item->column()] = item->text();
            if (item->row() > maxrow) maxrow = item->row();
            if (item->column() > maxcolumn) maxcolumn = item->column();
            if (item->row() < minrow) minrow = item->row();
            if (item->column() < mincolumn) mincolumn = item->column();
        }
        for (int i=minrow;i<=maxrow;i++)
        {
            for (int j=mincolumn;j<=maxcolumn;j++)
            {
                if (tablemap.contains(i))
                {
                    if (tablemap[i].contains(j))
                    {
                       // itembytes.append(tablemap[i][j]);
                        itembytes.append("\t");
                    }
                    else
                    {
                        itembytes.append("\t");
                    }
                }
                else
                {
                    //itembytes.append("\n");
                }
            }
            itembytes = itembytes.mid(0,itembytes.length()-1);
            itembytes.append("\n");
        }

        /*QMimeData * mime = new QMimeData();
        mime->setData("text/plain",itembytes);
        QApplication::clipboard()->setMimeData(mime);*/
        return;
    }
    else if(event->key() == Qt::Key_V && event->modifiers() & Qt::ControlModifier)
    {
        //Paste
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            //Can't paste when we don't know where to paste!
            return;
        }
        if (ui.tableWidget->selectedItems().size() != 1)
        {
            QMessageBox::information(0,"Error","Pasting to a selection group is not supported yet. Please select the top left cell you wish to paste from");
            return;
        }
        int rowindex = ui.tableWidget->selectedItems()[0]->row();
        int columnindex = ui.tableWidget->selectedItems()[0]->column();
        int newrow = 0;
        int newcolumn = 0;
        QByteArray data = QApplication::clipboard()->mimeData()->data("text/plain");
        QString datastring(data);
        QStringList datastringsplit = datastring.split("\n");

        //Check to make sure we're in-bounds first
        if (datastringsplit.size() + rowindex > ui.tableWidget->rowCount()+1)
        {
            QMessageBox::information(0,"Error","Attempted to paste a block that does not fit!");
            return;
        }
        foreach(QString line,datastringsplit)
        {
            if (line.split("\t").size() + columnindex > ui.tableWidget->columnCount()+1)
            {
                QMessageBox::information(0,"Error","Attempted to paste a block that does not fit!");
                return;
            }
        }
        ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
        ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
        foreach(QString line,datastringsplit)
        {
            QStringList linesplit = line.split("\t");
            foreach (QString item,linesplit)
            {
                if (item != "")
                {
                    ui.tableWidget->item(rowindex+newrow,columnindex+newcolumn)->setText(item);
                }
                newcolumn++;
            }
            newcolumn=0;
            newrow++;
        }
        writeTable(true);
        connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
        connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
        return;
    }
    QWidget::keyPressEvent(event);
}
void TableView2D::writeTable(bool ram)
{
    if (ram)
    {
        /*tableData->setWritesEnabled(false);
        for (int j=0;j<ui.tableWidget->columnCount();j++)
        {
            tableData->setCell(0,j,ui.tableWidget->item(0,j)->text().toDouble());
            tableData->setCell(1,j,ui.tableWidget->item(1,j)->text().toDouble());
        }
        tableData->writeWholeLocation(ram);
        tableData->setWritesEnabled(true);*/
    }
}
void TableView2D::exportJson(QString filename)
{
    //Create a JSON file similar to MTX's yaml format.
/*
    QJsonObject topmap;
    QJsonObject x;
    QJsonObject y;
    QJsonObject z;
    QJsonArray xlist;
    QJsonArray ylist;
    QJsonArray zlist;

    topmap["2DTable"] = QString("");
    topmap["title"] = m_metaData.tableTitle;
    topmap["description"] = m_metaData.tableTitle;
    x["unit"] = m_metaData.xAxisTitle;
    x["label"] = m_metaData.xAxisTitle;
    y["unit"] = m_metaData.yAxisTitle;
    y["label"] = m_metaData.yAxisTitle;

    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        //Reformat the number to be XXXX.XX to make Fred happy.
        double val = ui.tableWidget->item(0,i)->text().toDouble();
        xlist.append(QString::number(val,'f',2));
        double val2 = ui.tableWidget->item(1,i)->text().toDouble();
        ylist.append(QString::number(val2,'f',2));
    }
    y["values"] = ylist;
    x["values"] = xlist;
    topmap["X"] = x;
    topmap["Y"] = y;

    QJsonDocument doc = QJsonDocument::fromVariant(topmap);
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QLOG_ERROR() << "Unable to open file to output JSON!";
        return;
    }
    file.write(doc.toJson());
    file.close();*/
}

void TableView2D::exportClicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save Json File",".","Json Files (*.json)");
    if (filename == "")
    {
        return;
    }
    if (!filename.toLower().endsWith(".json"))
    {
        filename = filename + ".json";
    }
    exportJson(filename);
}
void TableView2D::importClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Load Json File",".","Json Files (*.json)");
    if (filename == "")
    {
        return;
    }
    //Create a JSON file similar to MTX's yaml format.
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        //QLOG_DEBUG() << "Unable to open file to output JSON!";
        QMessageBox::information(0,"Error","Unable to open JSON file:" + file.errorString());
        return;
    }
    QByteArray toparsebytes = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(toparsebytes);

    if (doc.isEmpty())
    {
        //QLOG_ERROR() << "Unable to parse import json";
        QMessageBox::information(0,"Error","Unable to parse JSON.");
        return;
    }

    QJsonObject topmap = doc.object();
    QJsonObject x = topmap["X"].toObject();
    QJsonObject y = topmap["Y"].toObject();
    QJsonArray xlist = x["values"].toArray();
    QJsonArray ylist = y["values"].toArray();
    QString type = topmap["type"].toString();
    QString title = topmap["title"].toString();
    QString description = topmap["description"].toString();


    if (xlist.size() != ui.tableWidget->columnCount())
    {
        //Error here, wrong number of columns
        QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(xlist.size()) + " columns of axis data, but table has " + QString::number(ui.tableWidget->columnCount()) + " columns of axis data");
        return;
    }

    if (ylist.size() != ui.tableWidget->columnCount())
    {
        //Error here, wrong number of data!
        QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(ylist.size()) + " columns of data, but table has " + QString::number(ui.tableWidget->columnCount()) + " columns of data");
        return;
    }

    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        setSilentValue(0,i,xlist[i].toString());
        setSilentValue(1,i,ylist[i].toString());
    }
    writeTable(true);
}

void TableView2D::tableCurrentCellChanged(int currentrow,int currentcolumn,int prevrow,int prevcolumn)
{
    Q_UNUSED(prevrow)
    Q_UNUSED(prevcolumn)
    if (currentrow == -1 || currentcolumn == -1)
    {
        return;
    }
    m_currRow = currentrow;
    m_currCol = currentcolumn;
    currentvalue = ui.tableWidget->item(currentrow,currentcolumn)->text().toDouble();
}
void TableView2D::resizeColumnWidths()
{
    unsigned int max = 0;
    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        for (int j=0;j<ui.tableWidget->rowCount();j++)
        {
            if (ui.tableWidget->item(j,i))
            {
                //1.3 is required to make text show correctly
                unsigned int test = ui.tableWidget->fontMetrics().horizontalAdvance(ui.tableWidget->item(j,i)->text()) * 1.3;
                if (test > max)
                {
                    max = test;
                }
            }
        }
    }
    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        ui.tableWidget->setColumnWidth(i,max);
    }
}
void TableView2D::loadRamClicked()
{
    if (QMessageBox::information(0,"Warning","Doing this will reload the table from flash, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        //QLOG_DEBUG() << "Ok";
        //emit reloadTableData(m_locationid,true);
        //tableData->updateFromRam();
    }
    else
    {
        //QLOG_DEBUG() << "Not ok";
    }
}

void TableView2D::loadFlashClicked()
{
    if (QMessageBox::information(0,"Warning","Doing this will reload the table from flash, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        //QLOG_DEBUG() << "Ok";
        //emit reloadTableData(m_locationid,false);
        //tableData->updateFromFlash();
    }
    else
    {
        //QLOG_DEBUG() << "Not ok";
    }
}

void TableView2D::tableCellChanged(int row,int column)
{
   // QLOG_DEBUG() << "Cell changed";
    bool conversionOk = false;
    double tempValue=ui.tableWidget->item(row,column)->text().toDouble(&conversionOk);
    if (!conversionOk)
    {
        QMessageBox::information(0,"Error","Value entered is not a number!");
        setSilentValue(row,column,QString::number(currentvalue,'f',2));
        //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
        return;
    }
    setValue(row,column,tempValue);
}
void TableView2D::resizeEvent(QResizeEvent *evt)
{
    Q_UNUSED(evt)
    /*for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        ui.tableWidget->setColumnWidth(i,(ui.tableWidget->width() / ui.tableWidget->columnCount())-1);
    }*/
    resizeColumnWidths();
}
void TableView2D::setSilentValue(int row,int column,QString value)
{
    ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
    ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
    ui.tableWidget->item(row,column)->setText(value);
    connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
    connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
}
/*
bool TableView2D::setData(unsigned short locationid,DataBlock *data)
{
    if (!metaDataValid)
    {
        m_metaData = Table2DMetaData();
    }
    if (tableData == 0)
    {
        tableData = dynamic_cast<Table2DData*>(data);
        connect(tableData,SIGNAL(update()),this,SLOT(updateTable()));
    }

    //tableData = new Table2DData(locationid,m_isFlashOnly,data,m_metaData);
    //tableData = new FETable2DData();
    //tableData = (Table2DData*)newtableData;
    //tableData->setData(locationid,m_isFlashOnly,data,m_metaData,m_isSignedData);
    //connect(tableData,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
    QLOG_DEBUG() << "TableView2D::passData" << "0x" + QString::number(locationid,16).toUpper();
    m_locationid = locationid;
    return updateTable();
}
bool TableView2D::setData(QString name,DataBlock *data)
{
    if (!metaDataValid)
    {
        m_metaData = Table2DMetaData();
    }
    if (tableData == 0)
    {
        tableData = dynamic_cast<Table2DData*>(data);
        connect(tableData,SIGNAL(update()),this,SLOT(updateTable()));
    }
    if (tableData == 0)
    {
        return false;
    }
    QLOG_DEBUG() << "TableView2D::passData" << name;
    return updateTable();
}*/
bool TableView2D::updateTable()
{
    ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
    QList<QPair<int,int> > selectedlist;
    if (ui.tableWidget->selectedItems().size() > 0)
    {
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            selectedlist.append(QPair<int,int>(ui.tableWidget->selectedItems()[i]->row(),ui.tableWidget->selectedItems()[i]->column()));
        }
    }
    ui.tableWidget->clear();
    ui.tableWidget->setColumnCount(0);
    ui.tableWidget->setRowCount(2);
    for (int x=0;x<m_xAxis->size();x++)
    {
        ui.tableWidget->setColumnCount(ui.tableWidget->columnCount()+1);
        ui.tableWidget->setItem(0,ui.tableWidget->columnCount()-1,new QTableWidgetItem(QString::number(m_xAxis->getValue(x).toFloat(),'f',2)));
        ui.tableWidget->setItem(1,ui.tableWidget->columnCount()-1,new QTableWidgetItem(QString::number(m_yAxis->getValue(x).toFloat(),'f',2)));
        //ui.tableWidget->setItem(1,ui.tableWidget->columnCount()-1,new QTableWidgetItem(QString::number(tableData->values()[i],'f',m_metaData.yDp)));
/*		if (tableData->maxActualYAxis() == tableData->minActualYAxis())
        {
            ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,255,(255/2.0),255));
        }
        else
        {
            if ((tableData->values()[i] - tableData->minActualYAxis()) < (tableData->maxActualYAxis() - tableData->minActualYAxis())/4)
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,(255*(((tableData->values()[i] - tableData->minActualYAxis()))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),255));
            }
            else if ((tableData->values()[i] - tableData->minActualYAxis()) < (((tableData->maxActualYAxis()-tableData->minActualYAxis())/4)*2))
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,255,255-(255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)))));
            }
            else if ((tableData->values()[i] - tableData->minActualYAxis()) < (((tableData->maxActualYAxis() - tableData->minActualYAxis())/4)*3))
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb((255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)*2))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),255,0));
            }
            else
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(255,255-(255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)*3))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),0));
            }
        }
        keys.append(tableData->axis()[i]);
        values.append(tableData->values()[i]);*/
    }

    /*if (tableData->axis().size() == 0)
    {
        //Invalid/empty data
        QLOG_ERROR() << "2D Table axis had zero values. This is INVALID and should be fixed.";
        return false;
    }
    double first = tableData->axis()[0];
    int order = 0;
    QVector<double> keys;
    QVector<double> values;
    for (int i=0;i<tableData->columns();i++)
    {
        if (i == 1)
        {
            if (tableData->axis()[i] < first)
            {
                order = 1;
            }
            else
            {
                order = 2;
            }
        }
        if (order == 1)
        {
            if (tableData->axis()[i] > first)
            {
                //Out of order table axis.
                QLOG_ERROR() << "2D Table axis is out of order!";
                //return false;
            }
        }
        else if (order == 2)
        {
            if (tableData->axis()[i] < first)
            {
                //Out of order table axis.
                QLOG_ERROR() << "2D Table axis is out of order!";
                //return false;
            }
        }
        first = tableData->axis()[i];

        ui.tableWidget->setColumnCount(ui.tableWidget->columnCount()+1);
        ui.tableWidget->setItem(0,ui.tableWidget->columnCount()-1,new QTableWidgetItem(QString::number(tableData->axis()[i],'f',m_metaData.xDp)));
        ui.tableWidget->setItem(1,ui.tableWidget->columnCount()-1,new QTableWidgetItem(QString::number(tableData->values()[i],'f',m_metaData.yDp)));
        if (tableData->maxActualYAxis() == tableData->minActualYAxis())
        {
            ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,255,(255/2.0),255));
        }
        else
        {
            if ((tableData->values()[i] - tableData->minActualYAxis()) < (tableData->maxActualYAxis() - tableData->minActualYAxis())/4)
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,(255*(((tableData->values()[i] - tableData->minActualYAxis()))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),255));
            }
            else if ((tableData->values()[i] - tableData->minActualYAxis()) < (((tableData->maxActualYAxis()-tableData->minActualYAxis())/4)*2))
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(0,255,255-(255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)))));
            }
            else if ((tableData->values()[i] - tableData->minActualYAxis()) < (((tableData->maxActualYAxis() - tableData->minActualYAxis())/4)*3))
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb((255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)*2))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),255,0));
            }
            else
            {
                ui.tableWidget->item(1,ui.tableWidget->columnCount()-1)->setBackgroundColor(QColor::fromRgb(255,255-(255*(((tableData->values()[i] - tableData->minActualYAxis())-(((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0)*3))/((tableData->maxActualYAxis() - tableData->minActualYAxis())/4.0))),0));
            }
        }
        keys.append(tableData->axis()[i]);
        values.append(tableData->values()[i]);
    }*/
    /*QCPGraph *graph = ui.plot->graph(0);
    graph->setData(keys,values);
    graph->rescaleAxes();
    //m_wideAxisRect->axis(QCPAxis::atLeft)->setLabel(tableData->axisLabel());

    //m_wideAxisRect->axis(QCPAxis::atBottom)->setLabel(tableData->valueLabel());
    connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
//	curve->setSamples(samples);
    ui.plot->replot();*/
    ui.tableWidget->setCurrentCell(m_currRow,m_currCol);
        resizeColumnWidths();
    for (int i=0;i<selectedlist.size();i++)
    {
        ui.tableWidget->item(selectedlist[i].first,selectedlist[i].second)->setSelected(true);
    }


    selectedlist.clear();
    //ui.tableWidget->resizeColumnsToContents();
    return true;

}

void TableView2D::reColorTable(int rownum,int colnum)
{
    Q_UNUSED(rownum)
    Q_UNUSED(colnum)
    /*
    //QLOG_DEBUG() << "Recoloring" << rownum << colnum;
    if (rownum == ui.tableWidget->rowCount()-1 || colnum == 0)
    {
        return;
    }
    if (rownum == -1 && colnum == -1)
    {
        //Recolor the whole table
        ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
        ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
        for (int row=0;row<tableData->rows();row++)
        {
            for (int col=0;col<tableData->columns();col++)
            {
                double val = tableData->values()[row][col];
                //ui.tableWidget->setItem((tableData->rows()-1)-(row),col+1,new QTableWidgetItem(formatNumber(val,m_metaData.zDp)));
                if (val < tableData->maxZAxis()/4)
                {
                    ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(0,(255*((val)/(tableData->maxZAxis()/4.0))),255));
                }
                else if (val < ((tableData->maxZAxis()/4)*2))
                {
                    ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(0,255,255-(255*((val-((tableData->maxZAxis()/4.0)))/(tableData->maxZAxis()/4.0)))));
                }
                else if (val < ((tableData->maxZAxis()/4)*3))
                {
                    ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb((255*((val-((tableData->maxZAxis()/4.0)*2))/(tableData->maxZAxis()/4.0))),255,0));
                }
                else
                {
                    ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(255,255-(255*((val-((tableData->maxZAxis()/4.0)*3))/(tableData->maxZAxis()/4.0))),0));
                }
            }
        }
        connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
        connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
    }
    else
    {


        //QLOG_DEBUG() << "Loc:" << (tableData->rows()-1)-(rownum) << colnum - 1;
        ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
        ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
        double val = tableData->values()[(tableData->rows()-1)-(rownum)][colnum-1];
        //QLOG_DEBUG() << "Value:" << val;

        //ui.tableWidget->setItem((tableData->rows()-1)-(row),col+1,new QTableWidgetItem(formatNumber(val,m_metaData.zDp)));
        if (val < tableData->maxZAxis()/4)
        {
            ui.tableWidget->item(rownum,colnum)->setBackgroundColor(QColor::fromRgb(0,(255*((val)/(tableData->maxZAxis()/4.0))),255));
        }
        else if (val < ((tableData->maxZAxis()/4)*2))
        {
            ui.tableWidget->item(rownum,colnum)->setBackgroundColor(QColor::fromRgb(0,255,255-(255*((val-((tableData->maxZAxis()/4.0)))/(tableData->maxZAxis()/4.0)))));
        }
        else if (val < ((tableData->maxZAxis()/4)*3))
        {
            ui.tableWidget->item(rownum,colnum)->setBackgroundColor(QColor::fromRgb((255*((val-((tableData->maxZAxis()/4.0)*2))/(tableData->maxZAxis()/4.0))),255,0));
        }
        else
        {
            ui.tableWidget->item(rownum,colnum)->setBackgroundColor(QColor::fromRgb(255,255-(255*((val-((tableData->maxZAxis()/4.0)*3))/(tableData->maxZAxis()/4.0))),0));
        }
        connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
        connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));

    }
    */
}
void TableView2D::tracingCheckBoxStateChanged(int newstate)
{
    if (newstate == Qt::Checked)
    {
        m_tracingEnabled = true;
    }
    else
    {
        m_tracingEnabled = false;
        ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
        ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
        for (int i=0;i<m_highlightItemList.size();i++)
        {
            ui.tableWidget->item(m_highlightItemList[i].first,m_highlightItemList[i].second)->setForeground(QColor::fromRgb(0,0,0));
            ui.tableWidget->item(m_highlightItemList[i].first,m_highlightItemList[i].second)->setData(Qt::UserRole+1,false);
        }
        m_highlightItemList.clear();
        connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
        connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
    }
}
void TableView2D::passDatalog(QVariantMap data)
{
    if (!m_tracingEnabled)
    {
        return;
    }
    /*if (data.contains(m_metaData.xHighlight))
    {
        double xval = data[m_metaData.xHighlight].toDouble();
        int xloc = 1;
        double lowercolumnratio = 0;
        double uppercolumnratio = 0;
        int lowercolumn = 0;
        int highercolumn = 1;
        for (int x=1;x<ui.tableWidget->columnCount();x++)
        {
            double testval = ui.tableWidget->item(0,x)->text().toDouble();
            double prevtestval;
            double nexttestval;
            if (x == 1)
            {
                prevtestval = 0;
            }
            else
            {
                prevtestval = ui.tableWidget->item(0,x-1)->text().toDouble();
            }
            if (x == ui.tableWidget->columnCount()-1)
            {
                nexttestval = testval + 1;
            }
            else
            {
                nexttestval = ui.tableWidget->item(0,x+1)->text().toDouble();
            }
            double lowerlimit = testval - ((testval - prevtestval) / 2.0);
            double upperlimit = testval + ((nexttestval - testval) / 2.0);
            if (xval > lowerlimit && xval < upperlimit)
            {
                xloc = x;
                lowercolumn = x-1;
                highercolumn = x+1;
                lowercolumnratio = (xval - lowerlimit) / (upperlimit - lowerlimit);
                uppercolumnratio = (upperlimit - xval) / (upperlimit - lowerlimit);
                if (xval > testval)
                {
                    lowercolumn = x+1;
                    highercolumn = x+2;
                    lowercolumnratio = (xval - testval) / (nexttestval - testval);
                    uppercolumnratio = (nexttestval - xval) / (nexttestval - testval);
                }
                else
                {
                    lowercolumn = x;
                    highercolumn = x+1;
                    lowercolumnratio = (xval - prevtestval) / (testval - prevtestval);
                    uppercolumnratio = (testval - xval) / (testval - prevtestval);
                }
                //0 500 1000
                //Val is at 750.
                //lowerrowratio should be 50%, and upper should be 50%
                //(xval - lowerlimit) / (upperlimit - lowerlimit) //0-1.0
                //(upperlimit - xval) / (upperlimit - lowerlimit) //0-1.0


                break;
            }
        }

        if (xloc != -1)
        {
            if (xloc == m_oldXLoc)
            {
                //No change, no reason to continue;
                //return;
            }
            ui.tableWidget->disconnect(SIGNAL(cellChanged(int,int)));
            ui.tableWidget->disconnect(SIGNAL(currentCellChanged(int,int,int,int)));
            QList<QPair<int,int> > undonelist;
            for (int i=0;i<m_highlightItemList.size();i++)
            {
                if (!ui.tableWidget->item(m_highlightItemList[i].first,m_highlightItemList[i].second)->isSelected())
                {
                    ui.tableWidget->item(m_highlightItemList[i].first,m_highlightItemList[i].second)->setTextColor(QColor::fromRgb(0,0,0));
                    ui.tableWidget->item(m_highlightItemList[i].first,m_highlightItemList[i].second)->setData(Qt::UserRole+1,false);
                }
                else
                {
                    undonelist.append(QPair<int,int>(m_highlightItemList[i].first,m_highlightItemList[i].second));
                }
            }
            m_highlightItemList.clear();
            m_highlightItemList.append(undonelist);
            m_oldXLoc = xloc;

            if (highercolumn < ui.tableWidget->columnCount())
            {
                if (!ui.tableWidget->item(1,lowercolumn)->isSelected())
                {
                    m_highlightItemList.append(QPair<int,int>(1,lowercolumn));
                    ui.tableWidget->item(1,lowercolumn)->setData(Qt::UserRole+1,true);
                    ui.tableWidget->item(1,lowercolumn)->setData(Qt::UserRole+2,lowercolumnratio);
                }
                if (!ui.tableWidget->item(1,highercolumn)->isSelected())
                {
                    m_highlightItemList.append(QPair<int,int>(1,highercolumn));
                    ui.tableWidget->item(1,highercolumn)->setData(Qt::UserRole+1,true);
                    ui.tableWidget->item(1,highercolumn)->setData(Qt::UserRole+2,uppercolumnratio);
                }
            }
            else
            {
                if (!ui.tableWidget->item(1,lowercolumn)->isSelected())
                {
                    m_highlightItemList.append(QPair<int,int>(1,lowercolumn));
                    ui.tableWidget->item(1,lowercolumn)->setData(Qt::UserRole+1,true);
                    ui.tableWidget->item(1,lowercolumn)->setData(Qt::UserRole+2,lowercolumnratio);
                }
            }

            connect(ui.tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChanged(int,int)));
            connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
        }
        else
        {
            QLOG_ERROR() << "Error parsing datalog, xloc and yloc aren't != -1";
        }
    }*/
}

/*bool TableView2D::setData(unsigned short locationid,QByteArray rawdata)
{
    return setData(locationid,rawdata,(Table2DData*)(tableData));
}
bool TableView2D::setData(unsigned short locationid,QByteArray rawdata,Table2DMetaData metadata,TableData *newtableData)
{
    m_metaData = metadata;
    metaDataValid = true;
    return setData(locationid,rawdata,newtableData);
}*/
/*void TableView2D::setMetaData(Table2DMetaData metadata)
{
    m_metaData = metadata;
    metaDataValid = true;
}*/

TableView2D::~TableView2D()
{
}
void TableView2D::saveClicked()
{
    //emit saveToFlash(m_locationid);
   // tableData->saveRamToFlash();
}
