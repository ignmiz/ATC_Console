
#include "test_atcprofiledescent.h"

void Test_ATCProfileDescent::test_mixedTimeInterval()
{
    double error = 1e-12;

    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 5 << 2 << 0;
    distance << 7 << 3 << 0;

    ATCProfileDescent foo(levels, time, distance);

    //Test of function with one interval
    QVERIFY(ATCMath::compareDouble(foo.mixedTimeInterval(2, 5), 0, error));
    QVERIFY(ATCMath::compareDouble(foo.mixedTimeInterval(1.5, 3), (1.0 / 3.0), error));

    QVERIFY(foo.mixedTimeInterval(0, 3) == -1);
    QVERIFY(foo.mixedTimeInterval(2, -2) == 3);

    QVERIFY(foo.mixedTimeInterval(-1, -10) == 3);
    QVERIFY(foo.mixedTimeInterval(3, 10) == -1);

    //Test of function with vector of intervals
    QVector<double> intervals;
    intervals << -2 << 0 << 1 << 2 << 3.5 << 8;

    QVector<double> correctResults;
    correctResults << 3 << 2 << 1.5 << 1 << 0.5 << -1;

    QVERIFY(foo.mixedTimeInterval(2, intervals) == correctResults);
}

void Test_ATCProfileDescent::test_mixedDistanceInterval()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    time << 5 << 2 << 0;
    distance << 7 << 3 << 0;

    ATCProfileDescent foo(levels, time, distance);

    //Test of function with one interval
    QVERIFY(foo.mixedDistanceInterval(2, 7) == 0);
    QVERIFY(foo.mixedDistanceInterval(1.5, 3.5) == 0.5);

    QVERIFY(foo.mixedDistanceInterval(0, 4) == -1);
    QVERIFY(foo.mixedDistanceInterval(2, -3) == 3);

    QVERIFY(foo.mixedDistanceInterval(-1, -14) == 3);
    QVERIFY(foo.mixedDistanceInterval(3, 14) == -1);

    //Test of function with vector of intervals
    QVector<double> intervals;
    intervals << -3 << 0 << 1.5 << 3 << 5 << 11;

    QVector<double> correctResults;
    correctResults << 3 << 2 << 1.5 << 1 << 0.5 << -1;

    qDebug() << foo.mixedDistanceInterval(2, intervals);
    QVERIFY(foo.mixedDistanceInterval(2, intervals) == correctResults);
}
