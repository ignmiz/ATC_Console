#ifndef TEST_ATCFLIGHTPLAN_H
#define TEST_ATCFLIGHTPLAN_H

#include "atcflightplan.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightPlan : public QObject
{
    Q_OBJECT

public:
    explicit Test_ATCFlightPlan();
    ~Test_ATCFlightPlan();

private slots:
    void test_constructObject();
    void test_setFlightRules();
    void test_setCompany();
    void test_setType();
    void test_setRoute();
    void test_setTAS();
    void test_setAltitude();
    void test_setDepartureTime();
    void test_setEnrouteTime();

private:
    QString opf;
    QString apf;
};

#endif // TEST_ATCFLIGHTPLAN_H
