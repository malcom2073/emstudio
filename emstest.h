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
    bool testSimple2DArray();
    bool testSimple2DWrite();
private:
    MSPComms *m_comms;
    bool m_failure;
signals:
public slots:
    void interrogationCompleted();
};

#endif // EMSTEST_H
