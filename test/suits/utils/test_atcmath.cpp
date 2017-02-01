
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

    double error = 10e-8;

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

    double error = 10e-8;

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

    double error = 10e-8;

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
