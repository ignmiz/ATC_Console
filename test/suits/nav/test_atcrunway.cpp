
#include "test_atcrunway.h"

void Test_ATCRunway::test_constructObject_correct()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);
}

void Test_ATCRunway::test_constructObject_incorrectLat()
{
    ATCRunway foo("18", "36", 180, 360, 91, 20, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(qIsNaN(foo.getStartPoint().latitude()));
    QVERIFY(qIsNaN(foo.getStartPoint().longitude()));
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, -91, 20, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(qIsNaN(foo.getStartPoint().latitude()));
    QVERIFY(qIsNaN(foo.getStartPoint().longitude()));
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, 10, 20, 91, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
    QVERIFY(qIsNaN(foo.getEndPoint().latitude()));
    QVERIFY(qIsNaN(foo.getEndPoint().longitude()));
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, 10, 20, -91, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
    QVERIFY(qIsNaN(foo.getEndPoint().latitude()));
    QVERIFY(qIsNaN(foo.getEndPoint().longitude()));
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);
}

void Test_ATCRunway::test_constructObject_incorrectLon()
{
    ATCRunway foo("18", "36", 180, 360, 10, 181, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(qIsNaN(foo.getStartPoint().latitude()));
    QVERIFY(qIsNaN(foo.getStartPoint().longitude()));
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, 10, -181, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(qIsNaN(foo.getStartPoint().latitude()));
    QVERIFY(qIsNaN(foo.getStartPoint().longitude()));
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, 10, 20, 30, 181);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
    QVERIFY(qIsNaN(foo.getEndPoint().latitude()));
    QVERIFY(qIsNaN(foo.getEndPoint().longitude()));
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);

    foo = ATCRunway("18", "36", 180, 360, 10, 20, 30, -181);
    QVERIFY(foo.getRunwayID1() == "18");
    QVERIFY(foo.getRunwayID2() == "36");
    QVERIFY(foo.getMagneticHDG1() == 180);
    QVERIFY(foo.getMagneticHDG2() == 360);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
    QVERIFY(qIsNaN(foo.getEndPoint().latitude()));
    QVERIFY(qIsNaN(foo.getEndPoint().longitude()));
    QVERIFY(foo.getExtendedCentreline1() == nullptr);
    QVERIFY(foo.getExtendedCentreline2() == nullptr);
}

void Test_ATCRunway::test_getRunway1()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getRunwayID1() == "18");
}

void Test_ATCRunway::test_getRunway2()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getRunwayID2() == "36");
}

void Test_ATCRunway::test_getMagneticHDG1()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getMagneticHDG1() == 180);
}

void Test_ATCRunway::test_getMagneticHDG2()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getMagneticHDG2() == 360);
}

void Test_ATCRunway::test_getStartPoint()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getStartPoint().latitude() == 10);
    QVERIFY(foo.getStartPoint().longitude() == 20);
}

void Test_ATCRunway::test_getEndPoint()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);
    QVERIFY(foo.getEndPoint().latitude() == 30);
    QVERIFY(foo.getEndPoint().longitude() == 40);
}

void Test_ATCRunway::test_setGetExtendedCentreline1()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);

    ATCRunwayExtendedCentreline *line = new ATCRunwayExtendedCentreline(new QGraphicsLineItem());
    foo.setExtendedCentreline1(line);
    QVERIFY(foo.getExtendedCentreline1() == line);
}

void Test_ATCRunway::test_setGetExtendedCentreline2()
{
    ATCRunway foo("18", "36", 180, 360, 10, 20, 30, 40);

    ATCRunwayExtendedCentreline *line = new ATCRunwayExtendedCentreline(new QGraphicsLineItem());
    foo.setExtendedCentreline2(line);
    QVERIFY(foo.getExtendedCentreline2() == line);
}
