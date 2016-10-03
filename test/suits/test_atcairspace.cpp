#include "test_atcairspace.h"

void Test_ATCAirspace::test_constructObject()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.getSectorVectorSize() == 1);
    QVERIFY(airspace.getSectorARTCCLowVectorSize() == 1);
    QVERIFY(airspace.getSectorARTCCHighVectorSize() == 1);
    QVERIFY(airspace.getSectorARTCCVectorSize() == 1);
    QVERIFY(airspace.getFixesVectorSize() == 2);
    QVERIFY(airspace.getVORsVectorSize() == 1);
    QVERIFY(airspace.getNDBsVectorSize() == 1);
    QVERIFY(airspace.getAirportsVectorSize() == 1);
    QVERIFY(airspace.getAirport(0)->getRunwayVectorSize() == 2);
    QVERIFY(airspace.getSIDsVectorSize() == 1);
    QVERIFY(airspace.getSTARsVectorSize() == 1);
    QVERIFY(airspace.getSIDSymbolsVectorSize() == 1);
    QVERIFY(airspace.getSTARSymbolsVectorSize() == 1);
    QVERIFY(airspace.getAirwayLowVectorSize() == 1);
    QVERIFY(airspace.getAirwayHighVectorSize() == 1);
}

void Test_ATCAirspace::test_coordsStringToDouble()
{
    ATCAirspace airspace(testSCT, testESE);

    QString coord = "N050.30.30.500";
    QVERIFY(airspace.coordsStringToDouble(coord) == (50 + 30.0/60.0 + 30.0/3600.0 + 500.0/1000.0/3600.0));

    coord = "S050.30.30.500";
    QVERIFY(airspace.coordsStringToDouble(coord) == -(50 + 30.0/60.0 + 30.0/3600.0 + 500.0/1000.0/3600.0));

    coord = "E050.30.30.500";
    QVERIFY(airspace.coordsStringToDouble(coord) == (50 + 30.0/60.0 + 30.0/3600.0 + 500.0/1000.0/3600.0));

    coord = "W050.30.30.500";
    QVERIFY(airspace.coordsStringToDouble(coord) == -(50 + 30.0/60.0 + 30.0/3600.0 + 500.0/1000.0/3600.0));
}

void Test_ATCAirspace::test_isValidCoordsFormat()
{
    ATCAirspace airspace(testSCT, testESE);

    QString coord = "N050.30.30.500";
    QVERIFY(airspace.isValidCoordsFormat(coord));

    coord = "E050.30.30.500";
    QVERIFY(airspace.isValidCoordsFormat(coord));

    coord = "S050.30.30.500";
    QVERIFY(airspace.isValidCoordsFormat(coord));

    coord = "W050.30.30.500";
    QVERIFY(airspace.isValidCoordsFormat(coord));

    coord = "E-050.30.30.500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050,30.30.500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050.30,30.500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050.30.30,500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050:30.30.500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050.30;30.500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));

    coord = "E050 30 30 500";
    QVERIFY(!airspace.isValidCoordsFormat(coord));
}

void Test_ATCAirspace::test_isValidNavaid()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isValidNavaid("ABAKU"));
    QVERIFY(!airspace.isValidNavaid("EVINA"));

    QVERIFY(airspace.isValidNavaid("BYZ"));
    QVERIFY(!airspace.isValidNavaid("OKC"));

    QVERIFY(airspace.isValidNavaid("CHO"));
    QVERIFY(!airspace.isValidNavaid("OE"));

    QVERIFY(airspace.isValidNavaid("EPBA"));
    QVERIFY(!airspace.isValidNavaid("EPBY"));
}

void Test_ATCAirspace::test_isFix()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isFix("ABAKU"));
    QVERIFY(!airspace.isFix("EVINA"));
}

void Test_ATCAirspace::test_isVOR()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isVOR("BYZ"));
    QVERIFY(!airspace.isVOR("OKC"));
}

void Test_ATCAirspace::test_isNDB()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isNDB("CHO"));
    QVERIFY(!airspace.isNDB("OE"));
}

void Test_ATCAirspace::test_isAirport()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isAirport("EPBA"));
    QVERIFY(!airspace.isAirport("EPBY"));
}

void Test_ATCAirspace::test_isAirwayLow()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isAirwayLow("L23"));
    QVERIFY(!airspace.isAirwayLow("X63"));
}

void Test_ATCAirspace::test_isAirwayHigh()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.isAirwayHigh("UL23"));
    QVERIFY(!airspace.isAirwayHigh("UX63"));
}

