
#include "test_atccompany.h"


void Test_ATCCompany::test_constructObject()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");

    QVERIFY(foo.getCode() == "TST");
    QVERIFY(foo.getCallsign() == "CALLSIGN");
    QVERIFY(foo.getName() == "Full Airline Name");
}

void Test_ATCCompany::test_getCode()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");
    QVERIFY(foo.getCode() == "TST");
}

void Test_ATCCompany::test_getCallsign()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");
    QVERIFY(foo.getCallsign() == "CALLSIGN");
}

void Test_ATCCompany::test_getName()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");
    QVERIFY(foo.getName() == "Full Airline Name");
}

void Test_ATCCompany::test_setCode()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");

    foo.setCode("ABC");
    QVERIFY(foo.getCode() == "ABC");

    foo.setCode("KSFGA");
    QVERIFY(foo.getCode() == "KSF");

    foo.setCode("abc");
    QVERIFY(foo.getCode() == "ABC");

    foo.setCode("aBcDe");
    QVERIFY(foo.getCode() == "ABC");
}

void Test_ATCCompany::test_setCallsign()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");

    foo.setCallsign("NEWCALL");
    QVERIFY(foo.getCallsign() == "NEWCALL");

    foo.setCallsign("nEwcAll");
    QVERIFY(foo.getCallsign() == "NEWCALL");
}

void Test_ATCCompany::test_setName()
{
    ATCCompany foo("TST", "CALLSIGN", "Full Airline Name");

    foo.setName("New Name");
    QVERIFY(foo.getName() == "New Name");
}
