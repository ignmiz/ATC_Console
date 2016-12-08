
#include "test_atcabstractairway.h"

void Test_ATCAbstractAirway::test_constructObject()
{
    Mock_ATCAbstractAirway airway("TEST");
    QVERIFY(airway.getName() == "TEST");
    QVERIFY(airway.getCoordsVectorSize() == 0);
}

void Test_ATCAbstractAirway::test_getName()
{
    Mock_ATCAbstractAirway airway("TEST");
    QVERIFY(airway.getName() == "TEST");
}

void Test_ATCAbstractAirway::test_setName()
{
    Mock_ATCAbstractAirway airway("TEST");
    airway.setName("NEW");
    QVERIFY(airway.getName() == "NEW");
}

void Test_ATCAbstractAirway::test_appendGetCoords1()
{
    Mock_ATCAbstractAirway airway("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    airway.appendCoords1(fix);

    QVERIFY(airway.getCoords1(0) == fix);
}

void Test_ATCAbstractAirway::test_appendGetCoords2()
{
    Mock_ATCAbstractAirway airway("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    airway.appendCoords2(fix);

    QVERIFY(airway.getCoords2(0) == fix);
}

void Test_ATCAbstractAirway::test_appendGetLine()
{
    Mock_ATCAbstractAirway airway("TEST");

    QGraphicsLineItem *line = new QGraphicsLineItem();
    airway.appendLine(line);

    QVERIFY(airway.getLine(0) == line);
}

void Test_ATCAbstractAirway::test_getCoordsVectorSize()
{
    Mock_ATCAbstractAirway airway("TEST");
    QVERIFY(airway.getCoordsVectorSize() == 0);

    ATCAirspaceFix *fix1 = new ATCAirspaceFix(10, 20);
    airway.appendCoords1(fix1);
    QVERIFY(airway.getCoordsVectorSize() == 1);

    ATCAirspaceFix *fix2 = new ATCAirspaceFix(10, 20);
    airway.appendCoords2(fix2);
    QVERIFY(airway.getCoordsVectorSize() == 1);
}

void Test_ATCAbstractAirway::test_setColor()
{
    Mock_ATCAbstractAirway airway("TEST");

    QGraphicsLineItem *line = new QGraphicsLineItem();
    airway.appendLine(line);
    airway.setColor(Qt::red);

    QVERIFY(airway.getLine(0)->pen().color() == Qt::red);
}

void Test_ATCAbstractAirway::test_show()
{
    Mock_ATCAbstractAirway airway("TEST");
    QVERIFY(!airway.isVisible());

    QGraphicsLineItem *line = new QGraphicsLineItem();
    airway.appendLine(line);

    airway.show();

    QVERIFY(airway.isVisible());
    QVERIFY(airway.getLine(0)->isVisible());
}

void Test_ATCAbstractAirway::test_hide()
{
    Mock_ATCAbstractAirway airway("TEST");
    QVERIFY(!airway.isVisible());

    QGraphicsLineItem *line = new QGraphicsLineItem();
    airway.appendLine(line);

    airway.hide();

    QVERIFY(!airway.isVisible());
    QVERIFY(!airway.getLine(0)->isVisible());
}

void Test_ATCAbstractAirway::test_isVisible()
{
    Mock_ATCAbstractAirway airway("TEST");

    airway.hide();
    QVERIFY(!airway.isVisible());

    airway.show();
    QVERIFY(airway.isVisible());
}

void Test_ATCAbstractAirway::test_appendGetRouteFix()
{
    Mock_ATCAbstractAirway airway("TEST");

    airway.appendRouteFix("TEST1");
    airway.appendRouteFix("TEST2");

    QVERIFY(airway.getRouteFixes().size() == 2);
    QVERIFY(airway.getRouteFixes().at(0) == "TEST1");
    QVERIFY(airway.getRouteFixes().at(1) == "TEST2");
}
