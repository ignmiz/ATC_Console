#ifndef ATCAIRCRAFTTYPEFACTORY_H
#define ATCAIRCRAFTTYPEFACTORY_H

#include "atcaircrafttype.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfoList>
#include <QString>

class ATCAircraftTypeFactory
{
public:
    explicit ATCAircraftTypeFactory(QString DBdir);
    ~ATCAircraftTypeFactory();

    ATCAircraftType* getType();
    ATCAircraftType* getType(int i);
    ATCAircraftType* getType(QString ICAOcode);

private:
    QVector<ATCAircraftType*> types;
};

#endif // ATCAIRCRAFTTYPEFACTORY_H
