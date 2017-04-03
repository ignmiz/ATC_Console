
#include "test_atcmath.h"

void Test_ATCMath::test_rad2deg()
{
    QVERIFY(ATCMath::rad2deg(ATCConst::PI/2) == ATCConst::PI/2 * 180/ATCConst::PI);
}

void Test_ATCMath::test_deg2rad()
{
    QVERIFY(ATCMath::deg2rad(45) == 45 * ATCConst::PI/180);
}

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
    QVERIFY(ATCMath::m2ft(1) == (1/0.3048));
}

void Test_ATCMath::test_ft2m()
{
    QVERIFY(ATCMath::ft2m(1) == 0.3048);
}

void Test_ATCMath::test_mps2kt()
{
    QVERIFY(ATCMath::mps2kt(1) == 1.94384449);
}

void Test_ATCMath::test_kt2mps()
{
    QVERIFY(ATCMath::kt2mps(1) == 0.51444444);
}

void Test_ATCMath::test_cas2tas()
{
    double cas = ATCMath::kt2mps(250);
    double error = 1E-4;

    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(1000).p, ATCMath::atmosISA(1000).rho), ATCMath::kt2mps(261.8680), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(2000).p, ATCMath::atmosISA(2000).rho), ATCMath::kt2mps(274.5248), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(3000).p, ATCMath::atmosISA(3000).rho), ATCMath::kt2mps(288.0309), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(4000).p, ATCMath::atmosISA(4000).rho), ATCMath::kt2mps(302.4505), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(5000).p, ATCMath::atmosISA(5000).rho), ATCMath::kt2mps(317.8517), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(6000).p, ATCMath::atmosISA(6000).rho), ATCMath::kt2mps(334.3056), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(7000).p, ATCMath::atmosISA(7000).rho), ATCMath::kt2mps(351.8859), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(8000).p, ATCMath::atmosISA(8000).rho), ATCMath::kt2mps(370.6681), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(9000).p, ATCMath::atmosISA(9000).rho), ATCMath::kt2mps(390.7281), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(10000).p, ATCMath::atmosISA(10000).rho), ATCMath::kt2mps(412.1409), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(11000).p, ATCMath::atmosISA(11000).rho), ATCMath::kt2mps(434.9783), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(12000).p, ATCMath::atmosISA(12000).rho), ATCMath::kt2mps(465.8702), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(13000).p, ATCMath::atmosISA(13000).rho), ATCMath::kt2mps(498.3457), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(14000).p, ATCMath::atmosISA(14000).rho), ATCMath::kt2mps(532.3885), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::cas2tas(cas, ATCMath::atmosISA(15000).p, ATCMath::atmosISA(15000).rho), ATCMath::kt2mps(567.9692), error));
}

void Test_ATCMath::test_cas2mach()
{
    double cas = ATCMath::kt2mps(250);
    double error = 1E-5;

    ISA isa = ATCMath::atmosISA(0);

    QVERIFY(ATCMath::compareDouble(ATCMath::cas2mach(cas, isa.p, isa.rho, isa.a), 0.37794, error));
}

void Test_ATCMath::test_tas2cas()
{
    double tas = ATCMath::kt2mps(300);
    double error = 1E-4;

    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho), ATCMath::kt2mps(300), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(1000).p, ATCMath::atmosISA(1000).rho), ATCMath::kt2mps(286.5878), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(2000).p, ATCMath::atmosISA(2000).rho), ATCMath::kt2mps(273.4342), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(3000).p, ATCMath::atmosISA(3000).rho), ATCMath::kt2mps(260.5468), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(4000).p, ATCMath::atmosISA(4000).rho), ATCMath::kt2mps(247.9329), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(5000).p, ATCMath::atmosISA(5000).rho), ATCMath::kt2mps(235.6003), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(6000).p, ATCMath::atmosISA(6000).rho), ATCMath::kt2mps(223.5536), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(7000).p, ATCMath::atmosISA(7000).rho), ATCMath::kt2mps(211.7989), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(8000).p, ATCMath::atmosISA(8000).rho), ATCMath::kt2mps(200.3430), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(9000).p, ATCMath::atmosISA(9000).rho), ATCMath::kt2mps(189.1905), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(10000).p, ATCMath::atmosISA(10000).rho), ATCMath::kt2mps(178.3447), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(11000).p, ATCMath::atmosISA(11000).rho), ATCMath::kt2mps(167.8101), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(12000).p, ATCMath::atmosISA(12000).rho), ATCMath::kt2mps(155.2665), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(13000).p, ATCMath::atmosISA(13000).rho), ATCMath::kt2mps(143.6370), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(14000).p, ATCMath::atmosISA(14000).rho), ATCMath::kt2mps(132.8599), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::tas2cas(tas, ATCMath::atmosISA(15000).p, ATCMath::atmosISA(15000).rho), ATCMath::kt2mps(122.8765), error));
}

void Test_ATCMath::test_tas2mach()
{
    double tas = ATCMath::kt2mps(250);
    double error = 1E-5;

    QVERIFY(ATCMath::compareDouble(ATCMath::tas2mach(tas, ATCMath::atmosISA(0).a), 0.37794, error));
}

void Test_ATCMath::test_mach2tas()
{
    double mach = 0.5;
    double a = ATCMath::atmosISA(0).a;
    double error = 1E-5;

    QVERIFY(ATCMath::compareDouble(ATCMath::mach2tas(mach, a), 170.147, error));
}

void Test_ATCMath::test_mach2cas()
{
    double mach = 0.5;
    ISA isa = ATCMath::atmosISA(0);
    double error = 1E-5;

    QVERIFY(ATCMath::compareDouble(ATCMath::mach2cas(mach, isa.p, isa.rho, isa.a), 170.147, error));
}

