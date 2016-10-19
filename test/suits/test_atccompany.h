#ifndef TEST_ATCCOMPANY_H
#define TEST_ATCCOMPANY_H

#include "atccompany.h"

#include <QTest>
#include <QObject>

class Test_ATCCompany : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getCode();
    void test_getCallsign();
    void test_getName();
    void test_setCode();
    void test_setCallsign();
    void test_setName();

};

#endif // TEST_ATCCOMPANY_H
