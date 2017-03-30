#ifndef ATCFLIGHT_H
#define ATCFLIGHT_H

#include "atcflightplan.h"
#include "atcflighttag.h"
#include "atcrouteprediction.h"
#include "atcflags.h"
#include "atctrailingdot.h"
#include "atcprofileclimb.h"
#include "atcprofiledescent.h"

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

struct Temp //Structure for temporary, constant data members preallocated before simulation
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

    QPair<double, double> rwyDesThr;
    QPair<double, double> rwyDesAppRange;
    double rwyDesAzimuth;

    double routeDistance;
};

class ATCFlight : public QObject
{
    Q_OBJECT

public:
    explicit ATCFlight();
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

    QVector<QPair<double, double>>& getWaypoints();
    QVector<QPair<double, double>>& getProjectedWaypoints();
    QPair<double, double> getWaypoint(int i);
    QPair<double, double> getProjectedWaypoint(int i);
    int getWaypointsVectorSize();
    int getProjectedWaypointsVectorSize();
    int getWaypointIndex();
    bool isDCT();

    bool isCldFinalApp();
    bool isFinalApp();
    bool isGlidePath();
    double getAppTargetAltitude();

    bool isSimulated();

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

    void appendWaypoint(QPair<double, double> wp);
    void appendProjectedWaypoint(QPair<double, double> wp);
    void setWaypointIndex(int i);
    void setDCT(bool flag);

    void setCldFinalApp(bool flag);
    void setFinalApp(bool flag);
    void setGlidePath(bool flag);
    void setAppTargetAltitude(double altitudeM);

    void setSimulated(bool flag);

    void setTempFlag(bool flag);
    bool hasTemp();

    QVector<ATCTrailingDot*>& getTrailingDots();
    ATCTrailingDot* getTrailingDot(int i);
    int getTrailingDotsVectorSize();
    void appendTrailingDot(ATCTrailingDot *dot);
    void setTrailingDots(QVector<ATCTrailingDot*> &vector);
    void removeOldestDot();
    void rescaleDots();

    void clearWaypoints();

    void setDataLog(QString path);
    void closeDataLog();
    void logData(QString buffer);

    ATCProfileClimb* getProfileClimb();
    void setProfileClimb(ATCProfileClimb *profile);

    ATCProfileDescent* getProfileDescent();
    void setProfileDescent(ATCProfileDescent *profile);

    double getLegDistance(int i);
    int getLegDistanceVectorSize();
    void appendLegDistance(double dst);
    void clearLegDistances();

    double getLegAngleChange(int i);
    int getLegAngleChangeVectorSize();
    void appendLegAngleChange(double chng);
    void clearLegAngleChanges();

    bool hasAccuratePrediction();
    void setAccuratePredictionFlag(bool flag);

    void setDistanceToGo(double dst);
    double getDistanceToGo();

    void setDistanceToNext(double dst);
    double getDistanceToNext();

    void setTOC(double dst);
    double getTOC();

    void setTOCposition(QPair<double, double> &pos);
    QPair<double, double>& getTOCposition();

    void setTOD(double dst);
    double getTOD();

    void setTODposition(QPair<double, double> &pos);
    QPair<double, double>& getTODposition();

    void setTopLevel(double lvlM);
    double getTopLevel();

signals:
    void signalCreateDialogAltitude(ATCFlight *flight, QPoint point);
    void signalCreateDialogSpeed(ATCFlight *flight, QPoint point);
    void signalCreateDialogHeading(ATCFlight *flight, QPoint point);
    void signalCreateDialogRoute(ATCFlight *flight, QPoint point);
    void signalCreateDialogHandoff(ATCFlight *flight, QPoint point);
    void signalCreateDialogFlightPlan(ATCFlight *flight);
    void signalDisplayRoute(ATCFlight *flight);
    void signalUpdateRoute(ATCFlight *flight);
    void signalClearRoute(ATCFlight *flight);
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
    bool allocatedTemp = false;

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

    QVector<QPair<double, double>> waypoints;          //Format: lat, lon
    QVector<QPair<double, double>> projectedWaypoints; //Format: x, y
    int waypointIndex;
    bool dct = false;

    bool cldFinalApp = false;
    bool finalApp = false;
    bool glidePath = false;
    double appTargetAltitude;

    bool simulated = true;

    QVector<ATCTrailingDot*> trailingDots;

    bool dataLogged = false;
    QFile logFile;
    QTextStream logStream;

    ATCProfileClimb *profileClimb = nullptr;
    ATCProfileDescent *profileDescent = nullptr;

    QVector<double> legDistances;
    QVector<double> legAngleChanges;

    bool accuratePrediction = false;

    double distanceToGo;
    double distanceToNext = 0;

    double TOC = 0;
    QPair<double, double> TOCposition;  //Format: lat, lon

    double TOD = 0;
    QPair<double, double> TODposition;  //Format: lat, lon

    double topLevel;
};

#endif // ATCFLIGHT_H
