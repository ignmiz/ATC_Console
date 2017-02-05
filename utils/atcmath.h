#ifndef ATCMATH_H
#define ATCMATH_H

#include "atcconst.h"
#include "atcflags.h"
#include "atcaircrafttype.h"
#include "atcflight.h"

#include "GeographicLib/Geodesic.hpp"

#include <QDateTime>
#include <QtMath>

struct ISA
{
    double a;
    double rho;
    double p;
    double T;
};

class ATCMath
{
public:
    explicit ATCMath();
    ~ATCMath();

    //Conversion Functions
    static double rad2deg(double varRAD);
    static double deg2rad(double varDEG);

    static double m2nm(double varM);
    static double nm2m(double varNM);

    static double m2ft(double varM);
    static double ft2m(double varFT);

    static double mps2kt(double varMPS);
    static double kt2mps(double varKT);

    static double cas2tas(double casMPS, double p, double rho);
    static double cas2mach(double casMPS, double p, double rho, double a);

    static double tas2cas(double tasMPS, double p, double rho);
    static double tas2mach(double tasMPS, double a);

    static double mach2tas(double mach, double a);
    static double mach2cas(double mach, double p, double rho, double a);

    //Comparison Functions
    static bool compareDouble(double numActual, double numModel, double allowedError);

    //General
    static double min(double a, double b);
    static double max(double a, double b);

    //Simulation Functions
    static ISA atmosISA(double h);

    static double crossoverAltitude(double casMPS, double M);
    static double normalizeAngle(double angle, ATC::AngularUnits unitType);

    static double ESF(BADA::ClimbMode cm, BADA::AccelerationMode am, BADA::SpeedHoldMode shm, BADA::TroposphereMode trm, double mach, double temp, double dTemp);
    static double pathAngle(double thrust, double drag, double ESF, double m);
    static double bankAngle(double k1, double k2, double errorXTrack, double errorHdg, double bankLimitRad);

    static double DTA(double vMPS, double bankLimitRad, double dHdgRad, double flyOverDstM);
    static void projectAcftPosOnPath(GeographicLib::Geodesic &geo, double rtFix1Lat, double rtFix1Lon, double rtFix2Lat, double rtFix2Lon, double acftLat, double acftLon, double acftHdg, double &xtrackError, double &headingError, double &dstToNext);
    static void normalizeHdgError(double &hdgErrorRad);

    static double randomMass(int mMin, int mMax);
    static double recalculateSpeed(double vRef, double m, double mRef);
    static double recalculateReductionFactor(double Cred, double m, double mMin, double mMax);

    static double nominalSpeedCL(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vStallTO, double vCL1, double vCL2, double machCL, double a, double rho, double p);
    static double nominalSpeedCR(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vCR1, double vCR2, double machCR, double a, double rho, double p);
    static double nominalSpeedDS(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vStallLD, double vDS1, double vDS2, double machDS, double a, double rho, double p);
    static double nominalSpeed2tas(double vNom, BADA::SpeedHoldMode shm, double a, double rho, double p);

    static double liftCoefficient(double m, double rho, double tasMPS, double S, double phiRad);
    static double dragCoefficient(double CL, double CD0, double CD2, double dCD0);

    static double lift(double rho, double tasMPS, double S, double CL);
    static double drag(double rho, double tasMPS, double S, double CD);
    static double thrust(double tasMPS, double currentAltM, double drag, BADA::ClimbMode cm, BADA::AccelerationMode am, BADA::ReducedPowerMode rpm, ATC::EngineType engine, double CTc1, double CTc2, double CTc3, double CTdes, double Cpowred);

    static double calculateState();     //DUMMY

    static BADA::ClimbMode assignCM(double currentAltM, double targetAltM);
    static BADA::FlightPhase assignFP(double currentAltM, double altDesM, double tasMPS, double Vdes1KT, double p, double rho);
    static BADA::ReducedPowerMode assignRPM(double currentAltM, double altMax);
    static BADA::SpeedHoldMode assignSHM(double currentAltM, BADA::ClimbMode cm, double xoverAltClbM, double xoverAltCrsM, double xoverAltDesM);
    static BADA::TroposphereMode assignTRM(double currentAltM);
    static BADA::AccelerationMode assignAM(double tasMPS, double tasNomMPS);

private:

};

#endif // ATCMATH_H
