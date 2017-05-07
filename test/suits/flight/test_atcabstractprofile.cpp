
#include "test_atcabstractprofile.h"

void Test_ATCAbstractProfile::test_timeInterval()
{
    Mock_ATCAbstractProfile foo;

    QVector<double> levels;
    QVector<double> time;

    levels << 0 << 1 << 2;
    time << 0 << 1 << 3;

    foo.getTimeInterpolator()->insertInterpolator(0, new ATCInterpolator(levels, time, ExType::Tangent));

    time.clear();
    time << 0 << 2 << 6;

    foo.getTimeInterpolator()->insertInterpolator(1, new ATCInterpolator(levels, time, ExType::Tangent));

    QVERIFY(foo.timeInterval(-1, 1, 2) == 0);
    QVERIFY(foo.timeInterval(-1, 2, 1) == 0);
    QVERIFY(foo.timeInterval(-1, 0.5, 1.5) == 0);
    QVERIFY(foo.timeInterval(-1, 1.5, 0.5) == 0);
    QVERIFY(foo.timeInterval(-1, -1, 3) == 0);
    QVERIFY(foo.timeInterval(-1, 3, -1) == 0);

    QVERIFY(foo.timeInterval(0, 1, 2) == 2);
    QVERIFY(foo.timeInterval(0, 2, 1) == 2);
    QVERIFY(foo.timeInterval(0, 0.5, 1.5) == 1.5);
    QVERIFY(foo.timeInterval(0, 1.5, 0.5) == 1.5);
    QVERIFY(foo.timeInterval(0, -1, 3) == 6);
    QVERIFY(foo.timeInterval(0, 3, -1) == 6);

    QVERIFY(foo.timeInterval(0.5, 1, 2) == 3);
    QVERIFY(foo.timeInterval(0.5, 2, 1) == 3);
    QVERIFY(foo.timeInterval(0.5, 0.5, 1.5) == 2.25);
    QVERIFY(foo.timeInterval(0.5, 1.5, 0.5) == 2.25);
    QVERIFY(foo.timeInterval(0.5, -1, 3) == 9);
    QVERIFY(foo.timeInterval(0.5, 3, -1) == 9);

    QVERIFY(foo.timeInterval(1, 1, 2) == 4);
    QVERIFY(foo.timeInterval(1, 2, 1) == 4);
    QVERIFY(foo.timeInterval(1, 0.5, 1.5) == 3);
    QVERIFY(foo.timeInterval(1, 1.5, 0.5) == 3);
    QVERIFY(foo.timeInterval(1, -1, 3) == 12);
    QVERIFY(foo.timeInterval(1, 3, -1) == 12);

    QVERIFY(foo.timeInterval(2, 1, 2) == 6);
    QVERIFY(foo.timeInterval(2, 2, 1) == 6);
    QVERIFY(foo.timeInterval(2, 0.5, 1.5) == 4.5);
    QVERIFY(foo.timeInterval(2, 1.5, 0.5) == 4.5);
    QVERIFY(foo.timeInterval(2, -1, 3) == 18);
    QVERIFY(foo.timeInterval(2, 3, -1) == 18);
}

void Test_ATCAbstractProfile::test_distanceInterval()
{
    Mock_ATCAbstractProfile foo;

    QVector<double> levels;
    QVector<double> distance;

    levels << 0 << 1 << 2;
    distance << 0 << 1 << 3;

    foo.getDistanceInterpolator()->insertInterpolator(0, new ATCInterpolator(levels, distance, ExType::Tangent));

    distance.clear();
    distance << 0 << 2 << 6;

    foo.getDistanceInterpolator()->insertInterpolator(1, new ATCInterpolator(levels, distance, ExType::Tangent));

    QVERIFY(foo.distanceInterval(-1, 1, 2) == 0);
    QVERIFY(foo.distanceInterval(-1, 2, 1) == 0);
    QVERIFY(foo.distanceInterval(-1, 0.5, 1.5) == 0);
    QVERIFY(foo.distanceInterval(-1, 1.5, 0.5) == 0);
    QVERIFY(foo.distanceInterval(-1, -1, 3) == 0);
    QVERIFY(foo.distanceInterval(-1, 3, -1) == 0);

    QVERIFY(foo.distanceInterval(0, 1, 2) == 2);
    QVERIFY(foo.distanceInterval(0, 2, 1) == 2);
    QVERIFY(foo.distanceInterval(0, 0.5, 1.5) == 1.5);
    QVERIFY(foo.distanceInterval(0, 1.5, 0.5) == 1.5);
    QVERIFY(foo.distanceInterval(0, -1, 3) == 6);
    QVERIFY(foo.distanceInterval(0, 3, -1) == 6);

    QVERIFY(foo.distanceInterval(0.5, 1, 2) == 3);
    QVERIFY(foo.distanceInterval(0.5, 2, 1) == 3);
    QVERIFY(foo.distanceInterval(0.5, 0.5, 1.5) == 2.25);
    QVERIFY(foo.distanceInterval(0.5, 1.5, 0.5) == 2.25);
    QVERIFY(foo.distanceInterval(0.5, -1, 3) == 9);
    QVERIFY(foo.distanceInterval(0.5, 3, -1) == 9);

    QVERIFY(foo.distanceInterval(1, 1, 2) == 4);
    QVERIFY(foo.distanceInterval(1, 2, 1) == 4);
    QVERIFY(foo.distanceInterval(1, 0.5, 1.5) == 3);
    QVERIFY(foo.distanceInterval(1, 1.5, 0.5) == 3);
    QVERIFY(foo.distanceInterval(1, -1, 3) == 12);
    QVERIFY(foo.distanceInterval(1, 3, -1) == 12);

    QVERIFY(foo.distanceInterval(2, 1, 2) == 6);
    QVERIFY(foo.distanceInterval(2, 2, 1) == 6);
    QVERIFY(foo.distanceInterval(2, 0.5, 1.5) == 4.5);
    QVERIFY(foo.distanceInterval(2, 1.5, 0.5) == 4.5);
    QVERIFY(foo.distanceInterval(2, -1, 3) == 18);
    QVERIFY(foo.distanceInterval(2, 3, -1) == 18);
}

