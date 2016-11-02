#ifndef TEST_ATCFLIGHTPLANFACTORY_H
#define TEST_ATCFLIGHTPLANFACTORY_H

#include "atcflightplanfactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightPlanFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_getFlightPlan();
};

#endif // TEST_ATCFLIGHTPLANFACTORY_H