void Test_ATCAirspace::test_appendSector()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCAirspaceSector *sector = new ATCAirspaceSector("TEST");
    airspace.appendSector(sector);

    QVERIFY(airspace.getSectorVectorSize() == 2);
    QVERIFY(airspace.getLastSector()->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSectorARTCCLow()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCSectorARTCCLow *sector = new ATCSectorARTCCLow("TEST");
    airspace.appendSectorARTCCLow(sector);

    QVERIFY(airspace.getSectorARTCCLowVectorSize() == 2);
    QVERIFY(airspace.getLastSectorARTCCLow()->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSectorARTCCHigh()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCSectorARTCCHigh *sector = new ATCSectorARTCCHigh("TEST");
    airspace.appendSectorARTCCHigh(sector);

    QVERIFY(airspace.getSectorARTCCHighVectorSize() == 2);
    QVERIFY(airspace.getLastSectorARTCCHigh()->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSectorARTCC()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCSectorARTCC *sector = new ATCSectorARTCC("TEST");
    airspace.appendSectorARTCC(sector);

    QVERIFY(airspace.getSectorARTCCVectorSize() == 2);
    QVERIFY(airspace.getLastSectorARTCC()->getName() == "TEST");
}

void Test_ATCAirspace::test_appendFix()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCNavFix *fix = new ATCNavFix("TEST1", 50, 20);
    airspace.appendFix(fix);

    QVERIFY(airspace.getFixesVectorSize() == 3);
    QVERIFY(airspace.getFix(2)->getName() == "TEST1");
}

void Test_ATCAirspace::test_appendVOR()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCBeaconVOR *vor = new ATCBeaconVOR("TST", 112.000, 50, 20);
    airspace.appendVOR(vor);

    QVERIFY(airspace.getVORsVectorSize() == 2);
    QVERIFY(airspace.getVOR(1)->getName() == "TST");
}

void Test_ATCAirspace::test_appendNDB()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCBeaconNDB *ndb = new ATCBeaconNDB("TS", 1100, 50, 20);
    airspace.appendNDB(ndb);

    QVERIFY(airspace.getNDBsVectorSize() == 2);
    QVERIFY(airspace.getNDB(1)->getName() == "TS");
}

void Test_ATCAirspace::test_appendAirport()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCAirport *airport = new ATCAirport("EPTS", 50, 20);
    airspace.appendAirport(airport);

    QVERIFY(airspace.getAirportsVectorSize() == 2);
    QVERIFY(airspace.getAirport(1)->getName() == "EPTS");
}

void Test_ATCAirspace::test_appendSID()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCProcedureSID *sid = new ATCProcedureSID("TEST", "EPTS", "01");
    airspace.appendSID(sid);

    QVERIFY(airspace.getSIDsVectorSize() == 2);
    QVERIFY(airspace.getSID(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSTAR()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCProcedureSTAR *star = new ATCProcedureSTAR("TEST", "EPTS", "01");
    airspace.appendSTAR(star);

    QVERIFY(airspace.getSTARsVectorSize() == 2);
    QVERIFY(airspace.getSTAR(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSIDSymbol()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCProcedureSIDSymbol *sidSymbol = new ATCProcedureSIDSymbol("TEST");
    airspace.appendSIDSymbol(sidSymbol);

    QVERIFY(airspace.getSIDSymbolsVectorSize() == 2);
    QVERIFY(airspace.getSIDSymbol(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_appendSTARSymbol()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCProcedureSTARSymbol *starSymbol = new ATCProcedureSTARSymbol("TEST");
    airspace.appendSTARSymbol(starSymbol);

    QVERIFY(airspace.getSTARSymbolsVectorSize() == 2);
    QVERIFY(airspace.getSTARSymbol(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_appendAirwayLow()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCAirwayLow *airway = new ATCAirwayLow("TEST");
    airspace.appendAirwayLow(airway);

    QVERIFY(airspace.getAirwayLowVectorSize() == 2);
    QVERIFY(airspace.getAirwayLow(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_appendAirwayHigh()
{
    ATCAirspace airspace(testSCT, testESE);

    ATCAirwayHigh *airway = new ATCAirwayHigh("TEST");
    airspace.appendAirwayHigh(airway);

    QVERIFY(airspace.getAirwayHighVectorSize() == 2);
    QVERIFY(airspace.getAirwayHigh(1)->getName() == "TEST");
}

void Test_ATCAirspace::test_getSectorVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorVectorSize() == 1);
}

void Test_ATCAirspace::test_getSectorARTCCLowVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCCLowVectorSize() == 1);
}

void Test_ATCAirspace::test_getSectorARTCCHighVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCCHighVectorSize() == 1);
}

void Test_ATCAirspace::test_getSectorARTCCVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCCVectorSize() == 1);
}

void Test_ATCAirspace::test_getFixesVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getFixesVectorSize() == 2);
}

void Test_ATCAirspace::test_getVORsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getVORsVectorSize() == 1);
}

void Test_ATCAirspace::test_getNDBsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getNDBsVectorSize() == 1);
}

