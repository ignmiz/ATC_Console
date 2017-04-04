#ifndef TEST_ATCPROFILECLIMB_H
#define TEST_ATCPROFILECLIMB_H

#include "atcprofileclimb.h"

#include <QObject>
#include <QTest>

class Test_ATCProfileClimb : public QObject
{
    Q_OBJECT

private slots:
    void test_mixedTimeInterval();
    void test_mixedDistanceInterval();
};

#endif // TEST_ATCPROFILECLIMB_H