void Test_ATCMath::test_compareDouble()
{
    QVERIFY(ATCMath::compareDouble(1.0001, 1, 1E-2));
    QVERIFY(!ATCMath::compareDouble(1.1, 1, 1E-2));
    QVERIFY(ATCMath::compareDouble(-1.001, -1, 1E-2));
    QVERIFY(!ATCMath::compareDouble(-1.1, 1, 1E-2));
    QVERIFY(!ATCMath::compareDouble(1, -1, 1E-1));
    QVERIFY(!ATCMath::compareDouble(-1, 1, 1E-1));
}

void Test_ATCMath::test_min()
{
    QVERIFY(ATCMath::min(3, 5) == 3);
    QVERIFY(ATCMath::min(5, 3) == 3);
}

void Test_ATCMath::test_max()
{
    QVERIFY(ATCMath::max(3, 5) == 5);
    QVERIFY(ATCMath::max(5, 3) == 5);
}

void Test_ATCMath::test_atmosISA()
{
    double errorA = 1E-5;
    double errorP = 1E-5;
    double errorRHO = 1E-5;

    QVERIFY(ATCMath::atmosISA(-2001).T == -1);
    QVERIFY(ATCMath::atmosISA(-2000).T == 301.15);
    QVERIFY(ATCMath::atmosISA(0).T == 288.15);
    QVERIFY(ATCMath::atmosISA(11000).T == 216.65);
    QVERIFY(ATCMath::atmosISA(20000).T == 216.65);
    QVERIFY(ATCMath::atmosISA(32000).T == 228.65);
    QVERIFY(ATCMath::atmosISA(47000).T == 270.65);
    QVERIFY(ATCMath::atmosISA(51000).T == 270.65);
    QVERIFY(ATCMath::atmosISA(71000).T == 214.65);
    QVERIFY(ATCMath::atmosISA(80000).T == 196.65);
    QVERIFY(ATCMath::atmosISA(80001).T == -1);

    QVERIFY(ATCMath::atmosISA(-1000).T == 294.65);
    QVERIFY(ATCMath::atmosISA(4000).T == 262.15);
    QVERIFY(ATCMath::atmosISA(15000).T == 216.65);
    QVERIFY(ATCMath::atmosISA(25000).T == 221.65);
    QVERIFY(ATCMath::atmosISA(40000).T == 251.05);
    QVERIFY(ATCMath::atmosISA(50000).T == 270.65);
    QVERIFY(ATCMath::atmosISA(60000).T == 245.45);
    QVERIFY(ATCMath::atmosISA(75000).T == 206.65);

    QVERIFY(ATCMath::atmosISA(-2001).a == -1);
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-2000).a, 347.8855566, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(0).a, 340.293988, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(11000).a, 295.0694935, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(20000).a, 295.0694935, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(32000).a, 303.1311502, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(47000).a, 329.798731, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(51000).a, 329.798731, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(71000).a, 293.7043717, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(80000).a, 281.1201267, errorA));
    QVERIFY(ATCMath::atmosISA(80001).a == -1);

    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-1000).a, 344.1107081, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(4000).a, 324.5786096, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(15000).a, 295.0694935, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(25000).a, 298.4549817, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(40000).a, 317.6326057, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(50000).a, 329.798731, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(60000).a, 314.0700204, errorA));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(75000).a, 288.1792252, errorA));

    QVERIFY(ATCMath::atmosISA(-2001).p == -1);
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-2000).p, 127773.73, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(0).p, 101325, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(11000).p, 22632.1, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(20000).p, 5474.89, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(32000).p, 868.019, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(47000).p, 110.906, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(51000).p, 66.9389, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(71000).p, 3.95642, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(80000).p, 0.88627847, errorP));
    QVERIFY(ATCMath::atmosISA(80001).p == -1);

    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-1000).p, 113929, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(4000).p, 61640.2, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(15000).p, 12044.6, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(25000).p, 2511.02, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(40000).p, 277.522, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(50000).p, 75.9448, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(60000).p, 20.3143, errorP));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(75000).p, 2.06792, errorP));

    QVERIFY(ATCMath::atmosISA(-2001).rho == -1);
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-2000).rho, 1.47808, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(0).rho, 1.22500, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(11000).rho, 0.363918, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(20000).rho, 0.0880349, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(32000).rho, 0.0132250, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(47000).rho, 0.00142753, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(51000).rho, 0.000861606, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(71000).rho, 0.0000642110, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(80000).rho, 0.0000157005, errorRHO));
    QVERIFY(ATCMath::atmosISA(80001).rho == -1);

    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(-1000).rho, 1.34700, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(4000).rho, 0.819129, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(15000).rho, 0.193674, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(25000).rho, 0.0394658, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(40000).rho, 0.00385101, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(50000).rho, 0.000977525, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(60000).rho, 0.000288321, errorRHO));
    QVERIFY(ATCMath::compareDouble(ATCMath::atmosISA(75000).rho, 0.0000348607, errorRHO));
}

void Test_ATCMath::test_crossoverAltitude()
{
    double error = 1E-5;
    QVERIFY(ATCMath::compareDouble(ATCMath::crossoverAltitude(ATCMath::kt2mps(280), 0.78), 9895.14388, error));
}

void Test_ATCMath::test_normalizeAngle()
{
    double angle = 540;
    QVERIFY(ATCMath::normalizeAngle(angle, ATC::Deg) == 180);

    angle = -400;
    QVERIFY(ATCMath::normalizeAngle(angle, ATC::Deg) == 320);

    double error = 1E-8;

    angle = ATCMath::deg2rad(540);
    QVERIFY(ATCMath::compareDouble(ATCMath::normalizeAngle(angle, ATC::Rad), ATCMath::deg2rad(180), error));

    angle = ATCMath::deg2rad(-400);
    QVERIFY(ATCMath::compareDouble(ATCMath::normalizeAngle(angle, ATC::Rad), ATCMath::deg2rad(320), error));
}

