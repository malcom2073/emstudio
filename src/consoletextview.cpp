#include "consoletextview.h"
#include "ui_consoletextview.h"

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
    ui->textBrowser->append(text);
}
