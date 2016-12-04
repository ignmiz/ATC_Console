#ifndef ATCMATH_H
#define ATCMATH_H

#include "atcconst.h"
#include "atcflags.h"

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

    static bool compareDouble(double numActual, double numModel, double allowedError);

    static ISA atmosISA(double h);

    static double crossoverAltitude(double CAS, double M);

    static double normalizeAngle(double angle, ATC::AngularUnits unitType);

    static double ESF();                    //DUMMY
    static double projectAcftPosOnPath();   //DUMMY

private:

};

#endif // ATCMATH_H
