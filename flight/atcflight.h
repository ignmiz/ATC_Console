#ifndef ATCFLIGHT_H
#define ATCFLIGHT_H

#include "atcflightplan.h"

#include <QString>

enum CM
{
    CM_C, CM_D, CM_L
};

enum FP
{
    FP_UD, FP_LD, FP_AP, FP_LA
};

enum RPM
{
    RPM_ON, RPM_OFF
};

enum SHM
{
    SHM_M, SHM_C
};

enum TRM
{
    TRM_L, TRM_H
};

enum AM
{
    AM_A, AM_D, AM_C
};

struct State
{
    double x;
    double y;
    double h;
    double v;
    double hdg;

    CM cm;
    FP fp;
    RPM rpm;
    SHM shm;
    TRM trm;
    AM am;
};

class ATCFlight
{
public:
    explicit ATCFlight(State s);
    explicit ATCFlight(State s, ATCFlightPlan *p);
    explicit ATCFlight(State s, ATCFlightPlan *p, QString sq);
    ~ATCFlight();

    ATCFlightPlan* getFlightPlan();
    QString getSquawk();
    State& getState();

    void setFlightPlan(ATCFlightPlan *fpl);
    void setSquawk(QString sq);
    void setState(State st);

private:
    ATCFlightPlan *plan;
    QString squawk;
    State state;

};

#endif // ATCFLIGHT_H
