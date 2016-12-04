#ifndef TEST_ATCMATH_H
#define TEST_ATCMATH_H

#include "atcmath.h"

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

    void test_atmosISA();

    void test_crossoverAltitude();
    void test_normalizeAngle();
};

#endif // TEST_ATCMATH_H
