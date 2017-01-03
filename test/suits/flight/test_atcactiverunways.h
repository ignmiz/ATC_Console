#ifndef TEST_ATCACTIVERUNWAYS_H
#define TEST_ATCACTIVERUNWAYS_H

#include "atcactiverunways.h"

#include <QObject>
#include <QTest>

class Test_ATCActiveRunways : public QObject
{
    Q_OBJECT

private slots:
    void test_getActiveAirports();
    void test_getActiveAirport();
    void test_clearActiveAirports();
};

#endif // TEST_ATCACTIVERUNWAYS_H
