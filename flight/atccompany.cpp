
#include "atccompany.h"


ATCCompany::ATCCompany(QString code, QString callsign, QString name) :
    code(code.left(3).toUpper()),
    callsign(callsign.toUpper()),
    name(name)
{

}

ATCCompany::~ATCCompany()
{

}

QString ATCCompany::getCode()
{
    return code;
}

QString ATCCompany::getCallsign()
{
    return callsign;
}

QString ATCCompany::getName()
{
    return name;
}

void ATCCompany::setCode(QString string)
{
    code = string.left(3).toUpper();
}

void ATCCompany::setCallsign(QString string)
{
    callsign = string.toUpper();
}

void ATCCompany::setName(QString string)
{
    name = string;
}
