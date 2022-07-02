#include "consoletextview.h"
#include "ui_consoletextview.h"
#include <QDateTime>
ConsoleTextView::ConsoleTextView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleTextView)
{
    ui->setupUi(this);
}

ConsoleTextView::~ConsoleTextView()
{
    delete ui;
}
void ConsoleTextView::consoleText(QString text)
{
    QList<QString> textsplit = text.split("`");
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyy/MM/dd hh:mm:ss");
    for (int i=0;i<textsplit.size()-1;i+=2)
    {
        if (textsplit[i] == "msg")
        {
            ui->textBrowser->append(formattedTime + " " + textsplit[i+1]);
        }
        else if (textsplit[i] == "outpin")
        {
            ui->textBrowser->append(formattedTime + " " + textsplit[i+1]);
        }
        else if (textsplit[i] == "rusEfiVersion")
        {

        }
        else
        {
            qDebug() << "Message not handled" << textsplit[i] << textsplit[i+1];
        }
    }

}
