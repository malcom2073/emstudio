#ifndef CONSOLETEXTVIEW_H
#define CONSOLETEXTVIEW_H

#include <QWidget>

namespace Ui {
class ConsoleTextView;
}

class ConsoleTextView : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleTextView(QWidget *parent = nullptr);
    ~ConsoleTextView();
public slots:
    void consoleText(QString text);
private:
    Ui::ConsoleTextView *ui;
};

#endif // CONSOLETEXTVIEW_H
