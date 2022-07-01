#include "parambuttonbar.h"
#include "ui_parambuttonbar.h"
#include <QPushButton>
#include <QMenu>
#include <QMdiSubWindow>
#include "tableview2d.h"
ParamButtonBar::ParamButtonBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamButtonBar)
{
    ui->setupUi(this);
}

ParamButtonBar::~ParamButtonBar()
{
    delete ui;
}
void ParamButtonBar::passMenuList(MenuSetup menu)
{
    m_metaMenu = menu;
    for (int i=0;i<menu.menulist.size();i++)
    {
        //menu.menulist.
        QPushButton *button = new QPushButton(this);
        connect(button,&QPushButton::clicked,this,&ParamButtonBar::buttonClicked);
        button->setText(menu.menulist[i].title);
        ui->horizontalLayout->addWidget(button);
        button->show();
        QMenu *guimenu = new QMenu(this);

        for (int j=0;j<menu.menulist[i].subMenuList.size();j++)
        {
            //if (menu.menunamemap.contains(menu.menulist[i].second[j]))
            //{
            //	item->addChild(new QTreeWidgetItem(QStringList() << menu.menunamemap[menu.menulist[i].second[j]]));
            //}
            //else
            //{
            //	item->addChild(new QTreeWidgetItem(QStringList() << menu.menulist[i].second[j]));
            //}
            QAction *action = guimenu->addAction(menu.menulist[i].subMenuList[j].title);
            connect(action,&QAction::triggered,this,&ParamButtonBar::actionTriggered);
            m_actionVariableMap.insert(action,menu.menulist[i].subMenuList[j].variable);
            //QTreeWidgetItem *childitem = new QTreeWidgetItem(QStringList() << menu.menulist[i].subMenuList[j].title << menu.menulist[i].subMenuList[j].variable);
            //m_treeWidgetToVariableNameMap[childitem] = menu.menulist[i].subMenuList[j].variable;
            //item->addChild(childitem);
        }
        m_buttonToMenuMap.insert(button,guimenu);
    }
}
void ParamButtonBar::buttonClicked()
{
    QPushButton *clicked = qobject_cast<QPushButton*>(sender());
    if (m_buttonToMenuMap.contains(clicked))
    {
        QPoint p(clicked->pos().x(),clicked->pos().y() + clicked->height());
        m_buttonToMenuMap[clicked]->popup(this->mapToGlobal(p));
    }
}
void ParamButtonBar::setActiveComms(MSPComms* comms)
{
    m_emsComms = comms;
}
void ParamButtonBar::burnData()
{
    m_emsComms->sendBurn();
}

