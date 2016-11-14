#ifndef TEST_ATCCOMPANYFACTORY_H
#define TEST_ATCCOMPANYFACTORY_H

#include "atccompanyfactory.h"
#include "atcpaths.h"

#include <QTest>
#include <QObject>
#include <QString>

class Test_ATCCompanyFactory : public QObject
{
    Q_OBJECT
public:
    Test_ATCCompanyFactory();
    ~Test_ATCCompanyFactory();

private slots:
    void test_getCompany();
    void test_appendCompany();

private:
    QString path;
};

#endif // TEST_ATCCOMPANYFACTORY_H
