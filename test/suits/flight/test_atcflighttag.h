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
};

#endif // TEST_ATCFLIGHTTAG_H
