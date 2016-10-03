#ifndef TEST_ATCABSTRACTPROCEDURE_H
#define TEST_ATCABSTRACTPROCEDURE_H

#include "mock_atcabstractprocedure.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractProcedure : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getName();
    void test_setName();
    void test_getAirport();
    void test_setAirport();
    void test_getRunwayID();
    void test_setRunwayID();
    void test_appendGetFixName();
    void test_setFixList();
    void test_getFixListSize();
};

#endif // TEST_ATCABSTRACTPROCEDURE_H
