#ifndef TEST_ATCFLIGHTNUMBERFACTORY_H
#define TEST_ATCFLIGHTNUMBERFACTORY_H

#include "atcflightnumberfactory.h"

#include <QObject>
#include <QTest>

class Test_ATCFlightNumberFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_getFligthNumber();
};

#endif // TEST_ATCFLIGHTNUMBERFACTORY_H