void Test_ATCMath::test_ESF()
{
    double temp = ATCMath::atmosISA(0).T;
    double error = 1e-8;

    //Level
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Constant, BADA::CAS, BADA::Low, 0.8, temp, 0) == 0);
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Constant, BADA::CAS, BADA::High, 0.8, temp, 0) == 0);
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Constant, BADA::Mach, BADA::Low, 0.8, temp, 0) == 0);
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Constant, BADA::Mach, BADA::High, 0.8, temp, 0) == 0);
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Decelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 0);
    QVERIFY(ATCMath::ESF(BADA::Level, BADA::Accelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 0);

    //Descend
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Descend, BADA::Constant, BADA::CAS, BADA::Low, 0.8, temp, 0), 0.767595351517540, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Descend, BADA::Constant, BADA::CAS, BADA::High, 0.8, temp, 0), 0.720457142712908, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Descend, BADA::Constant, BADA::Mach, BADA::Low, 0.8, temp, 0), 1.093180365868351, error));
    QVERIFY(ATCMath::ESF(BADA::Descend, BADA::Constant, BADA::Mach, BADA::High, 0.8, temp, 0) == 1);
    QVERIFY(ATCMath::ESF(BADA::Descend, BADA::Decelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 0.3);
    QVERIFY(ATCMath::ESF(BADA::Descend, BADA::Accelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 1.7);

    //Climb
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Climb, BADA::Constant, BADA::CAS, BADA::Low, 0.8, temp, 0), 0.767595351517540, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Climb, BADA::Constant, BADA::CAS, BADA::High, 0.8, temp, 0), 0.720457142712908, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ESF(BADA::Climb, BADA::Constant, BADA::Mach, BADA::Low, 0.8, temp, 0), 1.093180365868351, error));
    QVERIFY(ATCMath::ESF(BADA::Climb, BADA::Constant, BADA::Mach, BADA::High, 0.8, temp, 0) == 1);
    QVERIFY(ATCMath::ESF(BADA::Climb, BADA::Decelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 1.7);
    QVERIFY(ATCMath::ESF(BADA::Climb, BADA::Accelerate, BADA::Mach, BADA::Low, 0.8, temp, 0) == 0.3);
}

void Test_ATCMath::test_pathAngle()
{
    QVERIFY(ATCMath::compareDouble(ATCMath::pathAngle(2000, 1500, 1.7, 100), 1.048666174, 1e-8));
    QVERIFY(ATCMath::compareDouble(ATCMath::pathAngle(2000, 1500, 0, 100), 0, 1e-8));
}

void Test_ATCMath::test_bankAngle()
{
    QVERIFY(ATCMath::bankAngle(1, -20, 1, 0.01, 20) == 0.8);
    QVERIFY(ATCMath::bankAngle(1, -20, 1, 20, 20) == -20);
    QVERIFY(ATCMath::bankAngle(1, -20, 1, -20, 20) == 20);
}

void Test_ATCMath::test_DTA()
{
    double error = 1e-8;

    double v = 100;
    double bankLimit = ATCMath::deg2rad(30);
    double dHdg = ATCMath::deg2rad(90);
    double flyOverDst = 100;

    QVERIFY(ATCMath::compareDouble(ATCMath::DTA(v, bankLimit, dHdg, flyOverDst), 1766.20029, error));

    dHdg = ATCMath::deg2rad(-90);
    QVERIFY(ATCMath::compareDouble(ATCMath::DTA(v, bankLimit, dHdg, flyOverDst), 1766.20029, error));

    dHdg = ATCMath::deg2rad(160);
    QVERIFY(ATCMath::compareDouble(ATCMath::DTA(v, bankLimit, dHdg, flyOverDst), flyOverDst, error));

    dHdg = ATCMath::deg2rad(0);
    QVERIFY(ATCMath::compareDouble(ATCMath::DTA(v, bankLimit, dHdg, flyOverDst), 0, error));
}

void Test_ATCMath::test_projectAcftPosOnPath()
{
    double error = 1e-8;

    GeographicLib::Geodesic wgs = GeographicLib::Geodesic::WGS84();

    double fix1Lat = 0;
    double fix1Lon = 0;

    double fix2Lat = 0;
    double fix2Lon = 90;

    double acftLat = 0;
    double acftLon = 0;

    double heading = ATCMath::deg2rad(90);

    double xtrackError;
    double hdgError;
    double dstToNext;

    ATCMath::projectAcftPosOnPath(wgs, fix1Lat, fix1Lon, fix2Lat, fix2Lon, acftLat, acftLon, heading, xtrackError, hdgError, dstToNext);
    QVERIFY(xtrackError == 0);
    QVERIFY(hdgError == 0);
    QVERIFY(ATCMath::compareDouble(dstToNext, 10018754.17, error));

    acftLon = ATCMath::deg2rad(45);
    heading = ATCMath::deg2rad(360);

    ATCMath::projectAcftPosOnPath(wgs, fix1Lat, fix1Lon, fix2Lat, fix2Lon, acftLat, acftLon, heading, xtrackError, hdgError, dstToNext);
    QVERIFY(xtrackError == 0);
    QVERIFY(ATCMath::compareDouble(hdgError, ATCMath::deg2rad(-90), error));
    QVERIFY(ATCMath::compareDouble(dstToNext, 5009377.086, error));

    acftLon = ATCMath::deg2rad(90);
    heading = ATCMath::deg2rad(180);

    ATCMath::projectAcftPosOnPath(wgs, fix1Lat, fix1Lon, fix2Lat, fix2Lon, acftLat, acftLon, heading, xtrackError, hdgError, dstToNext);
    QVERIFY(xtrackError == 0);
    QVERIFY(ATCMath::compareDouble(hdgError, ATCMath::deg2rad(90), error));
    QVERIFY(ATCMath::compareDouble(dstToNext, 0, error));

    fix1Lat = -1;
    fix1Lon = 0;

    fix2Lat = 1;
    fix2Lon = 0;

    acftLat = ATCMath::deg2rad(0);
    acftLon = ATCMath::deg2rad(30);

    heading = ATCMath::deg2rad(360);

    ATCMath::projectAcftPosOnPath(wgs, fix1Lat, fix1Lon, fix2Lat, fix2Lon, acftLat, acftLon, heading, xtrackError, hdgError, dstToNext);
    QVERIFY(ATCMath::compareDouble(xtrackError, -3339584.724, 1e-5));
    QVERIFY(ATCMath::compareDouble(hdgError, ATCMath::deg2rad(0), error));

    heading = ATCMath::deg2rad(45);

    ATCMath::projectAcftPosOnPath(wgs, fix1Lat, fix1Lon, fix2Lat, fix2Lon, acftLat, acftLon, heading, xtrackError, hdgError, dstToNext);
    QVERIFY(ATCMath::compareDouble(xtrackError, -3339584.724, 1e-5));
    QVERIFY(ATCMath::compareDouble(hdgError, ATCMath::deg2rad(45), error));
}

