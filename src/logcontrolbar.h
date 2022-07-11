#ifndef LOGCONTROLBAR_H
#define LOGCONTROLBAR_H

#include <QWidget>

namespace Ui {
class LogControlBar;
}

class LogControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit LogControlBar(QWidget *parent = nullptr);
    ~LogControlBar();
    void setTime(int current,int total);
signals:
    void logPosChanged(int pos);
private slots:
    void sliderPressed();
    void sliderReleased();
    void sliderMoved(int pos);
private:
    int m_currentPos;
    bool m_sliderPressed;
    Ui::LogControlBar *ui;
};

#endif // LOGCONTROLBAR_H
