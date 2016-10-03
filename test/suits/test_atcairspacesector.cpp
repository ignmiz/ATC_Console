
#include "test_atcairspacesector.h"

void Test_ATCAirspaceSector::test_constructObject()
{
    ATCAirspaceSector sector("TEST");
    QVERIFY(sector.getName() == "TEST");
    QVERIFY(sector.getCoordinatesVectorSize() == 0);
    QVERIFY(sector.getPolygon() == nullptr);
}

void Test_ATCAirspaceSector::test_getName()
{
    ATCAirspaceSector sector("TEST");
    QVERIFY(sector.getName() == "TEST");
}

void Test_ATCAirspaceSector::test_setName()
{
    ATCAirspaceSector sector("TEST");
    sector.setName("NEW");
    QVERIFY(sector.getName() == "NEW");
}

void Test_ATCAirspaceSector::test_appendGetCoordinates()
{
    ATCAirspaceSector sector("TEST");
    QVERIFY(sector.getCoordinatesVectorSize() == 0);

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    sector.appendAirspaceFix(fix);

    QVERIFY(sector.getCoordinatesVectorSize() == 1);
    QVERIFY(sector.getCoordinates(0) == fix);
}

void Test_ATCAirspaceSector::test_getCoordinatesVectorSize()
{
    ATCAirspaceSector sector("TEST");
    QVERIFY(sector.getCoordinatesVectorSize() == 0);

    ATCAirspaceFix *fix1 = new ATCAirspaceFix(10, 20);
    sector.appendAirspaceFix(fix1);
    QVERIFY(sector.getCoordinatesVectorSize() == 1);

    ATCAirspaceFix *fix2 = new ATCAirspaceFix(30, 40);
    sector.appendAirspaceFix(fix2);
    QVERIFY(sector.getCoordinatesVectorSize() == 2);
}

void Test_ATCAirspaceSector::test_setGetPolygon()
{
    ATCAirspaceSector sector("TEST");

    QGraphicsPolygonItem *polygon = new QGraphicsPolygonItem();
    sector.setPolygon(polygon);
    QVERIFY(sector.getPolygon() == polygon);
}

void Test_ATCAirspaceSector::test_deleteAllAirspaceFixes()
{
    ATCAirspaceSector sector("TEST");
    QVERIFY(sector.getCoordinatesVectorSize() == 0);

    ATCAirspaceFix *fix1 = new ATCAirspaceFix(10, 20);
    sector.appendAirspaceFix(fix1);

    ATCAirspaceFix *fix2 = new ATCAirspaceFix(30, 40);
    sector.appendAirspaceFix(fix2);
    QVERIFY(sector.getCoordinatesVectorSize() == 2);

    sector.deleteAllAirspaceFixes();
    QVERIFY(sector.getCoordinatesVectorSize() == 0);
}
