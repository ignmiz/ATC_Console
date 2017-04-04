#ifndef TEST_ATCABSTRACTPROFILE_H
#define TEST_ATCABSTRACTPROFILE_H

#include "mock_atcabstractprofile.h"

#include <QObject>
#include <QTest>

class Test_ATCAbstractProfile : public QObject
{
    Q_OBJECT

private slots:
    void test_setTimeExtrapolationType();
    void test_setDistanceExtrapolationType();
    void test_timeInterval();
    void test_distanceInterval();
};

#endif // TEST_ATCABSTRACTPROFILE_H
