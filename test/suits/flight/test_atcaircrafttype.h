#ifndef TEST_ATCAIRCRAFTTYPE_H
#define TEST_ATCAIRCRAFTTYPE_H

#include "atcaircrafttype.h"

#include <QObject>
#include <QString>
#include <QTest>

class Test_ATCAircraftType : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();

private:
    QString APFpath = "E:/Qt/ATC_Console/ATC_Console/test/data/test_APF.APF";
    QString OPFpath = "E:/Qt/ATC_Console/ATC_Console/test/data/test_OPF.OPF";
};

#endif // TEST_ATCAIRCRAFTTYPE_H
