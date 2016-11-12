#ifndef TEST_ATCSIMULATION_H
#define TEST_ATCSIMULATION_H

#include "atcsimulation.h"

#include <QObject>
#include <QTest>

class Test_ATCSimulation : public QObject
{
    Q_OBJECT

private slots:
    void test_getFlight();
    void test_removeFlight();
};


#endif // TEST_ATCSIMULATION_H