void ParamButtonBar::actionTriggered()
{
    QAction* clickedaction = qobject_cast<QAction*>(sender());
    if (m_actionVariableMap.contains(clickedaction))
    {
        //Good to go here with m_actionVariableMap[clickedaction] as our QString variable!
        QString variablename =m_actionVariableMap[clickedaction];
        //QLOG_DEBUG() << "Variable:" << m_metaMenu.menulist[i].subMenuList[j].variable;
        //emit showTable(variablename);
        bool found = false;
        for (int k=0;k<m_metaMenu.dialoglist.size();k++)
        {
            if (m_metaMenu.dialoglist[k].variable == variablename)
            {
                //QLOG_DEBUG() << "Found Dialog:" << m_metaMenu.dialoglist[k].title << m_metaMenu.dialoglist[k].fieldList.size();
                if (m_metaMenu.dialoglist[k].panelList.size() > 0)
                {
                    //Has sub-panels, find and populate them.
                }
                found = true;
                generateDialog(m_metaMenu.dialoglist[k]);
                //Generate a dialog here.
            }
        }
        if (!found)
        {
            Table* table = m_emsComms->getTableFromName(variablename);
            if (table)
            {
                ParameterWidget *widget = new ParameterWidget();
                //connect(widget,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
                //widget->show();
                //paramWidgetList.append(widget);
                ArrayConfigData *xdata = m_emsComms->getArrayConfigData(table->xbin);
                ArrayConfigData *ydata = m_emsComms->getArrayConfigData(table->ybin);
                TableConfigData *zdata = m_emsComms->getTableConfigData(table->zbin);
                if (xdata && ydata && zdata)
                {
                    widget->addTable(xdata,ydata,zdata);
                }
                QMdiSubWindow *sub = new QMdiSubWindow(this->m_mdiArea);
                sub->setWidget(widget);
                sub->setAttribute(Qt::WA_DeleteOnClose,true);
                sub->show();
                m_mdiArea->addSubWindow(sub);
            }
            else
            {
                Curve* curve = m_emsComms->getCurveFromName(variablename);
                if (curve)
                {
                    ParameterWidget *widget = new ParameterWidget();
                    widget->setWindowTitle(curve->title);
                    ArrayConfigData *xdata = m_emsComms->getArrayConfigData(curve->xbin);
                    ArrayConfigData *ydata = m_emsComms->getArrayConfigData(curve->ybin);
                    if (xdata && ydata)
                    {
                        widget->addCurve(xdata,ydata);
                    }
                    QMdiSubWindow *sub = new QMdiSubWindow(this->m_mdiArea);
                    sub->setWidget(widget);
                    sub->setAttribute(Qt::WA_DeleteOnClose,true);
                    sub->show();
                    m_mdiArea->addSubWindow(sub);
                }
            }
            //No dialog found, show the raw data type.
        }

    }
}
void ParamButtonBar::passMdiArea(QMdiArea *area)
{
    m_mdiArea = area;
}
void ParamButtonBar::generateDialog(DialogItem item,QString dir,ParameterWidget *parent)
{
    //QString title,QList<DialogField> fieldlist
    if (item.panelList.size() > 0)
    {
        int stopper = 1;
    }
    QString title = item.title;
    QList<DialogField> fieldlist = item.fieldList;
    ParameterWidget *widget;
    if (parent)
    {
        widget = new ParameterWidget(parent);

        widget->disableSaveButton();
        parent->addPanel(widget,dir);
    }
    else
    {
        widget = new ParameterWidget();
        connect(widget,&ParameterWidget::burnData,this,&ParamButtonBar::burnData);
    }
    widget->setWindowTitle(item.title);
    connect(widget,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)),this,SIGNAL(saveSingleData(unsigned short,QByteArray,unsigned short,unsigned short)));
    //widget->show();
    //widget->resize(10,10);
    for (QMap<QString,DialogItem>::const_iterator i=item.panelMap.constBegin();i!=item.panelMap.constEnd();i++)
    {
        //generateDialog(i.value(),widget);
    }
    if (item.panelList.size() > 0)
    {
        //We have subpanels! Create them
        for (int i=0;i<item.panelList.size();i++)
        {
            for (int j=0;j<this->m_metaMenu.dialoglist.size();j++)
            {
                if (this->m_metaMenu.dialoglist.at(j).variable == item.panelList.at(i).second)
                {
                    if (item.panelList.at(i).first == "East")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"East",widget);
                    }
                    else if (item.panelList.at(i).first == "Center")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"Center",widget);
                    }
                    else if (item.panelList.at(i).first == "West")
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"West",widget);
                    }
                    else
                    {
                        generateDialog(this->m_metaMenu.dialoglist.at(j),"",widget);
                    }
                }
            }
        }
        //generateDialog()
    }
    //paramWidgetList.append(widget);
    for (int i=0;i<item.panelList.size();i++)
    {
        Table* table = m_emsComms->getTableFromName(item.panelList[i].second);
        if (table)
        {
            widget->setWindowTitle(table->title);
            ArrayConfigData *xdata = m_emsComms->getArrayConfigData(table->xbin);
            ArrayConfigData *ydata = m_emsComms->getArrayConfigData(table->ybin);
            TableConfigData *zdata = m_emsComms->getTableConfigData(table->zbin);
            if (xdata && ydata && zdata)
            {
                widget->addTable(xdata,ydata,zdata);
            }
        }
        else
        {
            Curve* curve = m_emsComms->getCurveFromName(item.panelList[i].second);
            if (curve)
            {
                widget->setWindowTitle(curve->title);
                ArrayConfigData *xdata = m_emsComms->getArrayConfigData(curve->xbin);
                ArrayConfigData *ydata = m_emsComms->getArrayConfigData(curve->ybin);
                if (xdata && ydata)
                {
                    widget->addCurve(xdata,ydata);
                }
            }
        }
    }
    for (int i=0;i<fieldlist.size();i++)
    {
        //QLOG_DEBUG() << "Field:" << fieldlist[i].title << fieldlist[i].variable;
        if (m_emsComms->getConfigList().contains(fieldlist[i].variable))
        {
            ScalarConfigData *data = m_emsComms->getScalarConfigData(fieldlist[i].variable);
            //widget->addParam(title,fieldlist[i],m_emsComms->getConfigData(fieldlist[i].variable));
            if (data)
            {
                widget->addParam(title,fieldlist[i],data);
            }
            else
            {
                BitConfigData *bdata = m_emsComms->getBitConfigData(fieldlist[i].variable);
                if (bdata)
                {
                    widget->addParam(title,fieldlist[i],bdata);
                    //widget->
                }
            }
        }
        else
        {
            qDebug() << "Bad Variable:" << fieldlist[i].variable;
        }
        /*for (int j=0;j<m_memoryConfigBlockList.size();j++)
        {
            //QLOG_DEBUG() << "Config block:" << m_memoryConfigBlockList[j].type();
            if (m_memoryConfigBlockList[j].name() == fieldlist[i].variable)
            {
                widget->addParam(title,fieldlist[i],m_memoryConfigBlockList[j]);
                QLOG_DEBUG() << "Found config block:" << m_memoryConfigBlockList[j].type();
                QLOG_DEBUG() << m_memoryConfigBlockList[j].name() << j;
                //This is the config block.
                //int sizeoverride = -1;
                if (m_memoryConfigBlockList[j].sizeOverride() != "")
                {
                    //There is a size override, find it.
                    for (int k=0;k<m_memoryConfigBlockList.size();k++)
                    {
                        if (m_memoryConfigBlockList[k].name() == m_memoryConfigBlockList[j].sizeOverride())
                        {
                            //sizeoverride = data[m_memoryConfigBlockList[k].offset()];
                        }
                    }
                }
            }
        }*/
        //fieldlist[i].condition
    }
    if (!parent)
    {
        QMdiSubWindow *sub = new QMdiSubWindow(this->m_mdiArea);
        sub->setWidget(widget);
        sub->setAttribute(Qt::WA_DeleteOnClose,true);
        sub->show();
        m_mdiArea->addSubWindow(sub);
    }
}
