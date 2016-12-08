#ifndef TEST_ATCABSTRACTAIRWAY_H
#define TEST_ATCABSTRACTAIRWAY_H

#include "mock_atcabstractairway.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractAirway : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getName();
    void test_setName();
    void test_appendGetCoords1();
    void test_appendGetCoords2();
    void test_appendGetLine();
    void test_getCoordsVectorSize();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
    void test_appendGetRouteFix();
};


#endif // TEST_ATCABSTRACTAIRWAY_H
