#ifndef PARAMBUTTONBAR_H
#define PARAMBUTTONBAR_H

#include <QWidget>

namespace Ui {
class ParamButtonBar;
}

class ParamButtonBar : public QWidget
{
    Q_OBJECT

public:
    explicit ParamButtonBar(QWidget *parent = nullptr);
    ~ParamButtonBar();

private:
    Ui::ParamButtonBar *ui;
};

#endif // PARAMBUTTONBAR_H
