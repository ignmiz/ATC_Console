#ifndef TEST_ATCPROFILEAPPROACH_H
#define TEST_ATCPROFILEAPPROACH_H

#include "atcprofileapproach.h"

#include <QObject>
#include <QTest>

class Test_ATCProfileApproach : public QObject
{
    Q_OBJECT

private slots:
    void test_timeInterval();
};

#endif // TEST_ATCPROFILEAPPROACH_H