void Test_ATCMath::test_sphericalRhumbIntersection()
{
    double error = 2 * 1e-3;

    GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();

    double thrLat = 52;
    double thrLon = 20;
    double azimuth = ATCMath::deg2rad(360);

    double acftLat = ATCMath::deg2rad(51.3);
    double acftLon = ATCMath::deg2rad(20.2);
    double acftHdg = ATCMath::deg2rad(270);

    double dstAcftToIntersect;
    double dstThrToIntersect;

    ATCMath::sphericalRhumbIntersection(geo, thrLat, thrLon, azimuth, acftLat, acftLon, acftHdg, dstAcftToIntersect, dstThrToIntersect);

    double checkDstAcftToIntersect;
    double checkDstThrToIntersect;

    double irrelevantAzimuth;

    GeographicLib::Rhumb::WGS84().Inverse(51.3, 20.2, 51.3, 20, checkDstAcftToIntersect, irrelevantAzimuth);
    GeographicLib::Rhumb::WGS84().Inverse(thrLat, thrLon, 51.3, 20, checkDstThrToIntersect, irrelevantAzimuth);

    QVERIFY(ATCMath::compareDouble(dstAcftToIntersect, checkDstAcftToIntersect, error));
    QVERIFY(ATCMath::compareDouble(dstThrToIntersect, checkDstThrToIntersect, error));
}

void Test_ATCMath::test_normalizeHdgError()
{
    double error = 1e-8;
    double hdg = ATCMath::deg2rad(30);

    hdg = ATCMath::normalizeHdgChange(hdg);
    QVERIFY(ATCMath::compareDouble(hdg, ATCMath::deg2rad(30), error));

    hdg = ATCMath::deg2rad(210);
    hdg = ATCMath::normalizeHdgChange(hdg);
    QVERIFY(ATCMath::compareDouble(hdg, ATCMath::deg2rad(-150), error));

    hdg = ATCMath::deg2rad(-270);
    hdg = ATCMath::normalizeHdgChange(hdg);
    QVERIFY(ATCMath::compareDouble(hdg, ATCMath::deg2rad(90), error));
}

void Test_ATCMath::test_randomMass()
{
    ATCPaths paths;

    ATCAircraftType test(paths.OPF_TEST_PATH, paths.APF_TEST_PATH);
    double mRef = test.getMass().ref;
    double mMin = qFloor(1.2 * test.getMass().min);

    for(int i = 0; i < 25000; i++)
    {
        double mRand = ATCMath::randomMass(mMin, mRef);

        QVERIFY(mRand >= mMin);
        QVERIFY(mRand <= mRef);
    }
}

void Test_ATCMath::test_recalculateSpeed()
{
    QVERIFY(ATCMath::recalculateSpeed(100, 8, 2) == 200);
}

void Test_ATCMath::test_recalculateReductionFactor()
{
    QVERIFY(ATCMath::recalculateReductionFactor(0, 100, 50, 150) == 1);
    QVERIFY(ATCMath::recalculateReductionFactor(0.5, 100, 50, 150) == 0.75);
}

