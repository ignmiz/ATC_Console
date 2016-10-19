
#include "test_atcrunwayextendedcentreline.h"

void Test_ATCRunwayExtendedCentreline::test_constructObject()
{
    ATCRunwayExtendedCentreline foo(new QGraphicsLineItem());
    QVERIFY(foo.getTicksVector().size() == 0);
    QVERIFY(foo.getCentreline() != nullptr);
}

void Test_ATCRunwayExtendedCentreline::test_setGetCentreline()
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    ATCRunwayExtendedCentreline foo(line);

    QVERIFY(foo.getCentreline() == line);
}

void Test_ATCRunwayExtendedCentreline::test_appendTick()
{
    ATCRunwayExtendedCentreline foo(new QGraphicsLineItem());
    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    QVERIFY(foo.getTicksVector().at(0) == tick);
}

void Test_ATCRunwayExtendedCentreline::test_getTicksVector()
{
    ATCRunwayExtendedCentreline foo(new QGraphicsLineItem());
    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    QVERIFY(foo.getTicksVector().at(0) == tick);
    QVERIFY(foo.getTicksVector().size() == 1);
}

void Test_ATCRunwayExtendedCentreline::test_setColor()
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    ATCRunwayExtendedCentreline foo(line);

    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    foo.setColor(Qt::red);

    QVERIFY(foo.getCentreline()->pen().color() == Qt::red);
    QVERIFY(foo.getTicksVector().at(0)->pen().color() == Qt::red);
}

void Test_ATCRunwayExtendedCentreline::test_show()
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    ATCRunwayExtendedCentreline foo(line);

    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    foo.show();

    QVERIFY(foo.getCentreline()->isVisible());
    QVERIFY(foo.getTicksVector().at(0)->isVisible());
    QVERIFY(foo.isVisible());
}

void Test_ATCRunwayExtendedCentreline::test_hide()
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    ATCRunwayExtendedCentreline foo(line);

    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    foo.hide();

    QVERIFY(!foo.getCentreline()->isVisible());
    QVERIFY(!foo.getTicksVector().at(0)->isVisible());
    QVERIFY(!foo.isVisible());
}

void Test_ATCRunwayExtendedCentreline::test_isVisible()
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    ATCRunwayExtendedCentreline foo(line);

    QGraphicsLineItem *tick = new QGraphicsLineItem();
    foo.appendTick(tick);

    foo.hide();
    QVERIFY(!foo.isVisible());

    foo.show();
    QVERIFY(foo.isVisible());
}
