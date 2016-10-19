#ifndef TEST_ATCAIRWAYLOW_H
#define TEST_ATCAIRWAYLOW_H

#include "atcairwaylow.h"

#include <QTest>
#include <QObject>

class Test_ATCAirwayLow : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
};

#endif // TEST_ATCAIRWAYLOW_H
