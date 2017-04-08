#ifndef TEST_ATCPROFILESPEED_H
#define TEST_ATCPROFILESPEED_H

#include "atcprofilespeed.h"

#include <QObject>
#include <QTest>

class Test_ATCProfileSpeed : public QObject
{
    Q_OBJECT

private slots:
    void test_nominalClimbSpeed();
    void test_nominalCruiseSpeed();
    void test_nominalDescentSpeed();
};

#endif // TEST_ATCPROFILESPEED_H
