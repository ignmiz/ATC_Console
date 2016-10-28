#ifndef TEST_ATCROUTEFACTORY_H
#define TEST_ATCROUTEFACTORY_H

#include "atcroutefactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QString>
#include <QTest>

class Test_ATCRouteFactory : public QObject
{
    Q_OBJECT

public:
    Test_ATCRouteFactory();
    ~Test_ATCRouteFactory();

private slots:
    void test_getRoute();

private:
    QString path;
};

#endif // TEST_ATCROUTEFACTORY_H
