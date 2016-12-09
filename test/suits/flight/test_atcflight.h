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
    void test_setAssignedSquawk();
    void test_setState();
    void test_setNavMode();
    void test_setTargetAltitude();
    void test_setTargetSpeed();
    void test_setHdgRestriction();
    void test_setNextFix();
    void test_setSimStartTime();
    void test_setTag();
    void test_setFixList();
};

#endif // TEST_ATCFLIGHT_H
