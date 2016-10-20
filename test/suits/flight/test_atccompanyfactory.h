#ifndef TEST_ATCCOMPANYFACTORY_H
#define TEST_ATCCOMPANYFACTORY_H

#include "atccompanyfactory.h"

#include <QTest>
#include <QObject>

class Test_ATCCompanyFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_newCompany();
};

#endif // TEST_ATCCOMPANYFACTORY_H
