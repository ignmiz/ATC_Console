
#include "test_atcabstractfix.h"

void Test_ATCAbstractFix::test_constructObject_correct()
{
    Mock_ATCAbstractFix fix(20, 50);
    QVERIFY(fix.latitude() == 20);
    QVERIFY(fix.longitude() == 50);
}

void Test_ATCAbstractFix::test_constructObject_incorrectLat()
{
    Mock_ATCAbstractFix fix(-91, 50);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = Mock_ATCAbstractFix(91, 50);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}

void Test_ATCAbstractFix::test_constructObject_incorrectLon()
{
    Mock_ATCAbstractFix fix(20, -181);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));

    fix = Mock_ATCAbstractFix(20, 181);
    QVERIFY(qIsNaN(fix.latitude()));
    QVERIFY(qIsNaN(fix.longitude()));
}