void Test_ATCMath::test_nominalSpeedCL()
{
    double a =  ATCMath::atmosISA(0).a;
    double rho = ATCMath::atmosISA(0).rho;
    double p = ATCMath::atmosISA(0).p;

    double CVmin = ATCConst::C_V_MIN;

    double error = 1e-8;

    //Jet
    QVERIFY(ATCMath::nominalSpeedCL(12000, BADA::Mach, ATC::Jet, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(5000, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3048, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Jet, 120, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1828.8, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1600, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL5), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1524, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL5), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1500, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1219.2, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(1000, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(914.4, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(500, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(457.2, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(300, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL1), error));

    //Turboprop
    QVERIFY(ATCMath::nominalSpeedCL(12000, BADA::Mach, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(5000, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3048, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Turboprop, 120, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(457.2, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(400, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL8), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(304.8, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL8), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(250, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(152.4, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(100, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL6), error));

    //Piston
    QVERIFY(ATCMath::nominalSpeedCL(12000, BADA::Mach, ATC::Piston, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(5000, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3048, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(3000, BADA::CAS, ATC::Piston, 120, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(457.2, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(400, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL8), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(304.8, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL8), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(250, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(152.4, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCL(100, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_CL6), error));
}

void Test_ATCMath::test_nominalSpeedCR()
{
    double a =  ATCMath::atmosISA(0).a;
    double rho = ATCMath::atmosISA(0).rho;
    double p = ATCMath::atmosISA(0).p;

    double error = 1e-8;

    //Jet
    QVERIFY(ATCMath::nominalSpeedCR(12000, BADA::Mach, ATC::Jet, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(5000, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(4267.2, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Jet, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1828.8, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Jet, 200, 280, 0.8, a, rho, p), ATCMath::kt2mps(200), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(914.4, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Jet, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(170), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Jet, 150, 280, 0.8, a, rho, p), ATCMath::kt2mps(150), error));

    //Turboprop
    QVERIFY(ATCMath::nominalSpeedCR(12000, BADA::Mach, ATC::Turboprop, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(5000, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3048, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Turboprop, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1828.8, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(180), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Turboprop, 160, 280, 0.8, a, rho, p), ATCMath::kt2mps(160), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(914.4, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(180), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Turboprop, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(150), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Turboprop, 130, 280, 0.8, a, rho, p), ATCMath::kt2mps(130), error));

    //Piston
    QVERIFY(ATCMath::nominalSpeedCR(12000, BADA::Mach, ATC::Piston, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(5000, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3048, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(3000, BADA::CAS, ATC::Piston, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1828.8, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(180), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(1500, BADA::CAS, ATC::Piston, 160, 280, 0.8, a, rho, p), ATCMath::kt2mps(160), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(914.4, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(180), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Piston, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(150), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedCR(500, BADA::CAS, ATC::Piston, 130, 280, 0.8, a, rho, p), ATCMath::kt2mps(130), error));
}

void Test_ATCMath::test_nominalSpeedDS()
{
    double a =  ATCMath::atmosISA(0).a;
    double rho = ATCMath::atmosISA(0).rho;
    double p = ATCMath::atmosISA(0).p;

    double CVmin = ATCConst::C_V_MIN;

    double error = 1e-8;

    //Jet
    QVERIFY(ATCMath::nominalSpeedDS(12000, BADA::Mach, ATC::Jet, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(5000, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3048, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3000, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3000, BADA::CAS, ATC::Jet, 120, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1828.8, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1500, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1500, BADA::CAS, ATC::Jet, 120, 200, 280, 0.8, a, rho, p), ATCMath::kt2mps(200), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(914.4, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(750, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(609.6, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(500, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(457.2, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(400, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(304.8, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(200, BADA::CAS, ATC::Jet, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES1), error));

    //Turboprop
    QVERIFY(ATCMath::nominalSpeedDS(12000, BADA::Mach, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(5000, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3048, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3000, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3000, BADA::CAS, ATC::Turboprop, 120, 240, 280, 0.8, a, rho, p), ATCMath::kt2mps(240), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1828.8, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(250), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1500, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(1500, BADA::CAS, ATC::Turboprop, 120, 200, 280, 0.8, a, rho, p), ATCMath::kt2mps(200), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(914.4, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(220), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(750, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(609.6, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES4), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(500, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(457.2, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES3), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(400, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(304.8, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(200, BADA::CAS, ATC::Turboprop, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES1), error));

    //Piston
    QVERIFY(ATCMath::nominalSpeedDS(12000, BADA::Mach, ATC::Piston, 120, 260, 280, 0.8, a, rho, p) == 0.8 * a);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(5000, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3048, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(280), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(3000, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(260), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(457.2, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(260), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(400, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(304.8, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES7), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(250, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES6), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(152.4, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES6), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeedDS(100, BADA::CAS, ATC::Piston, 120, 260, 280, 0.8, a, rho, p), ATCMath::kt2mps(CVmin * 120 + ATCConst::V_DES5), error));
}

void Test_ATCMath::test_nominalSpeed2tas()
{
    QVERIFY(ATCMath::nominalSpeed2tas(0.8, BADA::Mach, 300, 1, 101325) == 240);
    QVERIFY(ATCMath::compareDouble(ATCMath::nominalSpeed2tas(200, BADA::CAS, ATCMath::atmosISA(0).a, ATCMath::atmosISA(0).rho, ATCMath::atmosISA(0).p), ATCMath::kt2mps(200), 10e-8));

}

void Test_ATCMath::test_liftCoefficient()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::liftCoefficient(100, 1.5, 20, 100, 0), 0.032688833333333, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::liftCoefficient(100, 1.5, 20, 100, ATCMath::deg2rad(30)), 0.037745813448990, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::liftCoefficient(100, 1.5, 20, 100, ATCMath::deg2rad(-30)), 0.037745813448990, error));
}

void Test_ATCMath::test_dragCoefficient()
{
    QVERIFY(ATCMath::dragCoefficient(0.5, 1, 2, 0) == 1.5);
    QVERIFY(ATCMath::dragCoefficient(0.5, 1, 2, 0.5) == 2);
}

void Test_ATCMath::test_lift()
{
    QVERIFY(ATCMath::lift(0.5, 20, 100, 0.75) == 7500);
}

void Test_ATCMath::test_drag()
{
    QVERIFY(ATCMath::lift(0.5, 20, 100, 0.04) == 400);
}

void Test_ATCMath::test_thrust()
{
    double error = 1e-8;

    //Jet
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::Off, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 397758017.7, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::On, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 159097207.1 , error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Descend, BADA::Accelerate, BADA::On, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 39775801.77, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Decelerate, BADA::On, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 39775801.77, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Accelerate, BADA::On, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 377870116.8, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Constant, BADA::On, ATC::Jet, 10000, 2, 0.001, 0.1, 0.4), 10000, error));

    //Turboprop
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::Off, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), -168729.5405, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::On, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), -73491.81619, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Descend, BADA::Accelerate, BADA::On, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), -16872.95405, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Decelerate, BADA::On, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), -16872.95405, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Accelerate, BADA::On, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), -160293.0635, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Constant, BADA::On, ATC::Turboprop, 10000, 2, 0.001, 0.1, 0.4), 10000, error));

    //Piston
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::Off, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), -32798398.95, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Climb, BADA::Accelerate, BADA::On, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), -13125359.58, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Descend, BADA::Accelerate, BADA::On, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), -3279839.895, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Decelerate, BADA::On, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), -3279839.895, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Accelerate, BADA::On, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), -31158479, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::thrust(100, 2000, 10000, BADA::Level, BADA::Constant, BADA::On, ATC::Piston, 10000, 2, 0.001, 0.1, 0.4), 10000, error));
}

void Test_ATCMath::test_ellipsoidRadius()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(0, ATCMath::deg2rad(0)), ATCConst::WGS84_A, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(2500, ATCMath::deg2rad(0)), ATCConst::WGS84_A + 2500, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(0, ATCMath::deg2rad(90)), ATCConst::WGS84_B, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(2500, ATCMath::deg2rad(90)), ATCConst::WGS84_B + 2500, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(0, ATCMath::deg2rad(30)), 6372770.60112581, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(0, ATCMath::deg2rad(45)), 6367417.72494398, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::ellipsoidRadius(0, ATCMath::deg2rad(60)), 6362078.31475710, error));
}

void Test_ATCMath::test_stateLonDot()
{
    double error = 1e-8;

    double v = 100;
    double hdg = ATCMath::deg2rad(0);
    double gamma = ATCMath::deg2rad(0);
    double lat = ATCMath::deg2rad(0);
    double r = ATCMath::ellipsoidRadius(0, lat);

    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 0, error));

    hdg = ATCMath::deg2rad(45);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.108641569e-5, error));

    hdg = ATCMath::deg2rad(90);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.567855943e-5, error));

    gamma = ATCMath::deg2rad(90);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 0, error));

    gamma = ATCMath::deg2rad(45);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.108641569e-5, error));

    lat = ATCMath::deg2rad(30);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.281227008e-5, error));

    lat = ATCMath::deg2rad(45);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.570495361e-05, error));

    lat = ATCMath::deg2rad(60);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 2.222879840e-05, error));

    lat = ATCMath::deg2rad(75);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 4.296908273e-05, error));

    lat = ATCMath::deg2rad(80);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 6.405243501e-05, error));

    lat = ATCMath::deg2rad(85);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLonDot(v, hdg, gamma, lat, r), 1.276270296e-04, error));
}

