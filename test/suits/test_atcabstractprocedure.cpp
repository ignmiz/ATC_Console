
#include "test_atcabstractprocedure.h"

void Test_ATCAbstractProcedure::test_constructObject()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QVERIFY(foo.getName() == "NAME");
    QVERIFY(foo.getAirport() == "CODE");
    QVERIFY(foo.getRunwayID() == "01");
    QVERIFY(foo.getFixListSize() == 0);
}

void Test_ATCAbstractProcedure::test_getName()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QVERIFY(foo.getName() == "NAME");
}

void Test_ATCAbstractProcedure::test_setName()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    foo.setName("TEST");
    QVERIFY(foo.getName() == "TEST");
}

void Test_ATCAbstractProcedure::test_getAirport()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QVERIFY(foo.getAirport() == "CODE");
}

void Test_ATCAbstractProcedure::test_setAirport()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    foo.setAirport("TEST");
    QVERIFY(foo.getAirport() == "TEST");
}

void Test_ATCAbstractProcedure::test_getRunwayID()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QVERIFY(foo.getRunwayID() == "01");
}

void Test_ATCAbstractProcedure::test_setRunwayID()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    foo.setRunwayID("19");
    QVERIFY(foo.getRunwayID() == "19");
}

void Test_ATCAbstractProcedure::test_appendGetFixName()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    foo.appendFixName("FIX1");
    foo.appendFixName("FIX2");

    QVERIFY(foo.getFixName(0) == "FIX1");
    QVERIFY(foo.getFixName(1) == "FIX2");
}

void Test_ATCAbstractProcedure::test_setFixList()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QStringList list;
    list << "FIX1" << "FIX2";

    foo.setFixList(list);

    QVERIFY(foo.getFixName(0) == "FIX1");
    QVERIFY(foo.getFixName(1) == "FIX2");
}

void Test_ATCAbstractProcedure::test_getFixListSize()
{
    Mock_ATCAbstractProcedure foo("NAME", "CODE", "01");
    QVERIFY(foo.getFixListSize() == 0);

    QStringList list;
    list << "FIX1" << "FIX2";

    foo.setFixList(list);

    QVERIFY(foo.getFixListSize() == 2);
}
