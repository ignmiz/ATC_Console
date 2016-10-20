#ifndef ATCCOMPANYFACTORY_H
#define ATCCOMPANYFACTORY_H

#include "atccompany.h"

#include <QFile>
#include <QString>
#include <QTextStream>

#include <QMessageBox>

class ATCCompanyFactory
{
public:
    explicit ATCCompanyFactory(QString path);
    ~ATCCompanyFactory();

    ATCCompany* newCompany();

    QString getCode(int i);
    QString getCallsign(int i);
    QString getName(int i);

private:
    QVector<QString> codes;
    QVector<QString> callsigns;
    QVector<QString> names;
};

#endif // ATCCOMPANYFACTORY_H