void Test_ATCMath::test_stateLatDot()
{
    double error = 1e-8;

    double v = 100;
    double hdg = ATCMath::deg2rad(90);
    double gamma = ATCMath::deg2rad(0);

    double lat = ATCMath::deg2rad(0);
    double r = ATCMath::ellipsoidRadius(0, lat);

    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 0, error));

    hdg = ATCMath::deg2rad(45);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.108641569e-05, error));

    hdg = ATCMath::deg2rad(0);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.567855943e-05, error));

    gamma = ATCMath::deg2rad(90);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 0, error));

    gamma = ATCMath::deg2rad(45);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.108641569e-05, error));

    lat = ATCMath::deg2rad(30);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.109575137e-05, error));

    lat = ATCMath::deg2rad(45);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.110507920e-05, error));

    lat = ATCMath::deg2rad(60);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.111439920e-05, error));

    lat = ATCMath::deg2rad(75);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.112121696e-05, error));

    lat = ATCMath::deg2rad(80);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.112258862e-05, error));

    lat = ATCMath::deg2rad(85);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.112342856e-05, error));

    lat = ATCMath::deg2rad(90);
    r = ATCMath::ellipsoidRadius(0, lat);
    QVERIFY(ATCMath::compareDouble(ATCMath::stateLatDot(v, hdg, gamma, r), 1.112371139e-05, error));
}

void Test_ATCMath::test_stateHDot()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::stateHDot(100, 0), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHDot(100, ATCMath::deg2rad(30)), 50, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHDot(100, ATCMath::deg2rad(45)), 70.71067812, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHDot(100, ATCMath::deg2rad(60)), 86.60254038, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHDot(100, ATCMath::deg2rad(90)), 100, error));
}

void Test_ATCMath::test_stateVDot()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(200, 100, 25, 0), 4, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(100, 100, 25, 0), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(200, 50, 25, 0), 6, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(200, 100, 50, 0), 2, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(200, 100, 25, 0.5), 2, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateVDot(200, 100, 25, 1), 0, error));
}

void Test_ATCMath::test_stateHdgDot()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 10, ATCMath::deg2rad(0)), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 10, ATCMath::deg2rad(30)), 0.5, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 10, ATCMath::deg2rad(45)), 0.7071067812, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 10, ATCMath::deg2rad(60)), 0.8660254038, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 10, ATCMath::deg2rad(90)), 1, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(50, 10, 10, ATCMath::deg2rad(90)), 0.5, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 5, 10, ATCMath::deg2rad(90)), 2, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::stateHdgDot(100, 10, 5, ATCMath::deg2rad(90)), 2, error));
}

void Test_ATCMath::test_assignCM()
{
    QVERIFY(ATCMath::assignCM(10000, 10000) == BADA::Level);
    QVERIFY(ATCMath::assignCM(10001, 10000) == BADA::Level);
    QVERIFY(ATCMath::assignCM(9999, 10000) == BADA::Level);
    QVERIFY(ATCMath::assignCM(9998, 10000) == BADA::Climb);
    QVERIFY(ATCMath::assignCM(10002, 10000) == BADA::Descend);
}

