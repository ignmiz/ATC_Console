#include "mainwindow.h"
#include "test_atcairport.h"

void Test_ATCAirport::test_constructObject_correct()
{
    ATCAirport airport("EPWA", 20, 50);
    QVERIFY(airport.getName() == "EPWA");
    QVERIFY(airport.latitude() == 20);
    QVERIFY(airport.longitude() == 50);
}

void Test_ATCAirport::test_constructObject_incorrectLat()
{
    ATCAirport airport("EPWA", -91, 50);
    QVERIFY(airport.getName() == "EPWA");
    QVERIFY(qIsNaN(airport.latitude()));
    QVERIFY(qIsNaN(airport.longitude()));

    airport = ATCAirport("EPWA", 91, 50);
    QVERIFY(airport.getName() == "EPWA");
    QVERIFY(qIsNaN(airport.latitude()));
    QVERIFY(qIsNaN(airport.longitude()));
}

void Test_ATCAirport::test_constructObject_incorrectLon()
{
    ATCAirport airport("EPWA", 20, -181);
    QVERIFY(airport.getName() == "EPWA");
    QVERIFY(qIsNaN(airport.latitude()));
    QVERIFY(qIsNaN(airport.longitude()));

    airport = ATCAirport("EPWA", 20, 181);
    QVERIFY(airport.getName() == "EPWA");
    QVERIFY(qIsNaN(airport.latitude()));
    QVERIFY(qIsNaN(airport.longitude()));
}

void Test_ATCAirport::test_deleteAllRunways()
{
    ATCAirport airport("EPWA", 20, 50);

    ATCRunway *runway1 = new ATCRunway("09", "27", 90, 270, 10, 20, 30, 40);
    airport.appendRunway(runway1);

    ATCRunway *runway2 = new ATCRunway("09", "27", 90, 270, 10, 20, 30, 40);
    airport.appendRunway(runway2);

    QVERIFY(airport.getRunwayVectorSize() == 2);

    airport.deleteAllRunways();

    QVERIFY(airport.getRunwayVectorSize() == 0);
}

void Test_ATCAirport::test_setGetName()
{
    ATCAirport airport("EPWA", 20, 50);

    airport.setName("EPPO");
    QVERIFY(airport.getName() == "EPPO");
}

void Test_ATCAirport::test_setGetScenePosition()
{
    ATCAirport airport("EPWA", 20, 50);

    airport.setScenePosition(new QPointF(10, 30));
    QVERIFY(airport.getScenePosition()->x() == 10);
    QVERIFY(airport.getScenePosition()->y() == 30);
}

void Test_ATCAirport::test_setGetSymbol()
{
    ATCAirport airport("EPWA", 20, 50);

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(5, 7, 20, 10);
    airport.setSymbol(ellipse);
    QVERIFY(airport.getSymbol()->rect().x() == 5);
    QVERIFY(airport.getSymbol()->rect().y() == 7);
    QVERIFY(airport.getSymbol()->rect().width() == 20);
    QVERIFY(airport.getSymbol()->rect().height() == 10);
}

void Test_ATCAirport::test_setColor()
{
    ATCAirport airport("EPWA", 20, 50);

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(5, 7, 20, 10);
    airport.setSymbol(ellipse);

    QColor color(Qt::red);
    airport.setColor(color);

    QVERIFY(airport.getSymbol()->pen().color() == color);
}

void Test_ATCAirport::test_isRunwayVectorEmpty()
{
    ATCAirport airport("EPWA", 20, 50);

    QVERIFY(airport.isRunwayVectorEmpty());

    ATCRunway *runway = new ATCRunway("09", "27", 90, 270, 10, 20, 30, 40);
    airport.appendRunway(runway);

    QVERIFY(!airport.isRunwayVectorEmpty());
}

void Test_ATCAirport::test_appendGetRunway_correct()
{
    ATCAirport airport("EPWA", 20, 50);

    ATCRunway *runway = new ATCRunway("09", "27", 90, 270, 10, 20, 30, 40);
    airport.appendRunway(runway);

    QVERIFY(airport.getRunwayVectorSize() == 1);
    QVERIFY(airport.getRunway(0)->getRunwayID1() == "09");
    QVERIFY(airport.getRunway(0)->getRunwayID2() == "27");
    QVERIFY(airport.getRunway(0)->getMagneticHDG1() == 90);
    QVERIFY(airport.getRunway(0)->getMagneticHDG2() == 270);
    QVERIFY(airport.getRunway(0)->getStartPoint().latitude() == 10);
    QVERIFY(airport.getRunway(0)->getStartPoint().longitude() == 20);
    QVERIFY(airport.getRunway(0)->getEndPoint().latitude() == 30);
    QVERIFY(airport.getRunway(0)->getEndPoint().longitude() == 40);
}

void Test_ATCAirport::test_show()
{
    ATCAirport airport("EPWA", 20, 50);

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(5, 7, 20, 10);
    airport.setSymbol(ellipse);

    airport.show();
    QVERIFY(airport.getSymbol()->isVisible());
}

void Test_ATCAirport::test_hide()
{
    ATCAirport airport("EPWA", 20, 50);

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(5, 7, 20, 10);
    airport.setSymbol(ellipse);

    airport.hide();
    QVERIFY(!airport.getSymbol()->isVisible());
}

void Test_ATCAirport::test_isVisible()
{
    ATCAirport airport("EPWA", 20, 50);

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(5, 7, 20, 10);
    airport.setSymbol(ellipse);

    airport.show();
    QVERIFY(airport.isVisible());

    airport.hide();
    QVERIFY(!airport.isVisible());
}
