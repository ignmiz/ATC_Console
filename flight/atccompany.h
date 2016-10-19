#ifndef ATCCOMPANY_H
#define ATCCOMPANY_H

#include <QString>

class ATCCompany
{
public:
    explicit ATCCompany(QString code, QString callsign, QString name);
    ~ATCCompany();

    QString getCode();
    QString getCallsign();
    QString getName();

    void setCode(QString string);
    void setCallsign(QString string);
    void setName(QString string);

private:
    QString code;
    QString callsign;
    QString name;


};

#endif // ATCCOMPANY_H
