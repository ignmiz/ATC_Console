
#include "test_atcprofileclimb.h"

void Test_ATCProfileClimb::test_mixedTimeInterval()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 5;
    distance << 0 << 3 << 7;

    ATCProfileClimb foo(levels, time, distance);

    //Test of function with one interval
    QVERIFY(foo.mixedTimeInterval(0, 5) == 2);
    QVERIFY(foo.mixedTimeInterval(0.25, 3) == 1.5);

    QVERIFY(foo.mixedTimeInterval(0, -2) == -1);
    QVERIFY(foo.mixedTimeInterval(2, 3) == 3);

    QVERIFY(foo.mixedTimeInterval(-1, 10) == 3);
    QVERIFY(foo.mixedTimeInterval(3, -10) == -1);

    //Test of function with vector of intervals
    QVector<double> intervals;
    intervals << -2 << 0 << 1 << 2 << 3.5 << 5 << 8;

    QVector<double> correctResults;
    correctResults << -1 << 0 << 0.5 << 1 << 1.5 << 2 << 3;

    QVERIFY(foo.mixedTimeInterval(0, intervals) == correctResults);
}

void Test_ATCProfileClimb::test_mixedDistanceInterval()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 0 << 2 << 5;
    distance << 0 << 3 << 7;

    ATCProfileClimb foo(levels, time, distance);

    //Test of function with one interval
    QVERIFY(foo.mixedDistanceInterval(0, 7) == 2);
    QVERIFY(foo.mixedDistanceInterval(0.25, 4.25) == 1.5);

    QVERIFY(foo.mixedDistanceInterval(0, -3) == -1);
    QVERIFY(foo.mixedDistanceInterval(2, 4) == 3);

    QVERIFY(foo.mixedDistanceInterval(-1, 14) == 3);
    QVERIFY(foo.mixedDistanceInterval(3, -14) == -1);

    //Test of function with vector of intervals
    QVector<double> intervals;
    intervals << -3 << 0 << 1.5 << 3 << 5 << 7 << 11;

    QVector<double> correctResults;
    correctResults << -1 << 0 << 0.5 << 1 << 1.5 << 2 << 3;

    QVERIFY(foo.mixedDistanceInterval(0, intervals) == correctResults);
}
