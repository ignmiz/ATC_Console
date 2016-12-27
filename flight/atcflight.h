#ifndef ATCFLIGHT_H
#define ATCFLIGHT_H

#include "atcflightplan.h"
#include "atcflighttag.h"
#include "atcrouteprediction.h"

#include <QString>
#include <QObject>

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
    ATC::NavMode getNavMode();
    QString getTargetAltitude();
    QString getTargetSpeed();
    int getHdgRestriction();
    QString getNextFix();
    QTime getSimStartTime();
    QStringList& getFixList();

    ATCFlightTag* getFlightTag();
    ATCRoutePrediction* getRoutePrediction();

    void setFlightPlan(ATCFlightPlan *fpl);
    void setSquawk(QString sq);
    void setAssignedSquawk(QString sq);
    void setState(State st);
    void setNavMode(ATC::NavMode m);
    void setTargetAltitude(QString altitude);
    void setTargetSpeed(QString speed);
    void setHdgRestriction(int hdg);
    void setNextFix(QString fix);
    void setSimStartTime(QTime time);
    void setFixList(QStringList list);

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

private slots:
    void slotCreateDialogAltitude(QPoint point);
    void slotCreateDialogSpeed(QPoint point);
    void slotCreateDialogHeading(QPoint point);
    void slotCreateDialogRoute(QPoint point);
    void slotCreateDialogHandoff(QPoint point);
    void slotCreateDialogFlightPlan();
    void slotDisplayRoute();

private:
    ATCFlightPlan *plan = nullptr;
    QString squawk;
    QString assignedSquawk;
    State state;

    ATCFlightTag *tag = nullptr;
    ATCRoutePrediction *prediction = nullptr;

    ATC::NavMode mode;
    QString targetAltitude;
    QString targetSpeed;

    int hdgRestriction;
    QString nextFix;
    QStringList fixList;

    QTime simStart;

};

#endif // ATCFLIGHT_H
