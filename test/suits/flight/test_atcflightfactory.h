#ifndef TEST_ATCFLIGHTFACTORY_H
#define TEST_ATCFLIGHTFACTORY_H

#include "atcflightfactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_newFlight();
    void test_generateSquawk();
};

#endif // TEST_ATCFLIGHTFACTORY_H
