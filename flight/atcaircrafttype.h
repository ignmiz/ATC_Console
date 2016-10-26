#ifndef ATCAIRCRAFTTYPE_H
#define ATCAIRCRAFTTYPE_H

#include "atcflags.h"

#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

struct AcType
{
    QString ICAOcode;
    int engineCount;
    ATC::EngineType engineType;
    ATC::WakeCategory wake;
};

struct Mass
{
    double ref;
    double min;
    double max;
    double payload;
    double gradient;
};

struct Envelope
{
    double VMO;
    double MMO;
    double h_max;
    double h_max_MTOW;
    double tempGradient;
};

struct Buffeting
{
    double Clbo;
    double K;
    double CM16;
};

struct Aero
{
    double V_stall;
    double CD0;
    double CD2;
};

struct Thrust
{
    double C_Tc1;
    double C_Tc2;
    double C_Tc3;
    double C_Tc4;
    double C_Tc5;

    double C_Tdes_low;
    double C_Tdes_high;
    double C_Tdes_app;
    double C_Tdes_ldg;

    double h_pdes;

};

struct Fuel
{
    double C_f1;
    double C_f2;
    double C_f3;
    double C_f4;
    double C_fcr;
};

struct Ground
{
    double TOL;
    double LDL;
    double span;
    double length;
};

struct Velocity
{
    double V_CL1_LO;
    double V_CL2_LO;
    double M_CL_LO;

    double V_CR1_LO;
    double V_CR2_LO;
    double M_CR_LO;

    double V_DS1_LO;
    double V_DS2_LO;
    double M_DS_LO;

    double V_CL1_AV;
    double V_CL2_AV;
    double M_CL_AV;

    double V_CR1_AV;
    double V_CR2_AV;
    double M_CR_AV;

    double V_DS1_AV;
    double V_DS2_AV;
    double M_DS_AV;

    double V_CL1_HI;
    double V_CL2_HI;
    double M_CL_HI;

    double V_CR1_HI;
    double V_CR2_HI;
    double M_CR_HI;

    double V_DS1_HI;
    double V_DS2_HI;
    double M_DS_HI;
};

class ATCAircraftType
{
public:
    explicit ATCAircraftType(QString OPFpath, QString APFpath);
    ~ATCAircraftType();

    AcType getAcType();
    Mass getMass();
    Envelope getEnvelope();
    Buffeting getBuffeting();
    Aero getAeroCR();
    Aero getAeroIC();
    Aero getAeroTO();
    Aero getAeroAP();
    Aero getAeroLD();
    Thrust getThrust();
    Fuel getFuel();
    Ground getGround();
    Velocity getVelocity();

    double getSurface();
    double getCDldg();

private:
    void interpretOPF(QString OPFpath);
    void interpretAPF(QString APFpath);

    QString OPFpath;
    QString APFpath;

    AcType actype;
    Mass mass;
    Envelope envelope;
    Buffeting buffeting;
    Aero aeroCR, aeroIC, aeroTO, aeroAP, aeroLD;
    Thrust thrust;
    Fuel fuel;
    Ground ground;
    Velocity velocity;

    double S;
    double dCD_ldg;


};

#endif // ATCAIRCRAFTTYPE_H
