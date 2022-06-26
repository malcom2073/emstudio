/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include "tableview3d.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
//#include "QsLog.h"

//#include <freeems/fetable3ddata.h>
//#include "tablewidgetdelegate.h"
TableView3D::TableView3D(QWidget *parent)
{
    Q_UNUSED(parent)
    m_isFlashOnly = false;
    //m_tableMap=0;
    ui.setupUi(this);
    tableData=0;
    m_tracingEnabled = false;
    connect(ui.savePushButton,SIGNAL(clicked()),this,SLOT(saveClicked()));
    connect(ui.loadFlashPushButton,SIGNAL(clicked()),this,SLOT(loadClicked()));
    connect(ui.loadRamPushButton,SIGNAL(clicked()),this,SLOT(loadRamClicked()));
    connect(ui.exportPushButton,SIGNAL(clicked()),this,SLOT(exportClicked()));
    connect(ui.importPushButton,SIGNAL(clicked()),this,SLOT(importClicked()));
    connect(ui.tableWidget,SIGNAL(hotKeyPressed(int,Qt::KeyboardModifier)),this,SLOT(hotKeyPressed(int,Qt::KeyboardModifier)));
    connect(ui.tracingCheckBox,SIGNAL(stateChanged(int)),this,SLOT(tracingCheckBoxStateChanged(int)));
    // TOOD: Add hotkeys again, Qt6 changed this.
    //ui.tableWidget->addHotkey(Qt::Key_Plus,Qt::ShiftModifier);
    //ui.tableWidget->addHotkey(Qt::Key_Minus,Qt::NoModifier);
    //ui.tableWidget->addHotkey(Qt::Key_Underscore,Qt::ShiftModifier);
    //ui.tableWidget->addHotkey(Qt::Key_Equal,Qt::NoModifier);
    connect(ui.tableWidget,SIGNAL(itemChangeRequest(int,int,int,int,QString)),this,SLOT(itemChangeRequest(int,int,int,int,QString)));
    //ui.tableWidget->setItemDelegate(new TableWidgetDelegate());

    setContextMenuPolicy(Qt::DefaultContextMenu);
    //QAction* fooAction = new QAction("foo",this);
    //QAction* barAction = new QAction("bar",this);
    //connect(fooAction, SIGNAL(triggered()), this, SLOT(doSomethingFoo()));
    //connect(barAction, SIGNAL(triggered()), this, SLOT(doSomethingBar()));
    //addAction(fooAction);
    //addAction(barAction);
    metaDataValid = true;
    /*if (!isram)
    {
        //Is only flash
        ui.loadRamPushButton->setVisible(false);
    }
    else if (!isflash)
    {
        //Is only ram
        ui.savePushButton->setVisible(false);
        ui.loadFlashPushButton->setVisible(false);
    }
    else
    {
        //Is both ram and flash
    }*/
    //ui.importPushButton->setVisible(false);
    //ui.tracingCheckBox->setVisible(false);
    //ui.tracingCheckBox->setEnabled(false);
    connect(ui.showMapPushButton,SIGNAL(clicked()),this,SLOT(showMapClicked()));
    m_inputTimer = new QTimer(this);
    connect(m_inputTimer,SIGNAL(timeout()),this,SLOT(inputTimerTimeout()));
}
void TableView3D::inputTimerTimeout()
{
    if (!m_firstKeypress)
    {
        setRange(m_queuedValList);
    }
    m_inputTimer->stop();
}

void TableView3D::tracingCheckBoxStateChanged(int newstate)
{
    if (newstate == Qt::Checked)
    {
        m_tracingEnabled = true;
        //ui.tableWidget->setTracingEnabled(true);
    }
    else
    {
        m_tracingEnabled = false;
        //ui.tableWidget->setTracingEnabled(false);
    }
}

