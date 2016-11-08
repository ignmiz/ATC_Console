#ifndef TEST_ATCAIRCRAFTTYPEFACTORY_H
#define TEST_ATCAIRCRAFTTYPEFACTORY_H

#include "atcaircrafttypefactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QString>
#include <QTest>

class Test_ATCAircraftTypeFactory : public QObject
{
    Q_OBJECT
public:
    Test_ATCAircraftTypeFactory();
    ~Test_ATCAircraftTypeFactory();

private slots:
    void test_getType();
    void test_getTypeVectorSize();

private:
    QString DBpath;
};

#endif // TEST_ATCAIRCRAFTTYPEFACTORY_H
