#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QWidget>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();
signals:
    void connectionSelected(bool isSerial,QString portorhost,int baudorport,QString inifile);
    void done();
private slots:
    void connectPushButtonClicked();
    void serialRadioButtonClicked();
    void tcpRadioButtonClicked();
    void iniBrowseButtonClicked();
private:
    Ui::ConnectionDialog *ui;
    QString m_iniFileName;
};

#endif // CONNECTIONDIALOG_H
