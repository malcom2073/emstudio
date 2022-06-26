#ifndef EMSTEST_H
#define EMSTEST_H

#include <QObject>
#include "mspcomms.h"
class EMSTest : public QObject
{
    Q_OBJECT
public:
    explicit EMSTest(QObject *parent = nullptr);
    void startTest();
private:
    MSPComms *m_comms;
signals:
public slots:
    void interrogationCompleted();
};

#endif // EMSTEST_H
