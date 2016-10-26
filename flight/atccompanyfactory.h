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

    ATCCompany* getCompany();
    ATCCompany* getCompany(int i);
    ATCCompany* getCompany(QString ICAOcode);

private:
    QVector<ATCCompany*> companies;
};

#endif // ATCCOMPANYFACTORY_H
