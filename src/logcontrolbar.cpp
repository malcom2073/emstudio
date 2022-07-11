#include "logcontrolbar.h"
#include "ui_logcontrolbar.h"

LogControlBar::LogControlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogControlBar)
{
    ui->setupUi(this);
    connect(ui->logTimeSlider,&QSlider::sliderPressed,this,&LogControlBar::sliderPressed);
    connect(ui->logTimeSlider,&QSlider::sliderReleased,this,&LogControlBar::sliderReleased);
    connect(ui->logTimeSlider,&QSlider::sliderMoved,this,&LogControlBar::sliderMoved);
    m_sliderPressed = false;
}
void LogControlBar::sliderPressed()
{
    m_sliderPressed = true;
}
void LogControlBar::sliderReleased()
{
    emit logPosChanged(m_currentPos);
    m_sliderPressed = false;
}
void LogControlBar::sliderMoved(int pos)
{
    if (m_sliderPressed)
    {
        m_currentPos = pos;
    }
}
LogControlBar::~LogControlBar()
{
    delete ui;
}
void LogControlBar::setTime(int current,int total)
{
    if (m_sliderPressed)
    {
        return;
    }
    m_currentPos = current;
    if (ui->logTimeSlider->maximum() != total)
    {
        ui->logTimeSlider->setMaximum(total);
    }
    ui->logTimeSlider->setValue(current);
}
