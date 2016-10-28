#ifndef TEST_ATCROUTE_H
#define TEST_ATCROUTE_H

#include "atcroute.h"

#include <QObject>
#include <QTest>

class Test_ATCRoute : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
};

#endif // TEST_ATCROUTE_H
