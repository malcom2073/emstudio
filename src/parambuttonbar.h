#ifndef PARAMBUTTONBAR_H
#define PARAMBUTTONBAR_H

#include <QWidget>

namespace Ui {
class ParamButtonBar;
}
#include "menusetup.h"
#include <QPushButton>
#include <QMdiArea>
#include "parameterwidget.h"
#include "mspcomms.h"
class ParamButtonBar : public QWidget
{
    Q_OBJECT

public:
    explicit ParamButtonBar(QWidget *parent = nullptr);
    ~ParamButtonBar();
    void passMenuList(MenuSetup menu);
    void setActiveComms(MSPComms* comms);
    void passMdiArea(QMdiArea *area);
private slots:
    void actionTriggered();
    void buttonClicked();
private:
    Ui::ParamButtonBar *ui;
    QMdiArea *m_mdiArea;
    QMap<QAction*,QString> m_actionVariableMap;
    QMap<QPushButton*,QMenu*> m_buttonToMenuMap;
    MenuSetup m_metaMenu;
    void generateDialog(DialogItem item,QString dir="",ParameterWidget *parent=0);
    MSPComms *m_emsComms;
};

#endif // PARAMBUTTONBAR_H
