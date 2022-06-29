#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    ui->serialRadioButton->setChecked(true);
    ui->tcpGroupBox->setEnabled(false);
    connect(ui->connectPushButton,&QPushButton::clicked,this,&ConnectionDialog::connectPushButtonClicked);
    connect(ui->serialRadioButton,&QRadioButton::clicked,this,&ConnectionDialog::serialRadioButtonClicked);
    connect(ui->tcpRadioButton,&QRadioButton::clicked,this,&ConnectionDialog::tcpRadioButtonClicked);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}
void ConnectionDialog::connectPushButtonClicked()
{
    if (ui->serialRadioButton->isChecked())
    {
        emit connectionSelected(true,ui->comLineEdit->text(),ui->comBaudLineEdit->text().toInt());
    }
    else
    {
        emit connectionSelected(false,ui->tcpHostLineEdit->text(),ui->tcpPortLineEdit->text().toInt());
    }
    emit done();
    this->close();
}
void ConnectionDialog::serialRadioButtonClicked()
{
    ui->tcpGroupBox->setEnabled(false);
    ui->serialGroupBox->setEnabled(true);
}
void ConnectionDialog::tcpRadioButtonClicked()
{
    ui->tcpGroupBox->setEnabled(true);
    ui->serialGroupBox->setEnabled(false);
}
