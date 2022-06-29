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
    void connectionSelected(bool isSerial,QString portorhost,int baudorport);
    void done();
private slots:
    void connectPushButtonClicked();
    void serialRadioButtonClicked();
    void tcpRadioButtonClicked();
private:
    Ui::ConnectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
