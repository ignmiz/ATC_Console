#ifndef TEST_ATCINTERPOLATOR2D_H
#define TEST_ATCINTERPOLATOR2D_H

#include "atcinterpolator2d.h"

#include <QObject>
#include <QTest>

class Test_ATCInterpolator2D : public QObject
{
    Q_OBJECT

private slots:
    void test_insertInterpolator();
    void test_removeInterpolator();
    void test_interpolate();
    void test_closestKeys();
};

#endif // TEST_ATCINTERPOLATOR2D_H
