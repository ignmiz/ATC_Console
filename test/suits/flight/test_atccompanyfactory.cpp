
#include "test_atccompanyfactory.h"

void Test_ATCCompanyFactory::test_constructObject()
{
    ATCCompanyFactory companyFactory("E:/Qt/ATC_Console/ATC_Console/test/data/test_company.txt");

    QVERIFY(companyFactory.getCode(0) == "AAA");
    QVERIFY(companyFactory.getCode(1) == "AAB");
    QVERIFY(companyFactory.getCode(2) == "AAC");

    QVERIFY(companyFactory.getCallsign(0) == "TSTCALL1");
    QVERIFY(companyFactory.getCallsign(1) == "TSTCALL2");
    QVERIFY(companyFactory.getCallsign(2) == "TSTCALL3");

    QVERIFY(companyFactory.getName(0) == "Test Name 1");
    QVERIFY(companyFactory.getName(1) == "Test Name 2");
    QVERIFY(companyFactory.getName(2) == "Test Name 3");
}

void Test_ATCCompanyFactory::test_newCompany()
{
    ATCCompanyFactory companyFactory("E:/Qt/ATC_Console/ATC_Console/test/data/test_company.txt");

    for(int i = 0; i < 100; i++)
    {
        ATCCompany *company = companyFactory.newCompany();

        QVERIFY(((company->getCode() == "AAA") && (company->getCallsign() == "TSTCALL1") && (company->getName() == "Test Name 1")) ||
                ((company->getCode() == "AAB") && (company->getCallsign() == "TSTCALL2") && (company->getName() == "Test Name 2")) ||
                ((company->getCode() == "AAC") && (company->getCallsign() == "TSTCALL3") && (company->getName() == "Test Name 3")));

        delete company;
    }
}
