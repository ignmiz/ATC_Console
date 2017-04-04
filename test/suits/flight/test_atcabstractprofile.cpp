
#include "test_atcabstractprofile.h"

void Test_ATCAbstractProfile::test_setTimeExtrapolationType()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 4;
    distance << 0 << 3 << 6;

    Mock_ATCAbstractProfile foo(levels, time, distance);
    QVERIFY(foo.getTimeExtrapolationType() == ExType::Tangent);

    foo.setTimeExtrapolationType(ExType::None);
    QVERIFY(foo.getTimeExtrapolationType() == ExType::None);
}

void Test_ATCAbstractProfile::test_setDistanceExtrapolationType()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 4;
    distance << 0 << 3 << 6;

    Mock_ATCAbstractProfile foo(levels, time, distance);
    QVERIFY(foo.getDistanceExtrapolationType() == ExType::Tangent);

    foo.setDistanceExtrapolationType(ExType::Saturation);
    QVERIFY(foo.getDistanceExtrapolationType() == ExType::Saturation);
}

void Test_ATCAbstractProfile::test_timeInterval()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 5;
    distance << 0 << 3 << 7;

    Mock_ATCAbstractProfile foo(levels, time, distance);

    QVERIFY(foo.timeInterval(1, 2) == 3);
    QVERIFY(foo.timeInterval(2, 1) == 3);

    QVERIFY(foo.timeInterval(0.5, 1.5) == 2.5);
    QVERIFY(foo.timeInterval(1.5, 0.5) == 2.5);

    QVERIFY(foo.timeInterval(-1, 3) == 10);
    QVERIFY(foo.timeInterval(3, -1) == 10);
}

void Test_ATCAbstractProfile::test_distanceInterval()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 5;
    distance << 0 << 3 << 7;

    Mock_ATCAbstractProfile foo(levels, time, distance);

    QVERIFY(foo.distanceInterval(1, 2) == 4);
    QVERIFY(foo.distanceInterval(2, 1) == 4);

    QVERIFY(foo.distanceInterval(0.5, 1.5) == 3.5);
    QVERIFY(foo.distanceInterval(1.5, 0.5) == 3.5);

    QVERIFY(foo.distanceInterval(-1, 3) == 14);
    QVERIFY(foo.distanceInterval(3, -1) == 14);
}


