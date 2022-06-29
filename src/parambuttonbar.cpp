#include "parambuttonbar.h"
#include "ui_parambuttonbar.h"

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
