/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2022 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio and is licensed under The MIT License (MIT)      *
 * as defined in LICENSE.md at the top level of this repository                     *
 ************************************************************************************/

#include "parameterwidget.h"
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
//#include "QsLog.h"
#include "configdata.h"
#include "tsbpheaders.h"
#include "arrayconfigdata.h"
#include "tableconfigdata.h"
#include "tableview3d.h"
ParameterWidget::ParameterWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.savePushButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
    //scrollWidget = new QWidget();
    //scrollWidget->setLayout(new QVBoxLayout());
    //scrollArea->setWidget(scrollWidget);
    //scrollArea->setWidget(new QWidget());
    ui.scrollArea->widget()->setLayout(new QVBoxLayout());
    ui.scrollArea->widget()->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
    //scrollArea->setLayout(new QVBoxLayout());
    //mainLayout = new QVBoxLayout();
    //this->setLayout(mainLayout);
    //mainLayout->addWidget(scrollArea);
    //mainLayout->addWidget(saveButton);
    //scrollArea->show();
    //scrollWidget->show();
    //scrollArea->show();
}
void ParameterWidget::addTable(ArrayConfigData *xdata,ArrayConfigData *ydata, TableConfigData *zdata)
{
    TableView3D *table = new TableView3D(ui.scrollArea->widget());
    table->setData("Lambda",xdata,ydata,zdata);
    table->show();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(table);
    ui.scrollArea->widget()->layout()->addItem(layout);

}
void ParameterWidget::saveButtonClicked()
{
    for (int i=0;i<m_scalarParamList.size();i++)
    {
        m_scalarParamList[i]->saveValue();
    }
    for (int i=0;i<m_comboParamList.size();i++)
    {
        m_comboParamList[i]->saveValue();
    }
    return;
    for (int i=0;i<m_fieldConfigList.size();i++)
    {
        QLineEdit *edit = m_nameToLineEditMap[m_fieldConfigList[i].first.variable];
        QStringList editsplit = edit->text().split(",");
        if (editsplit.size() == 0)
        {
            //It's a single value
            //int newval = backConvertAxis(edit->text().toDouble(),m_fieldConfigList[i].second.calc());
            //m_fieldConfigList[i].second.locationId()
            //m_fieldConfigList[i].second.offset();
            //m_fieldConfigList[i].second.size();
        }
        else
        {
            //Build a QByteArray based off the array of values.
            QByteArray array;
            QString valstr = "";
            for (int j=0;j<editsplit.size();j++)
            {
                if (m_fieldConfigList[i].second.elementSize() == 1)
                {
                    int newval = backConvertAxis(editsplit[j].toDouble(),m_fieldConfigList[i].second.calc());
                    unsigned char first = newval;
                    array.append(first);
                    valstr += QString::number(first,16).toUpper() + ",";
                }
                else if (m_fieldConfigList[i].second.elementSize() == 2)
                {
                    int newval = backConvertAxis(editsplit[j].toDouble(),m_fieldConfigList[i].second.calc());
                    unsigned char first = newval >> 8;
                    unsigned char second = newval;
                    array.append(first);
                    array.append(second);
                    valstr += QString::number(first,16).toUpper() + "," + QString::number(second,16).toUpper() + ",";
                }
            }
            //QLOG_DEBUG() << "Saving:" << "0x" + QString::number(m_fieldConfigList[i].second.locationId(),16).toUpper() << "Offset:" << QString::number(m_fieldConfigList[i].second.offset()) << valstr;
            emit saveSingleData(m_fieldConfigList[i].second.locationId(),array,m_fieldConfigList[i].second.offset(),array.size());
        }
    }
}

