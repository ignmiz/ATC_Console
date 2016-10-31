#ifndef TEST_ATCMATH_H
#define TEST_ATCMATH_H

#include "atcmath.h"

#include <QObject>
#include <QTest>

class Test_ATCMath : public QObject
{
    Q_OBJECT

private slots:
    void test_m2nm();
    void test_nm2m();

    void test_m2ft();
    void test_ft2m();

    void test_mps2kt();
    void test_kt2mps();

    void test_tempISA();
};

#endif // TEST_ATCMATH_H
