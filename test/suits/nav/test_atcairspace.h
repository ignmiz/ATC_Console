#ifndef TEST_ATCAIRSPACE_H
#define TEST_ATCAIRSPACE_H

#include "atcairspace.h"

#include <QTest>
#include <QObject>

class Test_ATCAirspace : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();

    void test_coordsStringToDouble();
    void test_isValidCoordsFormat();

    void test_isValidNavaid();
    void test_isFix();
    void test_isVOR();
    void test_isNDB();
    void test_isAirport();
    void test_isAirwayLow();
    void test_isAirwayHigh();

    void test_appendSector();
    void test_appendSectorARTCCLow();
    void test_appendSectorARTCCHigh();
    void test_appendSectorARTCC();
    void test_appendFix();
    void test_appendVOR();
    void test_appendNDB();
    void test_appendAirport();
    void test_appendSID();
    void test_appendSTAR();
    void test_appendSIDSymbol();
    void test_appendSTARSymbol();
    void test_appendAirwayLow();
    void test_appendAirwayHigh();

    void test_getSectorVectorSize();
    void test_getSectorARTCCLowVectorSize();
    void test_getSectorARTCCHighVectorSize();
    void test_getSectorARTCCVectorSize();
    void test_getFixesVectorSize();
    void test_getVORsVectorSize();
    void test_getNDBsVectorSize();
    void test_getAirportsVectorSize();
    void test_getSIDsVectorSize();
    void test_getSTARsVectorSize();
    void test_getSIDSymbolsVectorSize();
    void test_getSTARSymbolsVectorSize();
    void test_getAirwayLowVectorSize();
    void test_getAirwayHighVectorSize();

    void test_getSector();
    void test_getSectorARTCCLow();
    void test_getSectorARTCCHigh();
    void test_getSectorARTCC();
    void test_getFix();
    void test_getVOR();
    void test_getNDB();
    void test_getAirport();
    void test_getSID();
    void test_getSTAR();
    void test_getSIDSymbol();
    void test_getSTARSymbol();
    void test_getAirwayLow();
    void test_getAirwayHigh();

    void test_getSectorARTCCLowVector();
    void test_getSectorARTCCHighVector();
    void test_getSectorARTCCVector();
    void test_getFixesVector();
    void test_getNDBsVector();
    void test_getVORsVector();
    void test_getAirportsVector();
    void test_getSIDSymbolsVector();
    void test_getSTARSymbolsVector();
    void test_getAirwayLowVector();
    void test_getAirwayHighVector();

    void test_getLastSector();
    void test_getLastSectorARTCCLow();
    void test_getLastSectorARTCCHigh();
    void test_getLastSectorARTCC();
    void test_getLastSIDSymbol();
    void test_getLastSTARSymbol();
    void test_getLastAirwayLow();
    void test_getLastAirwayHigh();

    void test_getNavaidLatitude();
    void test_getNavaidLongitude();

    void test_findSectorARTCCLow();
    void test_findSectorARTCCHigh();
    void test_findSectorARTCC();
    void test_findAirport();
    void test_findCentreline();
    void test_findFix();
    void test_findVOR();
    void test_findNDB();
    void test_findSIDSymbol();
    void test_findSTARSymbol();
    void test_findAirwayLow();
    void test_findAirwayHigh();


private:
    const QString testSCT = "E:/Qt/ATC_Console/ATC_Console/test/data/test.sct";
    const QString testESE = "E:/Qt/ATC_Console/ATC_Console/test/data/test.ese";

};

#endif // TEST_ATCAIRSPACE_H
