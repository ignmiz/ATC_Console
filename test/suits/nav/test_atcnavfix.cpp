
#include "test_atcnavfix.h"

void Test_ATCNavFix::test_constructObject_correct()
{
    ATCNavFix fix("TEST", 20, 50);
    QVERIFY(fix.getName() == "TEST");
    QVERIFY(fix.latitude() == 20);
    QVERIFY(fix.longitude() == 50);
}

void Test_ATCNavFix::test_constructObject_incorrectLat()
{
    ATCNavFix fix("TEST", -91, 50);
    QVERIFY(fix.getName() == "TEST");
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = ATCNavFix("TEST", 91, 50);
    QVERIFY(fix.getName() == "TEST");
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}

void Test_ATCNavFix::test_constructObject_incorrectLon()
{
    ATCNavFix fix("TEST", 20, -181);
    QVERIFY(fix.getName() == "TEST");
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = ATCNavFix("TEST", 20, 181);
    QVERIFY(fix.getName() == "TEST");
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}

void Test_ATCNavFix::test_getName()
{
    ATCNavFix fix("TEST", 20, 50);
    QVERIFY(fix.getName() == "TEST");
}

void Test_ATCNavFix::test_setName()
{
    ATCNavFix fix("TEST", 20, 50);

    fix.setName("EPPO");
    QVERIFY(fix.getName() == "EPPO");
}

void Test_ATCNavFix::test_setGetScenePosition()
{
    ATCNavFix fix("TEST", 20, 50);

    fix.setScenePosition(new QPointF(10, 30));
    QVERIFY(fix.getScenePosition()->x() == 10);
    QVERIFY(fix.getScenePosition()->y() == 30);
}

void Test_ATCNavFix::test_setGetSymbol()
{
    ATCNavFix fix("TEST", 20, 50);

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    fix.setSymbol(poly);
    QVERIFY(fix.getSymbol() == poly);
}

void Test_ATCNavFix::test_setGetLabel()
{
    ATCNavFix fix("TEST", 20, 50);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    fix.setLabel(txt);
    QVERIFY(fix.getLabel() == txt);
}

void Test_ATCNavFix::test_setColor()
{
    ATCNavFix fix("TEST", 20, 50);

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    fix.setSymbol(poly);

    QColor color(Qt::red);
    fix.setColor(color);

    QVERIFY(fix.getSymbol()->pen().color() == color);
}

void Test_ATCNavFix::test_show()
{
    ATCNavFix fix("TEST", 20, 50);

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    fix.setSymbol(poly);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    fix.setLabel(txt);

    fix.show();
    QVERIFY(fix.getSymbol()->isVisible());
    QVERIFY(fix.getLabel()->isVisible());
}

void Test_ATCNavFix::test_hide()
{
    ATCNavFix fix("TEST", 20, 50);

    QGraphicsPolygonItem *poly = new QGraphicsPolygonItem();
    fix.setSymbol(poly);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    fix.setLabel(txt);

    fix.hide();
    QVERIFY(!fix.getSymbol()->isVisible());
    QVERIFY(!fix.getLabel()->isVisible());
}

void Test_ATCNavFix::test_isVisible()
{
    ATCNavFix fix("TEST", 20, 50);

    fix.show();
    QVERIFY(fix.isVisible());

    fix.hide();
    QVERIFY(!fix.isVisible());
}
