
#include "test_atcinterpolator2d.h"


void Test_ATCInterpolator2D::test_insertInterpolator()
{
    ATCInterpolator2D foo;

    ATCInterpolator *interp1 = new ATCInterpolator();
    ATCInterpolator *interp2 = new ATCInterpolator();
    ATCInterpolator *interp3 = new ATCInterpolator();

    foo.insertInterpolator(0.5, interp1);
    foo.insertInterpolator(0.6, interp2);
    foo.insertInterpolator(0.7, interp3);

    QVERIFY(foo.getInterpolator(0.5) == interp1);
    QVERIFY(foo.getInterpolator(0.6) == interp2);
    QVERIFY(foo.getInterpolator(0.7) == interp3);
}

void Test_ATCInterpolator2D::test_removeInterpolator()
{
    ATCInterpolator2D foo;

    ATCInterpolator *interp1 = new ATCInterpolator();
    ATCInterpolator *interp2 = new ATCInterpolator();
    ATCInterpolator *interp3 = new ATCInterpolator();

    foo.insertInterpolator(0.5, interp1);
    foo.insertInterpolator(0.6, interp2);
    foo.insertInterpolator(0.7, interp3);

    foo.removeInterpolator(0.5);
    QVERIFY(foo.getInterpolator(0.5) == nullptr);

    foo.removeInterpolator(0.7);
    QVERIFY(foo.getInterpolator(0.7) == nullptr);

    foo.removeInterpolator(0.6);
    QVERIFY(foo.getInterpolator(0.6) == nullptr);
}

void Test_ATCInterpolator2D::test_interpolate()
{
    ATCInterpolator2D foo;

    //Interpolator 1
    QVector<double> breakpoints1;
    QVector<double> values1;

    breakpoints1 << 0 << 1 << 2;
    values1 << 0 << 0 << 0;

    double key1 = 0;
    ATCInterpolator *interp1 = new ATCInterpolator(breakpoints1, values1, ExType::Tangent);

    foo.insertInterpolator(key1, interp1);

    //Interpolator 2
    QVector<double> breakpoints2;
    QVector<double> values2;

    breakpoints2 << 0 << 1 << 2;
    values2 << 1 << 1 << 1;

    double key2 = 1;
    ATCInterpolator *interp2 = new ATCInterpolator(breakpoints2, values2, ExType::Tangent);

    foo.insertInterpolator(key2, interp2);

    //Interpolator 3
    QVector<double> breakpoints3;
    QVector<double> values3;

    breakpoints3 << 0 << 1 << 2;
    values3 << 3 << 3 << 3;

    double key3 = 2;
    ATCInterpolator *interp3 = new ATCInterpolator(breakpoints3, values3, ExType::Tangent);

    foo.insertInterpolator(key3, interp3);

    //Test 2D interpolation
    QVector<double> sites;
    QVector<double> results(7);

    sites << -1 << 0 << 0.5 << 1 << 1.5 << 2 << 3;

    double key = -1;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, -1));

    key = -0.5;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, -0.5));

    key = 0;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 0));

    key = 0.5;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 0.5));

    key = 1;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 1));

    key = 1.5;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 2));

    key = 2;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 3));

    key = 3;
    foo.interpolate(key, sites, results);
    QVERIFY(results == QVector<double>(7, 5));
}
