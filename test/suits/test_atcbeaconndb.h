#ifndef TEST_ATCBEACONNDB_H
#define TEST_ATCBEACONNDB_H

#include "atcbeaconndb.h"

#include <QTest>
#include <QDebug>

class Test_ATCBeaconNDB : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();
};

#endif // TEST_ATCBEACONNDB_H
