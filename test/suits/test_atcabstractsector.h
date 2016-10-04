#ifndef TEST_ATCABSTRACTSECTOR_H
#define TEST_ATCABSTRACTSECTOR_H

#include "mock_atcabstractsector.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractSector : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getName();
    void test_setName();
    void test_appendGetCoords1();
    void test_appendGetCoords2();
    void test_getCoordsVectorSize();
    void test_appendGetPolygon();
    void test_getPolygonsVectorSize();
    void test_appendGetCoordsPair();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCABSTRACTSECTOR_H
