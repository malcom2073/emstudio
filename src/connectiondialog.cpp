#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QFileDialog>
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
    connect(ui->iniBrowsePushButton,&QPushButton::clicked,this,&ConnectionDialog::iniBrowseButtonClicked);
    ui->comLineEdit->setText("COM12");
    ui->comBaudLineEdit->setText("115200");
    ui->tcpHostLineEdit->setText("localhost");
    ui->tcpPortLineEdit->setText("29001");
    m_iniFileName = "";
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}
void ConnectionDialog::connectPushButtonClicked()
{
    if (ui->serialRadioButton->isChecked())
    {
        emit connectionSelected(true,ui->comLineEdit->text(),ui->comBaudLineEdit->text().toInt(),m_iniFileName);
    }
    else
    {
        emit connectionSelected(false,ui->tcpHostLineEdit->text(),ui->tcpPortLineEdit->text().toInt(),m_iniFileName);
    }
    this->hide();
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
void ConnectionDialog::iniBrowseButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    ui->iniPathLineEdit->setText(filename);
    m_iniFileName = filename;
}
