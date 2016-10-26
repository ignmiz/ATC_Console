#ifndef TEST_ATCAIRCRAFTTYPEFACTORY_H
#define TEST_ATCAIRCRAFTTYPEFACTORY_H

#include "atcaircrafttypefactory.h"

#include <QObject>
#include <QString>
#include <QTest>

class Test_ATCAircraftTypeFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_getType();

private:
    QString DBpath = "E:/Qt/ATC_Console/ATC_Console/test/data";
};

#endif // TEST_ATCAIRCRAFTTYPEFACTORY_H
