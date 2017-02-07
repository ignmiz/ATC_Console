#ifndef ATCFLIGHT_H
#define ATCFLIGHT_H

#include "atcflightplan.h"
#include "atcflighttag.h"
#include "atcrouteprediction.h"
#include "atcflags.h"

#include <QString>
#include <QObject>
#include <QPair>

struct State
{
    double x;
    double y;
    double h;
    double v;
    double hdg;

    BADA::ClimbMode cm;
    BADA::FlightPhase fp;
    BADA::ReducedPowerMode rpm;
    BADA::SpeedHoldMode shm;
    BADA::TroposphereMode trm;
    BADA::AccelerationMode am;
};

struct Temp //Structure for temporary data (such as mass) and simulation preallocated data members
{
    double m;

    double Cpowred;

    double vStallCR;
    double vStallIC;
    double vStallTO;
    double vStallAP;
    double vStallLD;

    double xoverAltClbM;
    double xoverAltCrsM;
    double xoverAltDesM;

    QVector<QPair<double, double>> waypoints;
};

class ATCFlight : public QObject
{
    Q_OBJECT

public:
    explicit ATCFlight(State s);
    explicit ATCFlight(State s, ATCFlightPlan *p);
    explicit ATCFlight(State s, ATCFlightPlan *p, QString sq);
    ~ATCFlight();

    ATCFlightPlan* getFlightPlan();
    QString getSquawk();
    QString getAssignedSquawk();
    State& getState();
    Temp& getTemp();
    ATC::NavMode getNavMode();
    QString getTargetAltitude();
    QString getTargetSpeed();
    int getHdgRestriction();
    QString getNextFix();
    QTime getSimStartTime();
    QStringList& getFixList();
    QStringList& getMainFixList();

    QString getRunwayDeparture();
    QString getRunwayDestination();
    QString getSID();
    QString getSTAR();

    ATCFlightTag* getFlightTag();
    ATCRoutePrediction* getRoutePrediction();

    void setFlightPlan(ATCFlightPlan *fpl);
    void setSquawk(QString sq);
    void setAssignedSquawk(QString sq);
    void setState(State st);
    void setTemp(Temp tp);
    void setNavMode(ATC::NavMode m);
    void setTargetAltitude(QString altitude);
    void setTargetSpeed(QString speed);
    void setHdgRestriction(int hdg);
    void setNextFix(QString fix);
    void setSimStartTime(QTime time);
    void setFixList(QStringList list);
    void setMainFixList(QStringList list);

    void setRunwayDeparture(QString rwy);
    void setRunwayDestination(QString rwy);
    void setSID(QString procedure);
    void setSTAR(QString procedure);

    void setFlightTag(ATCFlightTag *t);
    void setRoutePrediction(ATCRoutePrediction *pred);

signals:
    void signalCreateDialogAltitude(ATCFlight *flight, QPoint point);
    void signalCreateDialogSpeed(ATCFlight *flight, QPoint point);
    void signalCreateDialogHeading(ATCFlight *flight, QPoint point);
    void signalCreateDialogRoute(ATCFlight *flight, QPoint point);
    void signalCreateDialogHandoff(ATCFlight *flight, QPoint point);
    void signalCreateDialogFlightPlan(ATCFlight *flight);
    void signalDisplayRoute(ATCFlight *flight);
    void signalClearFlightElements(ATCFlight *flight);

public slots:
    void slotDisplayRoute();

private slots:
    void slotCreateDialogAltitude(QPoint point);
    void slotCreateDialogSpeed(QPoint point);
    void slotCreateDialogHeading(QPoint point);
    void slotCreateDialogRoute(QPoint point);
    void slotCreateDialogHandoff(QPoint point);
    void slotCreateDialogFlightPlan();

private:
    ATCFlightPlan *plan = nullptr;
    QString squawk;
    QString assignedSquawk;
    State state;
    Temp temp;

    ATCFlightTag *tag = nullptr;
    ATCRoutePrediction *prediction = nullptr;

    ATC::NavMode mode;
    QString targetAltitude;
    QString targetSpeed;

    int hdgRestriction;
    QString nextFix;
    QStringList fixList;        //List with SID & STAR fixes
    QStringList mainFixList;    //Only filed route fixes

    QTime simStart;

    QString depRwy;
    QString desRwy;
    QString sid;
    QString star;
};

#endif // ATCFLIGHT_H