void Test_ATCAirspace::test_getAirportsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirportsVectorSize() == 1);
}

void Test_ATCAirspace::test_getSIDsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSIDsVectorSize() == 1);
}

void Test_ATCAirspace::test_getSTARsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSTARsVectorSize() == 1);
}

void Test_ATCAirspace::test_getSIDSymbolsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSIDSymbolsVectorSize() == 1);
}

void Test_ATCAirspace::test_getSTARSymbolsVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSTARSymbolsVectorSize() == 1);
}

void Test_ATCAirspace::test_getAirwayLowVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirwayLowVectorSize() == 1);
}

void Test_ATCAirspace::test_getAirwayHighVectorSize()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirwayHighVectorSize() == 1);
}

void Test_ATCAirspace::test_getSector()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSector(0)->getName() == "CTA5");
}

void Test_ATCAirspace::test_getSectorARTCCLow()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCCLow(0)->getName() == "EPSY_SEKTOR_A");
}

void Test_ATCAirspace::test_getSectorARTCCHigh()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCCHigh(0)->getName() == "TALON");
}

void Test_ATCAirspace::test_getSectorARTCC()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSectorARTCC(0)->getName() == "POLAND");
}

void Test_ATCAirspace::test_getFix()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getFix(0)->getName() == "ABAKU");
}

void Test_ATCAirspace::test_getVOR()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getVOR(0)->getName() == "BYZ");
}

void Test_ATCAirspace::test_getNDB()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getNDB(0)->getName() == "CHO");
}

void Test_ATCAirspace::test_getAirport()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirport(0)->getName() == "EPBA");
}

void Test_ATCAirspace::test_getSID()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSID(0)->getName() == "XIMBA4A");
}

void Test_ATCAirspace::test_getSTAR()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSTAR(0)->getName() == "BIMPA1S");
}

void Test_ATCAirspace::test_getSIDSymbol()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSIDSymbol(0)->getName() == "EPBA 01");
}

void Test_ATCAirspace::test_getSTARSymbol()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getSTARSymbol(0)->getName() == "EPBA 01");
}

void Test_ATCAirspace::test_getAirwayLow()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirwayLow(0)->getName() == "L23");
}

void Test_ATCAirspace::test_getAirwayHigh()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getAirwayHigh(0)->getName() == "UL23");
}

void Test_ATCAirspace::test_getSectorARTCCLowVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCSectorARTCCLow*> current(airspace.getSectorARTCCLowVector());

    QVERIFY(current.size() == airspace.getSectorARTCCLowVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getSectorARTCCLow(i));
    }
}

void Test_ATCAirspace::test_getSectorARTCCHighVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCSectorARTCCHigh*> current(airspace.getSectorARTCCHighVector());

    QVERIFY(current.size() == airspace.getSectorARTCCHighVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getSectorARTCCHigh(i));
    }
}

void Test_ATCAirspace::test_getSectorARTCCVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCSectorARTCC*> current(airspace.getSectorARTCCVector());

    QVERIFY(current.size() == airspace.getSectorARTCCVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getSectorARTCC(i));
    }
}

void Test_ATCAirspace::test_getFixesVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCNavFix*> current(airspace.getFixesVector());

    QVERIFY(current.size() == airspace.getFixesVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getFix(i));
    }
}

void Test_ATCAirspace::test_getNDBsVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCBeaconNDB*> current(airspace.getNDBsVector());

    QVERIFY(current.size() == airspace.getNDBsVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getNDB(i));
    }
}

void Test_ATCAirspace::test_getVORsVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCBeaconVOR*> current(airspace.getVORsVector());

    QVERIFY(current.size() == airspace.getVORsVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getVOR(i));
    }
}

void Test_ATCAirspace::test_getAirportsVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCAirport*> current(airspace.getAirportsVector());

    QVERIFY(current.size() == airspace.getAirportsVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getAirport(i));
    }
}

void Test_ATCAirspace::test_getSIDSymbolsVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCProcedureSIDSymbol*> current(airspace.getSIDSymbolsVector());

    QVERIFY(current.size() == airspace.getSIDSymbolsVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getSIDSymbol(i));
    }
}

void Test_ATCAirspace::test_getSTARSymbolsVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCProcedureSTARSymbol*> current(airspace.getSTARSymbolsVector());

    QVERIFY(current.size() == airspace.getSTARSymbolsVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getSTARSymbol(i));
    }
}

void Test_ATCAirspace::test_getAirwayLowVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCAirwayLow*> current(airspace.getAirwayLowVector());

    QVERIFY(current.size() == airspace.getAirwayLowVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getAirwayLow(i));
    }
}

void Test_ATCAirspace::test_getAirwayHighVector()
{
    ATCAirspace airspace(testSCT, testESE);

    QVector<ATCAirwayHigh*> current(airspace.getAirwayHighVector());

    QVERIFY(current.size() == airspace.getAirwayHighVectorSize());

    for(int i = 0; i < current.size(); i++)
    {
        QVERIFY(current.at(i) == airspace.getAirwayHigh(i));
    }
}

