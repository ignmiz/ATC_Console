#ifndef TEST_ATCAIRSPACESECTOR_H
#define TEST_ATCAIRSPACESECTOR_H

#include "atcairspacesector.h"

#include <QTest>
#include <QObject>

class Test_ATCAirspaceSector : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getName();
    void test_setName();
    void test_appendGetCoordinates();
    void test_getCoordinatesVectorSize();
    void test_setGetPolygon();
    void test_deleteAllAirspaceFixes();
};

#endif // TEST_ATCAIRSPACESECTOR_H
