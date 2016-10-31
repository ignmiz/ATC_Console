
#include "test_atcmath.h"

void Test_ATCMath::test_m2nm()
{
    QVERIFY(ATCMath::m2nm(1852) == 1);
}

void Test_ATCMath::test_nm2m()
{
    QVERIFY(ATCMath::nm2m(1) == 1852);
}

void Test_ATCMath::test_m2ft()
{
    QVERIFY(ATCMath::m2ft(1) == 2.9342723);
}

void Test_ATCMath::test_ft2m()
{
    QVERIFY(ATCMath::ft2m(1) == 0.3408);
}

void Test_ATCMath::test_mps2kt()
{
    QVERIFY(ATCMath::mps2kt(1) == 1.94384449);
}

void Test_ATCMath::test_kt2mps()
{
    QVERIFY(ATCMath::kt2mps(1) == 0.51444444);
}

void Test_ATCMath::test_tempISA()
{
    QVERIFY(ATCMath::tempISA(-2001) == -1);
    QVERIFY(ATCMath::tempISA(-2000) == 301.15);
    QVERIFY(ATCMath::tempISA(0) == 288.15);
    QVERIFY(ATCMath::tempISA(11000) == 216.65);
    QVERIFY(ATCMath::tempISA(20000) == 216.65);
    QVERIFY(ATCMath::tempISA(32000) == 228.65);
    QVERIFY(ATCMath::tempISA(47000) == 270.65);
    QVERIFY(ATCMath::tempISA(51000) == 270.65);
    QVERIFY(ATCMath::tempISA(71000) == 214.65);
    QVERIFY(ATCMath::tempISA(80000) == 196.65);

    QVERIFY(ATCMath::tempISA(-1000) == 294.65);
    QVERIFY(ATCMath::tempISA(4000) == 262.15);
    QVERIFY(ATCMath::tempISA(15000) == 216.65);
    QVERIFY(ATCMath::tempISA(25000) == 221.65);
    QVERIFY(ATCMath::tempISA(40000) == 251.05);
    QVERIFY(ATCMath::tempISA(50000) == 270.65);
    QVERIFY(ATCMath::tempISA(60000) == 245.45);
    QVERIFY(ATCMath::tempISA(75000) == 206.65);
}
