#ifndef TEST_ATCFLIGHTTAG_H
#define TEST_ATCFLIGHTTAG_H

#include "atcflighttag.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightTag : public QObject
{
    Q_OBJECT

private slots:
    void test_setDiamond();
    void test_setDiamondPosition();
    void test_setLeader();
    void test_setLeaderEndPosition();
};

#endif // TEST_ATCFLIGHTTAG_H