void Test_ATCMath::test_assignFP()
{
    QVERIFY(ATCMath::assignFP(4000, 2919.0696, ATCMath::kt2mps(280), 290, ATCMath::atmosISA(2919.0696).p, ATCMath::atmosISA(2919.0696).rho) == BADA::UpperDescent);
    QVERIFY(ATCMath::assignFP(2919.0696, 2919.0696, ATCMath::kt2mps(280), 290, ATCMath::atmosISA(2919.0696).p, ATCMath::atmosISA(2919.0696).rho) == BADA::UpperDescent);

    QVERIFY(ATCMath::assignFP(2500, 2919.0696, ATCMath::kt2mps(250), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::LowerDescent);
    QVERIFY(ATCMath::assignFP(2200, 2919.0696, ATCMath::kt2mps(270), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::LowerDescent);
    QVERIFY(ATCMath::assignFP(2500, 2919.0696, ATCMath::kt2mps(270), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::LowerDescent);

    QVERIFY(ATCMath::assignFP(2000, 2919.0696, ATCMath::kt2mps(250), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::Approach);
    QVERIFY(ATCMath::assignFP(2000, 2919.0696, ATCMath::kt2mps(220), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::Approach);
    QVERIFY(ATCMath::assignFP(800, 2919.0696, ATCMath::kt2mps(250), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::Approach);

    QVERIFY(ATCMath::assignFP(800, 2919.0696, ATCMath::kt2mps(180), 290, ATCMath::atmosISA(0).p, ATCMath::atmosISA(0).rho) == BADA::Landing);
}

void Test_ATCMath::test_assignRPM()
{
    QVERIFY(ATCMath::assignRPM(ATCMath::ft2m(40000), ATCMath::ft2m(34982)) == BADA::Off);
    QVERIFY(ATCMath::assignRPM(ATCMath::ft2m(0.8 * 34982), ATCMath::ft2m(34982)) == BADA::Off);
    QVERIFY(ATCMath::assignRPM(ATCMath::ft2m(25000), ATCMath::ft2m(34982)) == BADA::On);
}

void Test_ATCMath::test_assignSHM()
{
    double xoverClb = ATCMath::crossoverAltitude(ATCMath::kt2mps(304), 0.79);
    double xoverCrs = ATCMath::crossoverAltitude(ATCMath::kt2mps(284), 0.71);
    double xoverDes = ATCMath::crossoverAltitude(ATCMath::kt2mps(294), 0.82);

    QVERIFY(ATCMath::assignSHM(11000, BADA::Climb, xoverClb, xoverCrs, xoverDes) == BADA::Mach);
    QVERIFY(ATCMath::assignSHM(11000, BADA::Level, xoverClb, xoverCrs, xoverDes) == BADA::Mach);
    QVERIFY(ATCMath::assignSHM(11000, BADA::Descend, xoverClb, xoverCrs, xoverDes) == BADA::Mach);

    QVERIFY(ATCMath::assignSHM(8945, BADA::Climb, xoverClb, xoverCrs, xoverDes) == BADA::Mach);
    QVERIFY(ATCMath::assignSHM(8266, BADA::Level, xoverClb, xoverCrs, xoverDes) == BADA::Mach);
    QVERIFY(ATCMath::assignSHM(9983, BADA::Descend, xoverClb, xoverCrs, xoverDes) == BADA::Mach);

    QVERIFY(ATCMath::assignSHM(8800, BADA::Climb, xoverClb, xoverCrs, xoverDes) == BADA::CAS);
    QVERIFY(ATCMath::assignSHM(8000, BADA::Level, xoverClb, xoverCrs, xoverDes) == BADA::CAS);
    QVERIFY(ATCMath::assignSHM(9800, BADA::Descend, xoverClb, xoverCrs, xoverDes) == BADA::CAS);
}

void Test_ATCMath::test_assignTRM()
{
    QVERIFY(ATCMath::assignTRM(15000) == BADA::High);
    QVERIFY(ATCMath::assignTRM(11000) == BADA::Low);
    QVERIFY(ATCMath::assignTRM(8000) == BADA::Low);
}

void Test_ATCMath::test_assignAM()
{
    QVERIFY(ATCMath::assignAM(10, 10) == BADA::Constant);
    QVERIFY(ATCMath::assignAM(9, 10) == BADA::Constant);
    QVERIFY(ATCMath::assignAM(11, 10) == BADA::Constant);
    QVERIFY(ATCMath::assignAM(15, 10) == BADA::Decelerate);
    QVERIFY(ATCMath::assignAM(5, 10) == BADA::Accelerate);
}

void Test_ATCMath::test_assignPredictionPhase()
{
    //Test target: PredictionPhase::Climb
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Climb, PredictionPhase::Climb) == PredictionPhase::Climb);
    QVERIFY(ATCMath::assignPredictionPhase(12, 10, BADA::Climb, PredictionPhase::Climb) == PredictionPhase::Climb);
    QVERIFY(ATCMath::assignPredictionPhase(10, 10, BADA::Climb, PredictionPhase::Cruise) == PredictionPhase::Climb);
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Climb, PredictionPhase::Descent) == PredictionPhase::Climb);
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Level, PredictionPhase::Climb) == PredictionPhase::Climb);

    //Test target: PredictionPhase::Cruise
    QVERIFY(ATCMath::assignPredictionPhase(10, 10, BADA::Level, PredictionPhase::Climb) == PredictionPhase::Cruise);
    QVERIFY(ATCMath::assignPredictionPhase(10, 10, BADA::Level, PredictionPhase::Cruise) == PredictionPhase::Cruise);
    QVERIFY(ATCMath::assignPredictionPhase(12, 10, BADA::Level, PredictionPhase::Climb) == PredictionPhase::Cruise);
    QVERIFY(ATCMath::assignPredictionPhase(10, 10, BADA::Level, PredictionPhase::Descent) == PredictionPhase::Cruise);

    //Test target: PredictionPhase::Descent
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Level, PredictionPhase::Descent) == PredictionPhase::Descent);
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Descend, PredictionPhase::Climb) == PredictionPhase::Descent);
    QVERIFY(ATCMath::assignPredictionPhase(10, 10, BADA::Descend, PredictionPhase::Climb) == PredictionPhase::Descent);
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Descend, PredictionPhase::Cruise) == PredictionPhase::Descent);
    QVERIFY(ATCMath::assignPredictionPhase(5, 10, BADA::Descend, PredictionPhase::Descent) == PredictionPhase::Descent);
}

