
#include "test_atcinterpolator.h"

void Test_ATCInterpolator::test_interpolate()
{
    QVector<double> x;
    QVector<double> y;
    QVector<double> sites;

    x << 0 << 1 << 2;
    y << 0 << 1 << 3;
    sites << 0 << 0.5 << 1 << 1.75 << 2;

    QVector<double> results(QVector<double>(sites.size()));

    ATCInterpolator interpolator1(x, y, ExType::None);
    interpolator1.interpolate(sites, results);
    QVERIFY(results.at(0) == 0);
    QVERIFY(results.at(1) == 0.5);
    QVERIFY(results.at(2) == 1);
    QVERIFY(results.at(3) == 2.5);
    QVERIFY(results.at(4) == 3);

    sites.clear();
    sites << -1 << 0 << 0.5 << 1 << 1.75 << 2 << 3;

    results.clear();
    results = QVector<double>(sites.size());

    ATCInterpolator interpolator2(x, y, ExType::Saturation);
    interpolator2.interpolate(sites, results);
    QVERIFY(results.at(0) == 0);
    QVERIFY(results.at(1) == 0);
    QVERIFY(results.at(2) == 0.5);
    QVERIFY(results.at(3) == 1);
    QVERIFY(results.at(4) == 2.5);
    QVERIFY(results.at(5) == 3);
    QVERIFY(results.at(6) == 3);

    results.clear();
    results = QVector<double>(sites.size());

    ATCInterpolator interpolator3(x, y, ExType::Tangent);
    interpolator3.interpolate(sites, results);
    QVERIFY(results.at(0) == -1);
    QVERIFY(results.at(1) == 0);
    QVERIFY(results.at(2) == 0.5);
    QVERIFY(results.at(3) == 1);
    QVERIFY(results.at(4) == 2.5);
    QVERIFY(results.at(5) == 3);
    QVERIFY(results.at(6) == 5);
}

void Test_ATCInterpolator::test_setExtrapolationType()
{
    QVector<double> x;
    QVector<double> y;

    x << 0 << 1 << 2;
    y << 0 << 1 << 3;

    ATCInterpolator interpolator(x, y, ExType::Tangent);
    QVERIFY(interpolator.getExtrapolationType() == ExType::Tangent);

    interpolator.setExtrapolationType(ExType::Saturation);
    QVERIFY(interpolator.getExtrapolationType() == ExType::Saturation);
}

void Test_ATCInterpolator::test_isCorrect()
{
    QVector<double> x;
    QVector<double> y;

    x << 0 << 1 << 2;
    y << 0 << 1 << 3;

    ATCInterpolator interpolator(x, y, ExType::Tangent);
    QVERIFY(interpolator.isCorrect() == true);
}
