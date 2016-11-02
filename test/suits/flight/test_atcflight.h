#ifndef TEST_ATCFLIGHT_H
#define TEST_ATCFLIGHT_H

#include "atcflight.h"
#include "atcflightplanfactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCFlight : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_setFlightPlan();
    void test_setSquawk();
    void test_setState();
};

#endif // TEST_ATCFLIGHT_H