void Test_ATCAirspace::test_getLastSector()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSector()->getName() == "CTA5");
}

void Test_ATCAirspace::test_getLastSectorARTCCLow()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSectorARTCCLow()->getName() == "EPSY_SEKTOR_A");
}

void Test_ATCAirspace::test_getLastSectorARTCCHigh()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSectorARTCCHigh()->getName() == "TALON");
}

void Test_ATCAirspace::test_getLastSectorARTCC()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSectorARTCC()->getName() == "POLAND");
}

void Test_ATCAirspace::test_getLastSIDSymbol()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSIDSymbol()->getName() == "EPBA 01");
}

void Test_ATCAirspace::test_getLastSTARSymbol()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastSTARSymbol()->getName() == "EPBA 01");
}

void Test_ATCAirspace::test_getLastAirwayLow()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastAirwayLow()->getName() == "L23");
}

void Test_ATCAirspace::test_getLastAirwayHigh()
{
    ATCAirspace airspace(testSCT, testESE);
    QVERIFY(airspace.getLastAirwayHigh()->getName() == "UL23");
}

void Test_ATCAirspace::test_getNavaidLatitude()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.getNavaidLatitude("BYZ") == airspace.coordsStringToDouble("N053.05.54.300"));
    QVERIFY(airspace.getNavaidLatitude("CHO") == airspace.coordsStringToDouble("N053.28.31.999"));
    QVERIFY(airspace.getNavaidLatitude("ABAKU") == airspace.coordsStringToDouble("N051.40.37.000"));
    QVERIFY(airspace.getNavaidLatitude("EPBA") == airspace.coordsStringToDouble("N049.48.17.999"));
    QVERIFY(airspace.getNavaidLatitude("OTHER") == -200);
}

void Test_ATCAirspace::test_getNavaidLongitude()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.getNavaidLongitude("BYZ") == airspace.coordsStringToDouble("E017.58.18.200"));
    QVERIFY(airspace.getNavaidLongitude("CHO") == airspace.coordsStringToDouble("E015.19.59.000"));
    QVERIFY(airspace.getNavaidLongitude("ABAKU") == airspace.coordsStringToDouble("E019.04.53.000"));
    QVERIFY(airspace.getNavaidLongitude("EPBA") == airspace.coordsStringToDouble("E019.00.07.199"));
    QVERIFY(airspace.getNavaidLongitude("OTHER") == -200);
}

void Test_ATCAirspace::test_findSectorARTCCLow()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findSectorARTCCLow("EPSY_SEKTOR_A")->getName() == "EPSY_SEKTOR_A");
    QVERIFY(airspace.findSectorARTCCLow("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findSectorARTCCHigh()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findSectorARTCCHigh("TALON")->getName() == "TALON");
    QVERIFY(airspace.findSectorARTCCHigh("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findSectorARTCC()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findSectorARTCC("POLAND")->getName() == "POLAND");
    QVERIFY(airspace.findSectorARTCC("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findAirport()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findAirport("EPBA")->getName() == "EPBA");
    QVERIFY(airspace.findAirport("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findCentreline()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findCentreline("EPBA", "07") == airspace.findAirport("EPBA")->getRunway(0)->getExtendedCentreline1());
    QVERIFY(airspace.findCentreline("EPBA", "25") == airspace.findAirport("EPBA")->getRunway(0)->getExtendedCentreline2());
    QVERIFY(airspace.findCentreline("OTHER", "00") == nullptr);
}

void Test_ATCAirspace::test_findFix()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findFix("ABAKU")->getName() == "ABAKU");
    QVERIFY(airspace.findFix("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findVOR()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findVOR("BYZ")->getName() == "BYZ");
    QVERIFY(airspace.findVOR("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findNDB()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findNDB("CHO")->getName() == "CHO");
    QVERIFY(airspace.findNDB("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findSIDSymbol()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findSIDSymbol("EPBA 01")->getName() == "EPBA 01");
    QVERIFY(airspace.findSIDSymbol("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findSTARSymbol()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findSTARSymbol("EPBA 01")->getName() == "EPBA 01");
    QVERIFY(airspace.findSTARSymbol("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findAirwayLow()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findAirwayLow("L23")->getName() == "L23");
    QVERIFY(airspace.findAirwayLow("OTHER") == nullptr);
}

void Test_ATCAirspace::test_findAirwayHigh()
{
    ATCAirspace airspace(testSCT, testESE);

    QVERIFY(airspace.findAirwayHigh("UL23")->getName() == "UL23");
    QVERIFY(airspace.findAirwayHigh("OTHER") == nullptr);
}
