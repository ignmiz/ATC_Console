#ifndef TEST_ATCINTERPOLATOR_H
#define TEST_ATCINTERPOLATOR_H

#include "atcinterpolator.h"

#include <QObject>
#include <QTest>

class Test_ATCInterpolator : public QObject
{
    Q_OBJECT

private slots:
    void test_interpolate();
};

#endif // TEST_ATCINTERPOLATOR_H
