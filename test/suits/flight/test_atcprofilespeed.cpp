
#include "test_atcprofilespeed.h"

void Test_ATCProfileSpeed::test_nominalClimbSpeed()
{
    QVector<double> levels;
    QVector<double> clbSpds;
    QVector<double> crsSpds;
    QVector<double> dscSpds;

    levels << 0 << 1 << 2 << 3;
    clbSpds << 0 << 1 << 2 << 3;
    crsSpds << 0 << 2 << 4 << 6;
    dscSpds << 0 << 3 << 6 << 9;

    ATCProfileSpeed foo(levels, clbSpds, crsSpds, dscSpds);

    QVERIFY(foo.nominalClimbSpeed(0) == 0);
    QVERIFY(foo.nominalClimbSpeed(1.75) == 1.75);
    QVERIFY(foo.nominalClimbSpeed(3) == 3);
    QVERIFY(foo.nominalClimbSpeed(-1) == 0);
    QVERIFY(foo.nominalClimbSpeed(4) == 3);
}

void Test_ATCProfileSpeed::test_nominalCruiseSpeed()
{
    QVector<double> levels;
    QVector<double> clbSpds;
    QVector<double> crsSpds;
    QVector<double> dscSpds;

    levels << 0 << 1 << 2 << 3;
    clbSpds << 0 << 1 << 2 << 3;
    crsSpds << 0 << 2 << 4 << 6;
    dscSpds << 0 << 3 << 6 << 9;

    ATCProfileSpeed foo(levels, clbSpds, crsSpds, dscSpds);

    QVERIFY(foo.nominalCruiseSpeed(0) == 0);
    QVERIFY(foo.nominalCruiseSpeed(1.75) == 3.5);
    QVERIFY(foo.nominalCruiseSpeed(3) == 6);
    QVERIFY(foo.nominalCruiseSpeed(-1) == 0);
    QVERIFY(foo.nominalCruiseSpeed(4) == 6);
}

void Test_ATCProfileSpeed::test_nominalDescentSpeed()
{
    QVector<double> levels;
    QVector<double> clbSpds;
    QVector<double> crsSpds;
    QVector<double> dscSpds;

    levels << 0 << 1 << 2 << 3;
    clbSpds << 0 << 1 << 2 << 3;
    crsSpds << 0 << 2 << 4 << 6;
    dscSpds << 0 << 3 << 6 << 9;

    ATCProfileSpeed foo(levels, clbSpds, crsSpds, dscSpds);

    QVERIFY(foo.nominalDescentSpeed(0) == 0);
    QVERIFY(foo.nominalDescentSpeed(1.75) == 5.25);
    QVERIFY(foo.nominalDescentSpeed(3) == 9);
    QVERIFY(foo.nominalDescentSpeed(-1) == 0);
    QVERIFY(foo.nominalDescentSpeed(4) == 9);
}
