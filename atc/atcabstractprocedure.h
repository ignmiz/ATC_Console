#ifndef ATCABSTRACTPROCEDURE_H
#define ATCABSTRACTPROCEDURE_H

#include "atcnavfix.h"

#include <QString>
#include <QVector>

class ATCAbstractProcedure
{
public:
    explicit ATCAbstractProcedure(QString procedureName);
    virtual ~ATCAbstractProcedure() = 0;

    QString getName();
    QString getAirport();
    QString getRunwayID();
    ATCNavFix* getFix(int iterator);
    int getFixListSize();

    void setName(QString procedureName);
    void setAirport(QString airportCode);
    void setRunwayID(QString runway);

    void appendFix(ATCNavFix* fix);

private:
    QString name;
    QString airport;
    QString runwayID;
    QVector<ATCNavFix*> fixList;
};

#endif // ATCABSTRACTPROCEDURE_H
