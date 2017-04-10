
#include "test_atcprofileapproach.h"

void Test_ATCProfileApproach::test_timeInterval()
{
    QVector<double> levels;
    QVector<double> time;

    levels << 0 << 1 << 2;
    time << 5 << 2 << 0;

    ATCProfileApproach foo(levels, time);

    QVERIFY(foo.timeInterval(1, 2) == 2);
    QVERIFY(foo.timeInterval(2, 1) == 2);

    QVERIFY(foo.timeInterval(0.5, 1.5) == 2.5);
    QVERIFY(foo.timeInterval(1.5, 0.5) == 2.5);

    QVERIFY(foo.timeInterval(-1, 3) == 10);
    QVERIFY(foo.timeInterval(3, -1) == 10);
}