void Test_ATCMath::test_mercatorProjectionLon()
{
    QVERIFY(ATCMath::mercatorProjectionLon(10, 0, 2) == 20);
    QVERIFY(ATCMath::mercatorProjectionLon(10, 5, 2) == 10);
    QVERIFY(ATCMath::mercatorProjectionLon(10, 5, 1) == 5);
}

void Test_ATCMath::test_mercatorProjectionLat()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(0, 1), 5.92855393E-12, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(30, 1), 3.12810368E+01, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(45, 1), 5.02274658E+01, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(60, 1), 7.51233992E+01, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(70, 1), 9.90708236E+01, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(80, 1), 1.39208063E+02, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(85, 1), 1.79027401E+02, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(89, 1), 2.71274913E+02, error));

    QVERIFY(ATCMath::compareDouble(ATCMath::mercatorProjectionLat(60, 5), 3.75616996E+02, error));
}

void Test_ATCMath::test_inverseMercatorLon()
{
    QVERIFY(ATCMath::inverseMercatorLon(20, 0, 2) == 10);
    QVERIFY(ATCMath::inverseMercatorLon(10, 5, 2) == 10);
    QVERIFY(ATCMath::inverseMercatorLon(5, 5, 1) == 10);
}

void Test_ATCMath::test_inverseMercatorLat()
{
    double mercatorError = 1e-8;
    double testError = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(0, 1), mercatorError, 1), 0, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(30, 1), mercatorError, 1), 30, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(45, 1), mercatorError, 1), 45, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(60, 1), mercatorError, 1), 60, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(70, 1), mercatorError, 1), 70, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(80, 1), mercatorError, 1), 80, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(85, 1), mercatorError, 1), 85, testError));
    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(89, 1), mercatorError, 1), 89, testError));

    QVERIFY(ATCMath::compareDouble(ATCMath::inverseMercatorLat(ATCMath::mercatorProjectionLat(60, 5), mercatorError, 5), 60, testError));
}

void Test_ATCMath::test_rotateX()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::rotateX(10, 0, 0), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateX(10, 0, 45), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateX(10, 0, 90), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateX(10, 0, -45), 5 * qSqrt(2), error));
}

void Test_ATCMath::test_rotateY()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::rotateY(10, 0, 0), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateY(10, 0, 45), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateY(10, 0, 90), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotateY(10, 0, -45), -5 * qSqrt(2), error));
}

void Test_ATCMath::test_translateToLocalX()
{
    QVERIFY(ATCMath::translateToLocalX(10, 2, 2) == 16);
    QVERIFY(ATCMath::translateToLocalX(5, 2, 2) == 6);
    QVERIFY(ATCMath::translateToLocalX(10, 1, 2) == 18);
    QVERIFY(ATCMath::translateToLocalX(10, 2, 4) == 32);
}

void Test_ATCMath::test_translateToLocalY()
{
    QVERIFY(ATCMath::translateToLocalY(10, 2, 2) == -16);
    QVERIFY(ATCMath::translateToLocalY(5, 2, 2) == -6);
    QVERIFY(ATCMath::translateToLocalY(10, 1, 2) == -18);
    QVERIFY(ATCMath::translateToLocalY(10, 2, 4) == -32);
}

void Test_ATCMath::test_translateFromLocalX()
{
    QVERIFY(ATCMath::translateFromLocalX(16, 2, 2) == 10);
    QVERIFY(ATCMath::translateFromLocalX(6, 2, 2) == 5);
    QVERIFY(ATCMath::translateFromLocalX(18, 1, 2) == 10);
    QVERIFY(ATCMath::translateFromLocalX(32, 2, 4) == 10);
}

void Test_ATCMath::test_translateFromLocalY()
{
    QVERIFY(ATCMath::translateFromLocalY(-16, 2, 2) == 10);
    QVERIFY(ATCMath::translateFromLocalY(-6, 2, 2) == 5);
    QVERIFY(ATCMath::translateFromLocalY(-18, 1, 2) == 10);
    QVERIFY(ATCMath::translateFromLocalY(-32, 2, 4) == 10);
}

void Test_ATCMath::test_geo2local()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::geo2local(ATCMath::deg2rad(60), ATCMath::deg2rad(60), 45, 20, 15, 2, 1, 4).x(), -67.0445705431541, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::geo2local(ATCMath::deg2rad(60), ATCMath::deg2rad(60), 45, 20, 15, 2, 1, 4).y(), -155.43648952893, error));
}

void Test_ATCMath::test_local2geo()
{
    double error = 1e-8;

    QVERIFY(ATCMath::compareDouble(ATCMath::local2geo(-67.0445705431541, -155.43648952893, 45, 20, 15, 2, 1, 4).x(), 60, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::local2geo(-67.0445705431541, -155.43648952893, 45, 20, 15, 2, 1, 4).y(), 60, error));
}

void Test_ATCMath::test_rotatePoint()
{
    double error = 1e-8;

    QPointF pt(10, 0);

    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 0, ATC::Deg).x(), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 45, ATC::Deg).x(), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 90, ATC::Deg).x(), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, -45, ATC::Deg).x(), 5 * qSqrt(2), error));

    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 0, ATC::Deg).y(), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 45, ATC::Deg).y(), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, 90, ATC::Deg).y(), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, -45, ATC::Deg).y(), -5 * qSqrt(2), error));

    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(0), ATC::Rad).x(), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(45), ATC::Rad).x(), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(90), ATC::Rad).x(), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(-45), ATC::Rad).x(), 5 * qSqrt(2), error));

    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(0), ATC::Rad).y(), 0, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(45), ATC::Rad).y(), 5 * qSqrt(2), error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(90), ATC::Rad).y(), 10, error));
    QVERIFY(ATCMath::compareDouble(ATCMath::rotatePoint(pt, ATCMath::deg2rad(-45), ATC::Rad).y(), -5 * qSqrt(2), error));
}
