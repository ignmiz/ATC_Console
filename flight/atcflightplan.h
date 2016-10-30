#ifndef ATCFLIGHTPLAN_H
#define ATCFLIGHTPLAN_H

#include "atcflags.h"

#include "atcaircrafttype.h"
#include "atcroute.h"
#include "atccompany.h"

#include <QTime>
#include <QString>

class ATCFlightPlan
{
public:
    explicit ATCFlightPlan();
    explicit ATCFlightPlan(ATC::FlightRules fr, ATCCompany *company, QString fltNo, ATCAircraftType *type, ATCRoute *route, int tas, int altitude, QTime depTime, QTime enrTime);
    ~ATCFlightPlan();

    ATC::FlightRules getFlightRules();
    ATCCompany* getCompany();
    QString getFlightNumber();
    ATCAircraftType* getType();
    ATCRoute& getRoute();
    int getTAS();
    int getAltitude();
    QTime getDepartureTime();
    QTime getEnrouteTime();

    void setFlightRules(ATC::FlightRules newRules);
    void setCompany(ATCCompany *newCompany);
    void setFlightNumber(QString newNo);
    void setType(ATCAircraftType *newType);
    void setRoute(ATCRoute &newRoute);
    void setTAS(int tas);
    void setAltitude(int alt);
    void setDepartureTime(QTime newDepTime);
    void setEnrouteTime(QTime newEnrTime);

private:
    ATC::FlightRules rules;
    ATCCompany *company;
    QString flightNumber;
    ATCAircraftType *type;
    ATCRoute route;

    int TAS;
    int altitude;
    QTime departureTime;
    QTime enrouteTime;
};

#endif // ATCFLIGHTPLAN_H
