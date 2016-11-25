#ifndef TEST_ATCFLIGHTTAG_H
#define TEST_ATCFLIGHTTAG_H

#include "atcflighttag.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightTag : public QObject
{
    Q_OBJECT

private slots:
    void test_setTagType();
    void test_setDiamond();
    void test_setDiamondPosition();
    void test_setLeader();
    void test_setLeaderEndPosition();
    void test_getTagBox();
    void test_getConnector();
    void test_getText();
};

#endif // TEST_ATCFLIGHTTAG_H
