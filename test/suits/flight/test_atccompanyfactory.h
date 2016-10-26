#ifndef TEST_ATCCOMPANYFACTORY_H
#define TEST_ATCCOMPANYFACTORY_H

#include "atccompanyfactory.h"

#include <QTest>
#include <QObject>
#include <QString>

class Test_ATCCompanyFactory : public QObject
{
    Q_OBJECT

private slots:
    void test_newCompany();

private:
    QString path = "E:/Qt/ATC_Console/ATC_Console/test/data/test_company.txt";
};

#endif // TEST_ATCCOMPANYFACTORY_H
