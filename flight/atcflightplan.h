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
    explicit ATCFlightPlan(ATC::FlightRules fr, ATCCompany *company, QString fltNo, ATCAircraftType *type, ATCRoute *route, int tas, QString altitude, QTime depTime, QTime enrTime, QTime fuelTime);
    ~ATCFlightPlan();

    ATC::FlightRules getFlightRules();
    ATCCompany* getCompany();
    QString getFlightNumber();
    ATCAircraftType* getType();
    ATCRoute& getRoute();
    int getTAS();
    QString getAltitude();
    QTime getDepartureTime();
    QTime getEnrouteTime();
    QTime getFuelTime();

    void setFlightRules(ATC::FlightRules newRules);
    void setCompany(ATCCompany *newCompany);
    void setFlightNumber(QString newNo);
    void setType(ATCAircraftType *newType);
    void setRoute(ATCRoute &newRoute);
    void setTAS(int tas);
    void setAltitude(QString alt);
    void setDepartureTime(QTime newDepTime);
    void setEnrouteTime(QTime newEnrTime);
    void setFuelTime(QTime newFuelTime);

private:
    ATC::FlightRules rules;
    ATCCompany *company;
    QString flightNumber;
    ATCAircraftType *type;
    ATCRoute route;

    int TAS;
    QString altitude;
    QTime departureTime;
    QTime enrouteTime;
    QTime fuelTime;
};

#endif // ATCFLIGHTPLAN_H
