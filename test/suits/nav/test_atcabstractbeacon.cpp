
#include "test_atcabstractbeacon.h"

void Test_ATCAbstractBeacon::test_constructObject_correct()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(bcn.latitude() == 20);
    QVERIFY(bcn.longitude() == 50);
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}

void Test_ATCAbstractBeacon::test_constructObject_incorrectLat()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, -91, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);

    bcn = Mock_ATCAbstractBeacon("BCN", 113.00, 91, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}

void Test_ATCAbstractBeacon::test_constructObject_incorrectLon()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, -181);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);

    bcn = Mock_ATCAbstractBeacon("BCN", 113.00, 20, 181);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}

void Test_ATCAbstractBeacon::test_getName()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);
    QVERIFY(bcn.getName() == "BCN");
}

void Test_ATCAbstractBeacon::test_setName()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    bcn.setName("TST");
    QVERIFY(bcn.getName() == "TST");
}

void Test_ATCAbstractBeacon::test_getFrequency()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);
    QVERIFY(bcn.getFrequency() == 113.00);
}

void Test_ATCAbstractBeacon::test_setFrequency()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    bcn.setFrequency(111.00);
    QVERIFY(bcn.getFrequency() == 111.00);
}

void Test_ATCAbstractBeacon::test_setGetSymbol()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    bcn.setSymbol(rect);

    QVERIFY(bcn.getSymbol() == rect);
}

void Test_ATCAbstractBeacon::test_setGetLabel()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    bcn.setLabel(txt);

    QVERIFY(bcn.getLabel() == txt);
}

void Test_ATCAbstractBeacon::test_setColor()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    bcn.setSymbol(rect);
    bcn.setColor(Qt::red);

    QVERIFY(bcn.getSymbol()->pen().color() == Qt::red);
}

void Test_ATCAbstractBeacon::test_show()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    bcn.setSymbol(rect);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    bcn.setLabel(txt);

    bcn.show();
    QVERIFY(bcn.isVisible());
    QVERIFY(bcn.getSymbol()->isVisible());
    QVERIFY(bcn.getLabel()->isVisible());
}

void Test_ATCAbstractBeacon::test_hide()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    bcn.setSymbol(rect);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    bcn.setLabel(txt);

    bcn.hide();
    QVERIFY(!bcn.isVisible());
    QVERIFY(!bcn.getSymbol()->isVisible());
    QVERIFY(!bcn.getLabel()->isVisible());
}

void Test_ATCAbstractBeacon::test_isVisible()
{
    Mock_ATCAbstractBeacon bcn("BCN", 113.00, 20, 50);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    bcn.setSymbol(rect);

    QGraphicsSimpleTextItem *txt = new QGraphicsSimpleTextItem();
    bcn.setLabel(txt);

    bcn.show();
    QVERIFY(bcn.isVisible());

    bcn.hide();
    QVERIFY(!bcn.isVisible());
}
