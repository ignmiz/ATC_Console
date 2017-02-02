#ifndef TEST_ATCMATH_H
#define TEST_ATCMATH_H

#include "atcmath.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCMath : public QObject
{
    Q_OBJECT

private slots:
    void test_rad2deg();
    void test_deg2rad();

    void test_m2nm();
    void test_nm2m();

    void test_m2ft();
    void test_ft2m();

    void test_mps2kt();
    void test_kt2mps();

    void test_cas2tas();
    void test_cas2mach();

    void test_tas2cas();
    void test_tas2mach();

    void test_mach2tas();
    void test_mach2cas();

    void test_compareDouble();

    void test_min();
    void test_max();

    void test_atmosISA();

    void test_crossoverAltitude();
    void test_normalizeAngle();

    void test_ESF();
    void test_pathAngle();
    void test_bankAngle();

    void test_randomMass();
    void test_recalculateSpeed();
    void test_recalculateReductionFactor();

    void test_nominalSpeedCL();
    void test_nominalSpeedCR();
    void test_nominalSpeedDS();
    void test_nominalSpeed2tas();

    void test_liftCoefficient();
    void test_dragCoefficient();

    void test_lift();
    void test_drag();
    void test_thrust();

    void test_assignCM();
    void test_assignFP();
    void test_assignRPM();
    void test_assignSHM();
    void test_assignTRM();
    void test_assignAM();
};

#endif // TEST_ATCMATH_H
