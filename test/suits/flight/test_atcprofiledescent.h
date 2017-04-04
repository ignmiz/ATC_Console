#ifndef TEST_ATCPROFILEDESCENT_H
#define TEST_ATCPROFILEDESCENT_H

#include "atcprofiledescent.h"

#include <QObject>
#include <QTest>

class Test_ATCProfileDescent : public QObject
{
    Q_OBJECT

private slots:
    void test_mixedTimeInterval();
    void test_mixedDistanceInterval();
};

#endif // TEST_ATCPROFILEDESCENT_H
