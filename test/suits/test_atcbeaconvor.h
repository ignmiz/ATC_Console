#ifndef TEST_ATCBEACONVOR_H
#define TEST_ATCBEACONVOR_H

#include "atcbeaconvor.h"

#include <QTest>
#include <QObject>

class Test_ATCBeaconVOR : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();
};

#endif // TEST_ATCBEACONVOR_H