void TableView3D::setValue(int row, int column,double value,bool ignoreselection)
{
    if (row >= ui.tableWidget->rowCount() || column >= ui.tableWidget->columnCount())
    {
        //QLOG_ERROR() << "Larger than life, should be unreachable code! FIXME!";
        return;
    }
    // Ignore bottom right corner if the disallow on editing fails
    if (row == -1 && column == -1)
    {
       // QLOG_ERROR() << "This should not happen! Bottom right corner ignored!";
        return;
    }
    //bool conversionOk = false; // Note, value of this is irrelevant, overwritten during call in either case, but throws a compiler error if not set.
    //double tempValue = ui.tableWidget->item(row,column)->text().toDouble(&conversionOk);
    double tempValue = value;
    double oldValue = tempValue;
    /*if (!conversionOk)
    {
        QMessageBox::information(0,"Error","Value entered is not a number!");
        setSilentValue(row,column,formatNumber(currentvalue));
        //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
        return;
    }*/

    //ui.tableWidget->item(row,column)->setText(QString::number(tempValue,'f',2));
    //tempValue = ui.tableWidget->item(row,column)->text().toDouble(&conversionOk);

    //New value has been accepted. Let's write it.

    //If the value is an axis, and there are multiple cells selected, cancel the edit!
    if (ui.tableWidget->selectedItems().size() > 1 && !ignoreselection)
    {
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            if (ui.tableWidget->selectedItems()[i].y() == ui.tableWidget->rowCount()-1)
            {
                //QLOG_ERROR() << "XAxis edit attempted with multiple cells selected. This is not allowed";
                QMessageBox::information(0,"Error","Editing of multiple AXIS cells at once is not allowed. Please select a single axis cell to edit at a time");
                //setSilentValue(row,column,m_currentValue);
                return;
            }
            if (ui.tableWidget->selectedItems()[i].x() == 0)
            {
                //QLOG_ERROR() << "YAxis edit attempted with multiple cells selected. This is not allowed";
                QMessageBox::information(0,"Error","Editing of multiple AXIS cells at once is not allowed. Please select a single axis cell to edit at a time");
                //setSilentValue(row,column,m_currentValue);
                return;
            }
        }
        setSilentValue(row,column,m_currentValue); //Reset the value, setRange will set it properly.
        //currentvalue = oldValue;
       // QLOG_DEBUG() << "Setting all cells to" << m_currentValue << "for" << row << column;
        QList<QPair<QPair<int,int>,double> > vallist;
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x());
            val.second = tempValue;
            vallist.append(val);
        }
        setRange(vallist);
    }
    else
    {
        if (row == -1)
        {

            if (tempValue > tableData->maxCalcedXAxis())
            {
                QMessageBox::information(0,"Error",QString("Value entered too large! Value range " + QString::number(tableData->minCalcedXAxis()) + "-" + QString::number(tableData->maxCalcedXAxis()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //setSilentValue(row,column,m_currentValue);
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                return;
            }
            else if (tempValue < tableData->minCalcedXAxis())
            {
                QMessageBox::information(0,"Error",QString("Value entered too small! Value range " + QString::number(tableData->minCalcedXAxis()) + "-" + QString::number(tableData->maxCalcedXAxis()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //setSilentValue(row,column,m_currentValue);
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                return;
            }
            //setSilentValue(row,column,formatNumber(tempValue,m_metaData.xDp));
            tableData->setXAxis(column,oldValue);
        }
        else if (column == -1)
        {
            //setSilentValue(row,column,formatNumber(tempValue,m_metaData.yDp));
            if (tempValue > tableData->maxCalcedYAxis())
            {
                QMessageBox::information(0,"Error",QString("Value entered too large! Value range " + QString::number(tableData->minCalcedYAxis()) + "-" + QString::number(tableData->maxCalcedYAxis()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                //setSilentValue(row,column,m_currentValue);
                return;
            }
            else if (tempValue < tableData->minCalcedYAxis())
            {
                QMessageBox::information(0,"Error",QString("Value entered too small! Value range " + QString::number(tableData->minCalcedYAxis()) + "-" + QString::number(tableData->maxCalcedYAxis()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                //setSilentValue(row,column,m_currentValue);
                return;
            }
            tableData->setYAxis(ui.tableWidget->rowCount()-(row+1),oldValue);
        }
        else
        {
            //setSilentValue(row,column,formatNumber(tempValue,m_metaData.zDp));
            if (tempValue > tableData->maxCalcedValue())
            {
                QMessageBox::information(0,"Error",QString("Value entered too large! Value range " + QString::number(tableData->minCalcedValue()) + "-" + QString::number(tableData->maxCalcedValue()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                //setSilentValue(row,column,m_currentValue);
                return;
            }
            if (tempValue < tableData->minCalcedValue())
            {
                QMessageBox::information(0,"Error",QString("Value entered too small! Value range " + QString::number(tableData->minCalcedValue()) + "-" + QString::number(tableData->maxCalcedValue()) + ". Entered value:") + ui.tableWidget->item(row,column));
                //ui.tableWidget->item(row,column)->setText(QString::number(currentvalue));
                //setSilentValue(row,column,m_currentValue);
                return;
            }
            tableData->setCell(ui.tableWidget->rowCount()-(row+1),column,oldValue);
        }
    }
}

void TableView3D::hotKeyPressed(int key,Qt::KeyboardModifier modifier)
{
    if (key == Qt::Key_Plus || key == Qt::Key_Equal)
    {
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            return;
        }
        QList<QPair<QPair<int,int>,double> > vallist;
        QList<QString> invalidreasons;
        bool valid = true;
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {

            QString text = ui.tableWidget->item(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x());
            double textd = text.toDouble();
            double maxval = 0;
            maxval = tableData->maxCalcedValue();
            if (modifier & Qt::ShiftModifier)
            {
                textd += maxval / 10;
            }
            else
            {
                textd += maxval / 100;
            }

            if (textd > maxval)
            {
                textd = maxval;
            }
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x());
            val.second = textd;
            vallist.append(val);
            //else if (column == 0) Y
            //else Z
            QString formattednumber = "";
            if (ui.tableWidget->selectedItems()[i].x() == -1)
            {
                //Y axis
                formattednumber = formatNumber(textd,m_metaData.yDp);
            }
            else if (ui.tableWidget->selectedItems()[i].y() == -1)
            {
                //X Axis
                formattednumber = formatNumber(textd,m_metaData.xDp);
            }
            else
            {
                //Value
                formattednumber = formatNumber(textd,m_metaData.zDp);
            }
            QString verifystr = verifyValue(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x(),formattednumber);
            if (verifystr != "GOOD")
            {
                invalidreasons.append(verifystr);
                valid = false;
            }
            else
            {
                ui.tableWidget->setItem(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x(),formattednumber);
            }

            //ui.tableWidget->selectedItems()[0]->setText(formatNumber(textd,m_metaData.zDp));
            //setValue(ui.tableWidget->selectedItems()[i]->row(),ui.tableWidget->selectedItems()[i]->column(),textd);

        }
        if (!valid)
        {
            //We had an error.
            m_inputTimer->stop();
            QMessageBox::information(0,"Error setting value: ",invalidreasons.at(0));
            setRange(m_queuedValList);
        }
        if (m_inputTimer->isActive())
        {
            //Timer is already active, too soon!
            //Let the table continue to increment... but don't fire off
            m_queuedValList = vallist;
            m_firstKeypress = false;
            //m_inputTimer->stop();
            //m_inputTimer->start(250);
        }
        else
        {
            m_firstKeypress = true;
            m_inputTimer->start(350);
            setRange(vallist);
        }
    }
    else if (key == Qt::Key_Minus || key == Qt::Key_Underscore)
    {
        if (ui.tableWidget->selectedItems().size() == 0)
        {
            return;
        }
        QList<QPair<QPair<int,int>,double> > vallist;
        QList<QString> invalidreasons;
        bool valid = true;
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
            QString text = "";
            text = ui.tableWidget->item(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x());

            double textd = text.toDouble();
            double minval = 0;
            double maxval = 0;
            maxval = tableData->maxCalcedValue();
            minval = tableData->minCalcedValue();
            if (modifier & Qt::ShiftModifier)
            {
                textd -= maxval / 10;
            }
            else
            {
                textd -= maxval / 100;
            }
            if (textd < minval)
            {
                textd = minval;
            }
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x());
            val.second = textd;
            vallist.append(val);
            QString formattednumber = "";

            if (ui.tableWidget->selectedItems()[i].x() == -1)
            {
                //Y axis
                formattednumber = formatNumber(textd,m_metaData.yDp);
            }
            else if (ui.tableWidget->selectedItems()[i].y() == -1)
            {
                //X Axis
                formattednumber = formatNumber(textd,m_metaData.xDp);
            }
            else
            {
                //Value
                formattednumber = formatNumber(textd,m_metaData.zDp);
            }
            QString verifystr = verifyValue(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x(),formattednumber);
            if (verifystr != "GOOD")
            {
                invalidreasons.append(verifystr);
                valid = false;
            }
            else
            {
                ui.tableWidget->setItem(ui.tableWidget->selectedItems()[i].y(),ui.tableWidget->selectedItems()[i].x(),formattednumber);
            }


            //ui.tableWidget->selectedItems()[0]->setText(formatNumber(textd,m_metaData.zDp));
            //setValue(ui.tableWidget->selectedItems()[0]->row(),ui.tableWidget->selectedItems()[0]->column(),textd);
        }
        if (!valid)
        {
            //We had an error.
            m_inputTimer->stop();
            QMessageBox::information(0,"Error setting value: ",invalidreasons.at(0));
            setRange(m_queuedValList);
        }
        if (m_inputTimer->isActive())
        {
            //Timer is already active, too soon!
            //Let the table continue to increment... but don't fire off
            m_queuedValList = vallist;
            m_firstKeypress = false;
            //m_inputTimer->stop();
            //m_inputTimer->start(250);
        }
        else
        {
            m_firstKeypress = true;
            m_inputTimer->start(250);
            setRange(vallist);
        }
    }
}
void TableView3D::regularKeyPressed(int key)
{

}

void TableView3D::keyPressEvent(QKeyEvent *event)
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
        /*foreach(QTableWidgetItem* item,ui.tableWidget->selectedItems())
        {
            tablemap[item->row()][item->column()] = item->text();
            if (item->row() > maxrow) maxrow = item->row();
            if (item->column() > maxcolumn) maxcolumn = item->column();
            if (item->row() < minrow) minrow = item->row();
            if (item->column() < mincolumn) mincolumn = item->column();
        }*/
        for (int i=minrow;i<=maxrow;i++)
        {
            for (int j=mincolumn;j<=maxcolumn;j++)
            {
                if (tablemap.contains(i))
                {
                    if (tablemap[i].contains(j))
                    {
                        //itembytes.append(tablemap[i][j]);
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

        //QMimeData * mime = new QMimeData();
        //mime->setData("text/plain",itembytes);
        //QApplication::clipboard()->setMimeData(mime);
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
        int rowindex = ui.tableWidget->selectedItems()[0].y();
        int columnindex = ui.tableWidget->selectedItems()[0].x();
        int newrow = 0;
        int newcolumn = 0;
        //QByteArray data = QApplication::clipboard()->mimeData()->data("text/plain");
        QByteArray data;
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

        //Disable signals, so we can write the table all at once
        //bool valid = true;
        QMap<int,QMap<int,QString> > tmpvaluemap;
        QStringList invalidreasons;
        QList<QPair<QPair<int,int>,double> > valuelist;
        //setRange
        foreach(QString line,datastringsplit)
        {
            QStringList linesplit = line.split("\t");
            foreach (QString item,linesplit)
            {
                if (item != "")
                {
                    if (!tmpvaluemap.contains(rowindex+newrow))
                    {
                        tmpvaluemap[rowindex+newrow] = QMap<int,QString>();
                    }
                    //newvallist.append(item.toDouble());
                    QPair<QPair<int,int>,double> val;
                    val.first = QPair<int,int>(rowindex+newrow,columnindex+newcolumn);
                    val.second = item.toDouble();
                    valuelist.append(val);

                    //tmpvaluemap[rowindex+newrow][columnindex+newcolumn] = ui.tableWidget->item(rowindex+newrow,columnindex+newcolumn)->text();
                    QString verifystr = verifyValue(rowindex+newrow,columnindex+newcolumn,item);
                    if (verifystr != "GOOD")
                    {
                        invalidreasons.append(verifystr);
                        //valid = false;
                    }
                    //ui.tableWidget->item(rowindex+newrow,columnindex+newcolumn)->setText(item);
                }
                newcolumn++;
            }
            newcolumn=0;
            newrow++;
        }
        setRange(valuelist);
        /*
        //Write the values, and re-enable the signals.
        if (valid)
        {
            writeTable(true);
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
        }*/
    }
}
QString TableView3D::verifyValue(int row,int column,QString item)
{
    if (row == -11 && column == -1)
    {
        //QLOG_ERROR() << "This should not happen! Bottom right corner ignored!";
        return "INVALID INDEX";
    }
    double tempValue = item.toDouble();

    if (row == -1)
    {
        if (tempValue > tableData->maxCalcedXAxis())
        {
            return "Axis value entered too large!";
        }
        else if (tempValue < tableData->minCalcedXAxis())
        {
            return "Axis value entered too small";
        }
    }
    else if (column == -1)
    {
        if (tempValue > tableData->maxCalcedYAxis())
        {
            return "Axis value too large";
        }
        else if (tempValue < tableData->minCalcedYAxis())
        {
            return  "Axis value too small";
        }
    }
    else
    {
        if (tempValue > tableData->maxCalcedValue())
        {
            return "Value too large for table";
        }
        if (tempValue < tableData->minCalcedValue())
        {
            return "Value too small for table";
        }
    }
    return "GOOD";
}
//Data is arranged as a
void TableView3D::setRange(QList<QPair<QPair<int,int>,double> > data)
{
    QMap<int,QMap<int,QString> > tmpvaluemap;
    bool valid = true;
    QList<QString> invalidreasons;
    for (int i=0;i<data.size();i++)
    {
        if (!tmpvaluemap.contains(data[i].first.first))
        {
            tmpvaluemap[data[i].first.first] = QMap<int,QString>();
        }
        tmpvaluemap[data[i].first.first][data[i].first.second] = ui.tableWidget->item(data[i].first.first,data[i].first.second);
        QString formatstr = formatNumber(data[i].second,m_metaData.zDp);
        QString verifystr = verifyValue(data[i].first.first,data[i].first.second,formatstr);
        if (verifystr != "GOOD")
        {
            invalidreasons.append(verifystr);
            valid = false;
        }
        ui.tableWidget->setItem(data[i].first.first,data[i].first.second,formatstr);
    }
    //Write the values, and re-enable the signals.
    if (valid)
    {
        writeTable(true);
    }
    else
    {
        for (QMap<int,QMap<int,QString> >::const_iterator i=tmpvaluemap.constBegin();i!=tmpvaluemap.constEnd();i++)
        {
            for (QMap<int,QString>::const_iterator j=i.value().constBegin();j!=i.value().constEnd();j++)
            {
                ui.tableWidget->setItem(i.key(),j.key(),j.value());
            }
        }
        QString errorstr = "";
        foreach(QString reason,invalidreasons)
        {
            errorstr += reason + ",";
        }
        QMessageBox::information(0,"Error",errorstr);
    }
}

void TableView3D::writeTable(bool ram)
{
    if (ram)
    {
        tableData->setWritesEnabled(false);
        for (int i=0;i<ui.tableWidget->rowCount();i++)
        {
            tableData->setYAxis((ui.tableWidget->rowCount()-1)-i,ui.tableWidget->item(i,-1).toDouble());
            for (int j=0;j<ui.tableWidget->columnCount();j++)
            {
                if (i == 0)
                {
                    //First one
                    tableData->setXAxis(j,ui.tableWidget->item(-1,j).toDouble());
                }
                tableData->setCell(i,j,ui.tableWidget->item((ui.tableWidget->rowCount()-1)-i,j).toDouble());
            }
        }
        tableData->writeWholeLocation(ram);
        tableData->setWritesEnabled(true);
    }
}

void TableView3D::showMapClicked()
{
    //emit show3DTable(m_locationId,tableData);
    //if (m_tableMap)
    {
//		m_tableMap->show();

    }
    //else
    {
        /*m_tableMap = new TableMap3D();
        m_tableMap->setGeometry(100,100,800,600);
        m_tableMap->show();
        m_tableMap->passData(tableData);
        emit show3DTable(m_locationId,m_tableData);*/
    }
}

void TableView3D::contextMenuEvent(QContextMenuEvent *evt)
{
    QMenu menu(this);
    menu.addAction("Test");
    menu.addAction("Second");
    menu.setGeometry(evt->x(),evt->y(),menu.width(),menu.height());
    menu.show();
}

void TableView3D::tableCurrentCellChanged(int currentrow,int currentcolumn,int prevrow,int prevcolumn)
{
    Q_UNUSED(prevrow)
    Q_UNUSED(prevcolumn)
    //if (currentrow == -1 || currentcolumn == -1 || !ui.tableWidget->item(currentrow,currentcolumn))
    //{
    //	return;
    //}
    m_currRow = currentrow;
    m_currCol = currentcolumn;
    //currentvalue = ui.tableWidget->item(currentrow,currentcolumn)->text().toDouble();
}
void TableView3D::currentSelectionChanged(QList<QPair<int,int> > selectionList)
{
    m_currentValue = "";
    if (selectionList.size() > 0)
    {
        m_currentValue = ui.tableWidget->item(selectionList.at(0).first,selectionList.at(0).second);
    }
}

void TableView3D::exportJson(QString filename)
{
    //Create a JSON file similar to MTX's yaml format.
    QVariantMap topmap;
    QVariantMap x;
    QVariantMap y;
    QVariantMap z;
    QVariantList xlist;
    QVariantList ylist;
    QVariantList zlist;

    topmap["3DTable"] = "";
    topmap["type"] = "3D";
    topmap["title"] = m_metaData.tableTitle;
    topmap["description"] = m_metaData.tableTitle;
    x["unit"] = m_metaData.xAxisTitle;
    x["label"] = m_metaData.xAxisTitle;
    y["unit"] = m_metaData.yAxisTitle;
    y["label"] = m_metaData.yAxisTitle;
    z["unit"] = m_metaData.zAxisTitle;
    z["label"] = m_metaData.zAxisTitle;

    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        double val = ui.tableWidget->item(-1,i).toDouble();
        xlist.append(QString::number(val,'f',2));
    }
    for (int i=0;i<ui.tableWidget->rowCount();i++)
    {
        double val = ui.tableWidget->item(i,-1).toDouble();
        ylist.append(QString::number(val,'f',2));
    }
    for (int j=0;j<ui.tableWidget->rowCount();j++)
    {
        QVariantList zrow;
        for (int i=0;i<ui.tableWidget->columnCount();i++)
        {
            zrow.append(ui.tableWidget->item(j,i));
        }
        zlist.append((QVariant)zrow);
    }

    y["values"] = ylist;
    x["values"] = xlist;
    z["values"] = zlist;
    topmap["X"] = x;
    topmap["Y"] = y;
    topmap["Z"] = z;

    QJsonDocument doc = QJsonDocument::fromVariant(topmap);
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        //QLOG_ERROR() << "Unable to open file to output JSON!";
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void TableView3D::importClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Load Json File",".","Json Files (*.json)");
    if (filename == "")
    {
        return;
    }
    //Create a JSON file similar to MTX's yaml format.

    //QByteArray serialized = serializer.serialize(topmap);

    //TODO: Open a message box and allow the user to select where they want to save the file.
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        //QLOG_ERROR() << "Unable to open file to output JSON!";
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
    QJsonObject x = topmap.value("X").toObject();
    QJsonObject y = topmap.value("Y").toObject();
    QJsonObject z = topmap.value("Z").toObject();
    QJsonArray xlist = x.value("values").toArray();
    QJsonArray ylist = y.value("values").toArray();
    QJsonArray zlist = z.value("values").toArray();
    QString type = topmap.value("type").toString();
    QString title = topmap.value("title").toString();
    QString description = topmap.value("description").toString();

    if (xlist.size() != ui.tableWidget->columnCount())
    {
        QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(xlist.size()) + " columns of axis data, but table has " + QString::number(ui.tableWidget->columnCount()) + " columns of axis data");
        return;

    }
    if (ylist.size() != ui.tableWidget->rowCount())
    {
        //Error here, wrong number of rows!
        QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(ylist.size()) + " rows of axis data, but table has " + QString::number(ui.tableWidget->rowCount()) + " rows of axis data");
        return;
    }
    if (zlist.size() != ui.tableWidget->rowCount())
    {
        QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(zlist.size()) + " rows of data, but table has " + QString::number(ui.tableWidget->rowCount()) + " rows of data");
        return;
    }
    for (int i=0;i<zlist.size();i++)
    {
        if (zlist[i].toArray().size() != ui.tableWidget->columnCount())
        {
            QMessageBox::information(0,"Error","Unable to load JSON file. File had " + QString::number(zlist[i].toArray().size()) + " columns of data, but table has " + QString::number(ui.tableWidget->columnCount()) + " columns of data");
            return;
        }

    }


    for (int i=0;i<ui.tableWidget->columnCount();i++)
    {
        setSilentValue(-1,i,xlist[i].toString());
    }
    for (int i=0;i<ui.tableWidget->rowCount();i++)
    {
        setSilentValue(i,-1,ylist[i].toString());
    }
    for (int j=0;j<ui.tableWidget->rowCount();j++)
    {
        QJsonArray zrow = zlist[j].toArray();
        for (int i=0;i<ui.tableWidget->columnCount();i++)
        {
            //zrow.append(ui.tableWidget->item(j,i)->text());
            setSilentValue(j,i,zrow[i].toString());
        }
    }
    writeTable(true);
}

void TableView3D::exportClicked()
{
    QString filename = QFileDialog::getSaveFileName(0,"Save Json File",".","Json Files (*.json)");
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
void TableView3D::loadRamClicked()
{
    if (QMessageBox::information(0,"Warning","Doing this will reload the table from ram, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
       // QLOG_DEBUG() << "Ok";
        //emit reloadTableData(m_locationId,true);
        tableData->updateFromRam();
    }
    else
    {
        //QLOG_DEBUG() << "Not ok";
    }
}

void TableView3D::loadClicked()
{
    if (QMessageBox::information(0,"Warning","Doing this will reload the table from flash, and wipe out any changes you may have made. Are you sure you want to do this?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        //QLOG_DEBUG() << "Ok";
        tableData->updateFromFlash();
        //emit reloadTableData(m_locationId,false);
    }
    else
    {
        //QLOG_DEBUG() << "Not ok";
    }

}
bool TableView3D::updateTable()
{
    //connect(ui.tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(tableCurrentCellChanged(int,int,int,int)));
    QList<QPair<int,int> > selectedlist;
    if (ui.tableWidget->selectedItems().size() > 0)
    {
        for (int i=0;i<ui.tableWidget->selectedItems().size();i++)
        {
//			selectedlist.append(QPair<int,int>(ui.tableWidget->selectedItems()[i]->row(),ui.tableWidget->selectedItems()[i]->column()));
        }
    }
    //QLOG_DEBUG() << "updateTable(): Clearing table";
    ui.tableWidget->clear();
    //ui.tableWidget->setMaxValues(tableData->maxCalcedXAxis(),tableData->maxCalcedYAxis(),tableData->maxCalcedValue());
    //ui.tableWidget->horizontalHeader()->hide();
    //ui.tableWidget->verticalHeader()->hide();
    ui.tableWidget->setRowCount(tableData->rows());
    ui.tableWidget->setColumnCount(tableData->columns());
    /*if (tableData->yAxis().size() == 0 || tableData->xAxis().size() == 0)
    {
        //Invalid/empty data
        //QLOG_ERROR() << "3D Table axis had zero values. This is INVALID and should be fixed.";
        return false;
    }*/
    /*
    double first = tableData->yAxis()[0];
    int order = 0;
    m_yAxisMax = first;
    for (int i=0;i<tableData->rows();i++)
    {
        if (tableData->yAxis()[i] > m_yAxisMax)
        {
            m_yAxisMax = tableData->yAxis()[i];
        }
        if (i == 1)
        {
            if (tableData->yAxis()[i] < first)
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
            if (tableData->yAxis()[i] > first)
            {
                //Out of order table axis.
                return false;
            }
        }
        else if (order == 2)
        {
            if (tableData->yAxis()[i] < first)
            {
                //Out of order table axis.
                return false;
            }
        }
        first = tableData->yAxis()[i];

        if (tableData->yAxis()[i] < first)
        {
            //Out of order axis;
            return false;
        }
        first = tableData->yAxis()[i];
        ui.tableWidget->setYAxis((tableData->rows()-1)-i,formatNumber(tableData->yAxis()[i],m_metaData.yDp));
    }
    first = tableData->xAxis()[0];
    m_xAxisMax = first;
    for (int i=0;i<tableData->columns();i++)
    {
        if (tableData->xAxis()[i] > m_xAxisMax)
        {
            m_xAxisMax = tableData->xAxis()[i];
        }
        if (i == 1)
        {
            if (tableData->xAxis()[i] < first)
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
            if (tableData->xAxis()[i] > first)
            {
                //Out of order table axis.
                return false;
            }
        }
        else if (order == 2)
        {
            if (tableData->xAxis()[i] < first)
            {
                //Out of order table axis.
                return false;
            }
        }
        first = tableData->xAxis()[i];
        if (tableData->xAxis()[i] < first)
        {
            //Out of order axis;
            return false;
        }
        first = tableData->xAxis()[i];
        ui.tableWidget->setXAxis(i,formatNumber(tableData->xAxis()[i],m_metaData.xDp));
    }*/
    for (int x=0;x<m_xAxis->size();x++)
    {
        ui.tableWidget->setXAxis(x,QString::number(m_xAxis->getValue(x).toFloat()));
    }
    for (int y=0;y<m_yAxis->size();y++)
    {
        ui.tableWidget->setXAxis(y,QString::number(m_yAxis->getValue(y).toFloat()));
    }
    m_valueMax = tableData->values()[0][0];
    for (int row=0;row<tableData->rows();row++)
    {
        for (int col=0;col<tableData->columns();col++)
        {
            double val = tableData->values()[row][col];
            if (val > m_valueMax)
            {
                m_valueMax = val;
            }
            ui.tableWidget->setItem((tableData->rows()-1)-row,col,formatNumber(val,m_metaData.zDp));
            //ui.tableWidget->setItem((tableData->rows()-1)-(row),col+1,new QTableWidgetItem(formatNumber(val,m_metaData.zDp)));
            if (val < tableData->maxCalcedValue()/4)
            {
                //ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(0,(255*((val)/(tableData->maxCalcedValue()/4.0))),255));
            }
            else if (val < ((tableData->maxCalcedValue()/4)*2))
            {
                //ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(0,255,255-(255*((val-((tableData->maxCalcedValue()/4.0)))/(tableData->maxCalcedValue()/4.0)))));
            }
            else if (val < ((tableData->maxCalcedValue()/4)*3))
            {
                //ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb((255*((val-((tableData->maxCalcedValue()/4.0)*2))/(tableData->maxCalcedValue()/4.0))),255,0));
            }
            else
            {
                //ui.tableWidget->item((tableData->rows()-1)-((row)),(col)+1)->setBackgroundColor(QColor::fromRgb(255,255-(255*((val-((tableData->maxCalcedValue()/4.0)*3))/(tableData->maxCalcedValue()/4.0))),0));
            }
        }
    }
    for (int i=0;i<selectedlist.size();i++)
    {
        //ui.tableWidget->item(selectedlist[i].first,selectedlist[i].second)->setSelected(true);
    }
    ui.tableWidget->setMaxValues(m_xAxisMax,m_yAxisMax,m_valueMax);


    selectedlist.clear();
    //QLOG_DEBUG() << "updateTable(): Done with table";
    return true;
    //return passData(locationid,data,physicallocation,Table3DMetaData());
}
/*
void TableView3D::setMetaData(Table3DMetaData metadata)
{
    m_metaData = metadata;
    metaDataValid = true;
}*/
bool TableView3D::setData(QString name,ArrayConfigData *x,ArrayConfigData *y,ArrayConfigData *z)
//bool TableView3D::setData(QString name,TSBPTable3DData *data)
{
    m_xAxis = x;
    m_yAxis = y;
    m_zAxis = z;
    Q_UNUSED(name)
    if (tableData == 0)
    {
//        tableData = data;
        //tableData = dynamic_cast<Table3DData*>(data);
        //connect(tableData,SIGNAL(update()),this,SLOT(updateTable()));
        //connect(tableData,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
    }
    else
    {
    }
    //m_locationId = locationid;
    return updateTable();
}
/*
bool TableView3D::setData(unsigned short locationid,DataBlock *data)
{

    if (tableData == 0)
    {
        tableData = dynamic_cast<Table3DData*>(data);
        connect(tableData,SIGNAL(update()),this,SLOT(updateTable()));
        //connect(tableData,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
    }
    else
    {
    }
    m_locationId = locationid;
    return updateTable();
}*/

void TableView3D::passDatalog(QVariantMap data)
{
    if (!m_tracingEnabled)
    {
        return;
    }
    if (data.contains(m_metaData.xHighlight) && data.contains(m_metaData.yHighlight))
    {
        double xval = data[m_metaData.xHighlight].toDouble();
        double yval = data[m_metaData.yHighlight].toDouble();
        ui.tableWidget->setTracingValue(xval,yval);
    }
}
QString TableView3D::formatNumber(double num,int prec)
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

void TableView3D::setSilentValue(int row,int column,QString value)
{
    ui.tableWidget->setItem(row,column,value);
}


void TableView3D::saveClicked()
{
    //emit saveToFlash(m_locationId);
    tableData->saveRamToFlash();
}
TableView3D::~TableView3D()
{
}
void TableView3D::itemChangeRequest(int minrow,int maxrow, int mincolumn, int maxcolumn,QString text)
{
    bool ok = false;
    double newval = text.toDouble(&ok);
    if (!ok)
    {
        //QLOG_ERROR() << "Attempted to enter a number into table that wasn't a number!";
        QMessageBox::information(0,"Error",text + " is not a number.'");
        return;
    }
    QList<QPair<QPair<int,int>,double> > vallist;
    for (int x=minrow;x<=maxrow;x++)
    {
        for (int y=mincolumn;y<=maxcolumn;y++)
        {
            QPair<QPair<int,int>,double> val;
            val.first = QPair<int,int>(x,y);
            val.second = newval;
            vallist.append(val);
        }
    }
    setRange(vallist);
}

void TableView3D::itemChangeRequest(int row,int column,QString text)
{

    bool ok = false;
    double newval = text.toDouble(&ok);
    if (!ok)
    {
        //QLOG_ERROR() << "Attempted to enter a number into table that wasn't a number!";
        QMessageBox::information(0,"Error",text + " is not a number.'");
        return;
    }
    setValue(row,column,newval);
}