QList<unsigned short> ParameterWidget::getLocationIdList()
{
    QList<unsigned short> retval;
    for (int i=0;i<m_fieldConfigList.size();i++)
    {
        if (!retval.contains(m_fieldConfigList[i].second.locationId()))
        {
            retval.append(m_fieldConfigList[i].second.locationId());
        }
    }
    return retval;
}
void ParameterWidget::updateValue(unsigned short locationid,QByteArray block)
{
    for (int i=0;i<m_fieldConfigList.size();i++)
    {
        if (m_fieldConfigList[i].second.locationId() == locationid)
        {

            QString valstr = "";
            QString bytestr = "";
            for (int k=0;k<m_fieldConfigList[i].second.size();k++)
            {
                unsigned int value = 0;
                for (int j=0;j<m_fieldConfigList[i].second.elementSize();j++)
                {
                    //QLOG_DEBUG() << (unsigned char)block[m_fieldConfigList[i].second.offset() + (k * m_fieldConfigList[i].second.elementSize()) + j];
                    bytestr += QString::number((unsigned char)block[m_fieldConfigList[i].second.offset() + (k * m_fieldConfigList[i].second.elementSize()) + j],16).toUpper() + ",";
                    value += ((unsigned char)block[m_fieldConfigList[i].second.offset() + (k * m_fieldConfigList[i].second.elementSize()) + j]) << (8 * (m_fieldConfigList[i].second.elementSize() - (j+1)));
                }
                //userValue = (ecuValue + translate) * scale

                valstr += QString::number(calcAxis(value,m_fieldConfigList[i].second.calc())) + ",";
            }
            valstr = valstr.mid(0,valstr.length()-1);
            //QLOG_DEBUG() << "Pre value:" << "0x" + QString::number(locationid,16) << bytestr;
            if (m_nameToLineEditMap.contains(m_fieldConfigList[i].first.variable))
            {
                m_nameToLineEditMap[m_fieldConfigList[i].first.variable]->setText(valstr);
            }

        }
    }
}
void ParameterWidget::addParam(QString title,DialogField field,ConfigData* data)
{
    Q_UNUSED(title)
    Q_UNUSED(field)
    //QHBoxLayout *layout = new QHBoxLayout();
    //QLabel *label = new QLabel(scrollArea);
    //label->show();
    //label->setText(field.title);
    //layout->addWidget(label);
    //QLineEdit *edit = new QLineEdit(scrollArea);
    //m_nameToLineEditMap[field.variable] = edit;
    //edit->show();
    //layout->addWidget(edit);
    if (data->type() == ConfigData::VALUE)
    {
        ScalarParam *param = new ScalarParam(ui.scrollArea->widget());
        param->show();
        param->setName(field.title);
        param->setConfig(data);
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(param);
        ui.scrollArea->widget()->layout()->addItem(layout);
        m_scalarParamList.append(param);
    }
    else if (data->type() == ConfigData::ENUM)
    {
        ComboParam *param = new ComboParam(ui.scrollArea->widget());
        param->show();
        param->setName(field.title);
        param->setConfig(data);
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(param);
        ui.scrollArea->widget()->layout()->addItem(layout);
        //m_scalarParamList.append(param);
        m_comboParamList.append(param);
    }
}

/*void ParameterWidget::addParam(QString title,DialogField field,ConfigBlock block)
{
    QLOG_DEBUG() << "Title:" << title;
    QLOG_DEBUG() << "Field:" << field.title << field.variable;
    QLOG_DEBUG() << "Found config block:" << block.type();
    QLOG_DEBUG() << block.name();
    m_fieldConfigList.append(QPair<DialogField,ConfigBlock>(field,block));
    //This is the config block.
    if (block.type() == "scalar" || block.type() == "value" || block.type() == "array")
    {
        QHBoxLayout *layout = new QHBoxLayout();
        QLabel *label = new QLabel(scrollArea);
        label->show();
        label->setText(field.title);
        layout->addWidget(label);
        QLineEdit *edit = new QLineEdit(scrollArea);
        m_nameToLineEditMap[field.variable] = edit;
        edit->show();
        layout->addWidget(edit);
        scrollArea->layout()->addItem(layout);
        //lineEditToConfigBlockMap[edit] = m_memoryConfigBlockList[j];
    }
    //fieldlist[i].condition
}*/
double ParameterWidget::calcAxis(int val,QList<QPair<QString,double> > metadata)
{
    if (metadata.size() == 0)
    {
        return val;
    }
    double newval = val;
    for (int j=0;j<metadata.size();j++)
    {
        if (metadata[j].first == "add")
        {
            newval += metadata[j].second;
        }
        else if (metadata[j].first == "sub")
        {
            newval -= metadata[j].second;
        }
        else if (metadata[j].first == "mult")
        {
            newval *= metadata[j].second;
        }
        else if (metadata[j].first == "div")
        {
            newval /= metadata[j].second;
        }
    }
    return newval;
}
int ParameterWidget::backConvertAxis(double val,QList<QPair<QString,double> > metadata)
{
    if (metadata.size() == 0)
    {
        return val;
    }
    double newval = val;
    for (int j=metadata.size()-1;j>=0;j--)
    {
        if (metadata[j].first == "add")
        {
            newval -= metadata[j].second;
        }
        else if (metadata[j].first == "sub")
        {
            newval += metadata[j].second;
        }
        else if (metadata[j].first == "mult")
        {
            newval /= metadata[j].second;
        }
        else if (metadata[j].first == "div")
        {
            newval *= metadata[j].second;
        }
    }
    return (int)newval;
}
