
#include "test_atcabstractsector.h"

void Test_ATCAbstractSector::test_constructObject()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}

void Test_ATCAbstractSector::test_getName()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(foo.getName() == "TEST");
}

void Test_ATCAbstractSector::test_setName()
{
    Mock_ATCAbstractSector foo("TEST");
    foo.setName("NEW");
    QVERIFY(foo.getName() == "NEW");
}

void Test_ATCAbstractSector::test_appendGetCoords1()
{
    Mock_ATCAbstractSector foo("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    foo.appendCoords1(fix);

    QVERIFY(foo.getCoords1(0) == fix);
}

void Test_ATCAbstractSector::test_appendGetCoords2()
{
    Mock_ATCAbstractSector foo("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    foo.appendCoords2(fix);

    QVERIFY(foo.getCoords2(0) == fix);
}

void Test_ATCAbstractSector::test_getCoordsVectorSize()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);

    ATCAirspaceFix *fix1 = new ATCAirspaceFix(10, 20);
    foo.appendCoords1(fix1);
    QVERIFY(foo.getCoordsVectorSize() == 1);

    ATCAirspaceFix *fix2 = new ATCAirspaceFix(10, 20);
    foo.appendCoords2(fix2);
    QVERIFY(foo.getCoordsVectorSize() == 1);
}

void Test_ATCAbstractSector::test_appendGetPolygon()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(foo.getPolygonsVectorSize() == 0);

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    foo.appendPolygon(poly);
    QVERIFY(foo.getPolygon(0) == poly);
}

void Test_ATCAbstractSector::test_getPolygonsVectorSize()
{
    Mock_ATCAbstractSector foo("TEST");

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    foo.appendPolygon(poly);
    QVERIFY(foo.getPolygonsVectorSize() == 1);
}

void Test_ATCAbstractSector::test_appendGetCoordsPair()
{
    Mock_ATCAbstractSector foo("TEST");

    coordsPair coords;
    coords.x1 = 1;
    coords.y1 = 2;
    coords.x2 = 3;
    coords.y2 = 4;

    foo.appendCoordsPair(coords);
    QVERIFY(foo.getCoordsPair(0).x1 == 1);
    QVERIFY(foo.getCoordsPair(0).y1 == 2);
    QVERIFY(foo.getCoordsPair(0).x2 == 3);
    QVERIFY(foo.getCoordsPair(0).y2 == 4);
}

void Test_ATCAbstractSector::test_setColor()
{
    Mock_ATCAbstractSector foo("TEST");

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    foo.appendPolygon(poly);

    foo.setColor(Qt::red);

    QVERIFY(foo.getPolygon(0)->pen().color() == Qt::red);
}

void Test_ATCAbstractSector::test_show()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(!foo.isVisible());

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    foo.appendPolygon(poly);

    foo.show();

    QVERIFY(foo.isVisible());
    QVERIFY(foo.getPolygon(0)->isVisible());
}

void Test_ATCAbstractSector::test_hide()
{
    Mock_ATCAbstractSector foo("TEST");
    QVERIFY(!foo.isVisible());

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    foo.appendPolygon(poly);

    foo.hide();

    QVERIFY(!foo.isVisible());
    QVERIFY(!foo.getPolygon(0)->isVisible());
}

void Test_ATCAbstractSector::test_isVisible()
{
    Mock_ATCAbstractSector foo("TEST");

    foo.hide();
    QVERIFY(!foo.isVisible());

    foo.show();
    QVERIFY(foo.isVisible());
}