//void Test_ATCAbstractProfile::test_mixedTimeInterval()
//{
//    Mock_ATCAbstractProfile foo;

//    QVector<double> levels;
//    QVector<double> time;

//    levels << 0 << 1 << 2;
//    time << 0 << 1 << 3;

//    foo.getTimeInterpolator()->insertInterpolator(0, new ATCInterpolator(levels, time, ExType::Tangent));
//    foo.getInverseTimeInterpolator()->insertInterpolator(0, new ATCInterpolator(time, levels, ExType::Tangent));

//    time.clear();
//    time << 0 << 2 << 6;

//    foo.getTimeInterpolator()->insertInterpolator(1, new ATCInterpolator(levels, time, ExType::Tangent));
//    foo.getInverseTimeInterpolator()->insertInterpolator(1, new ATCInterpolator(time, levels, ExType::Tangent));

//    //Test of function with one interval
////    QVERIFY(foo.mixedTimeInterval(-1, 0, 3) == 0);
////    QVERIFY(foo.mixedTimeInterval(-1, 0.5, 1.5) == 1.5);
////    QVERIFY(foo.mixedTimeInterval(-1, 0, -1) == -1);
////    QVERIFY(foo.mixedTimeInterval(-1, 2, 2) == 3);
////    QVERIFY(foo.mixedTimeInterval(-1, -1, 6) == 3);
////    QVERIFY(foo.mixedTimeInterval(-1, 3, -6) == -1);

////    //OK
////    QVERIFY(foo.mixedTimeInterval(0, 0, 3) == 2);
////    QVERIFY(foo.mixedTimeInterval(0, 0.5, 1.5) == 1.5);
////    QVERIFY(foo.mixedTimeInterval(0, 0, -1) == -1);
////    QVERIFY(foo.mixedTimeInterval(0, 2, 2) == 3);
////    QVERIFY(foo.mixedTimeInterval(0, -1, 6) == 3);
////    QVERIFY(foo.mixedTimeInterval(0, 3, -6) == -1);

//    QVERIFY(foo.mixedTimeInterval(0.5, 0, 4.5) == 2);
//    QVERIFY(foo.mixedTimeInterval(0.5, 0.5, 1.5) == 1.5);
//    QVERIFY(foo.mixedTimeInterval(0.5, 0, -1) == -1);
//    QVERIFY(foo.mixedTimeInterval(0.5, 2, 2) == 3);
//    QVERIFY(foo.mixedTimeInterval(0.5, -1, 6) == 3);
//    QVERIFY(foo.mixedTimeInterval(0.5, 3, -6) == -1);

//    //Test of function with vector of intervals
//    QVector<double> intervals;
//    intervals << -1 << 0 << 1 << 2 << 3 << 5;

//    QVector<double> correctResults;
//    correctResults << -1 << 0 << 1 << 1.5 << 2 << 3;

//    QVERIFY(foo.mixedTimeInterval(0, 0, intervals) == correctResults);
//}

//void Test_ATCAbstractProfile::test_mixedDistanceInterval()
//{
//    QVector<double> levels;
//    QVector<double> time;
//    QVector<double> distance;

//    levels << 0 << 1 << 2;
//    time << 0 << 2 << 5;
//    distance << 0 << 3 << 7;

//    ATCProfileClimb foo(levels, time, distance);

//    //Test of function with one interval
//    QVERIFY(foo.mixedDistanceInterval(0, 7) == 2);
//    QVERIFY(foo.mixedDistanceInterval(0.25, 4.25) == 1.5);

//    QVERIFY(foo.mixedDistanceInterval(0, -3) == -1);
//    QVERIFY(foo.mixedDistanceInterval(2, 4) == 3);

//    QVERIFY(foo.mixedDistanceInterval(-1, 14) == 3);
//    QVERIFY(foo.mixedDistanceInterval(3, -14) == -1);

//    //Test of function with vector of intervals
//    QVector<double> intervals;
//    intervals << -3 << 0 << 1.5 << 3 << 5 << 7 << 11;

//    QVector<double> correctResults;
//    correctResults << -1 << 0 << 0.5 << 1 << 1.5 << 2 << 3;

//    QVERIFY(foo.mixedDistanceInterval(0, intervals) == correctResults);
//}
