#ifndef TEST_ATCAIRCRAFTTYPE_H
#define TEST_ATCAIRCRAFTTYPE_H

#include "atcaircrafttype.h"
#include "atcpaths.h"

#include <QObject>
#include <QString>
#include <QTest>

class Test_ATCAircraftType : public QObject
{
    Q_OBJECT

public:
    Test_ATCAircraftType();
    ~Test_ATCAircraftType();

private slots:
    void test_constructObject();

private:
    QString APFpath;
    QString OPFpath;
};

#endif // TEST_ATCAIRCRAFTTYPE_H
