
#include "test_atcairspacefix.h"

void Test_ATCAirspaceFix::test_constructObject_correct()
{
    ATCAirspaceFix fix(20, 50);
    QVERIFY(fix.latitude() == 20);
    QVERIFY(fix.longitude() == 50);
}

void Test_ATCAirspaceFix::test_constructObject_incorrectLat()
{
    ATCAirspaceFix fix(-91, 50);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = ATCAirspaceFix(91, 50);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}

void Test_ATCAirspaceFix::test_constructObject_incorrectLon()
{
    ATCAirspaceFix fix(20, -181);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = ATCAirspaceFix(20, 181);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}
