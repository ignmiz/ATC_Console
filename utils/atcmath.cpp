
#include "atcmath.h"

double ATCMath::rad2deg(double varRAD)
{
    return varRAD * 180 / ATCConst::PI;
}

double ATCMath::deg2rad(double varDEG)
{
    return varDEG * ATCConst::PI / 180;
}

double ATCMath::m2nm(double varM)
{
    return varM / 1852;
}

double ATCMath::nm2m(double varNM)
{
    return varNM * 1852;
}

double ATCMath::m2ft(double varM)
{
    return varM / 0.3048;
}

double ATCMath::ft2m(double varFT)
{
    return varFT * 0.3048;
}

double ATCMath::mps2kt(double varMPS)
{
    return varMPS * 1.94384449;
}

double ATCMath::kt2mps(double varKT)
{
    return varKT * 0.51444444;
}

double ATCMath::cas2tas(double casMPS, double p, double rho)
{
    double p0 = 101325;
    double rho0 = 1.225;
    double kappa = 1.4;
    double mi = (kappa - 1) / kappa;

    double tasMPS = qPow(2/mi * p/rho * (qPow(1 + p0/p * (qPow(1 + mi/2 * rho0/p0 * qPow(casMPS, 2), 1/mi) - 1), mi) -1), 0.5);

    return tasMPS;
}

double ATCMath::cas2mach(double casMPS, double p, double rho, double a)
{
    return ATCMath::cas2tas(casMPS, p, rho) / a;
}

double ATCMath::tas2cas(double tasMPS, double p, double rho)
{
    double p0 = 101325;
    double rho0 = 1.225;
    double kappa = 1.4;
    double mi = (kappa - 1) / kappa;

    double casMPS = qPow(2/mi * p0/rho0 * (qPow(1 + p/p0 * (qPow(1 + mi/2 * rho/p * qPow(tasMPS, 2), 1/mi) - 1), mi) -1), 0.5);

    return casMPS;
}

double ATCMath::tas2mach(double tasMPS, double a)
{
    return tasMPS / a;
}

double ATCMath::mach2tas(double mach, double a)
{
    return mach * a;
}

double ATCMath::mach2cas(double mach, double p, double rho, double a)
{
    return ATCMath::tas2cas(mach * a, p, rho);
}

bool ATCMath::compareDouble(double numActual, double numModel, double allowedError)
{
    if(numModel != 0)
        if((numActual >= numModel - qFabs(numModel) * allowedError) && (numActual <= numModel + qFabs(numModel) * allowedError))
        {
            return true;
        }
        else
        {
            return false;
        }
    else
    {
        if((numActual >= numModel - allowedError) && (numActual <= numModel + allowedError))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

ISA ATCMath::atmosISA(double h)
{
    ISA isa;

    double betaTrop = -0.0065;
    double betaStrat1 = 0.001;
    double betaStrat2 = 0.0028;
    double betaMeso1 = -0.0028;
    double betaMeso2 = -0.002;

    double kappa = 1.4;
    double R = 287.05287;

    double gN = 9.80665;

    //Calculate temperature
    if((h >= -2000) && (h < 11000))
    {
        isa.T = 288.15 + betaTrop * (h - 0);
    }
    else if((h >= 11000) && (h < 20000))
    {
        isa.T = 216.65;
    }
    else if((h >= 20000) && (h < 32000))
    {
        isa.T = 216.65 + betaStrat1 * (h - 20000);
    }
    else if((h >= 32000) && (h < 47000))
    {
        isa.T = 228.65 + betaStrat2 * (h - 32000);
    }
    else if((h >= 47000) && (h < 51000))
    {
        isa.T = 270.65;
    }
    else if((h >= 51000) && (h < 71000))
    {
        isa.T = 270.65 + betaMeso1 * (h - 51000);
    }
    else if((h >= 71000) && (h <= 80000))
    {
        isa.T = 214.65 + betaMeso2 * (h - 71000);
    }
    else
    {
        isa.T = -1;
    }

    //Calculate pressure
    if((h >= -2000) && (h < 11000))
    {
        isa.p = 101325 * qPow((1 + betaTrop/288.15 * (h - 0)), -gN/(betaTrop * R));
    }
    else if((h >= 11000) && (h < 20000))
    {
        isa.p = 22632.1 * qExp(-gN/(R * isa.T) * (h - 11000));
    }
    else if((h >= 20000) && (h < 32000))
    {
        isa.p = 5474.89 * qPow((1 + betaStrat1/216.65 * (h - 20000)), -gN/(betaStrat1 * R));
    }
    else if((h >= 32000) && (h < 47000))
    {
        isa.p = 868.019 * qPow((1 + betaStrat2/228.65 * (h - 32000)), -gN/(betaStrat2 * R));
    }
    else if((h >= 47000) && (h < 51000))
    {
        isa.p = 110.906 * qExp(-gN/(R * isa.T) * (h - 47000));
    }
    else if((h >= 51000) && (h < 71000))
    {
        isa.p = 66.9389 * qPow((1 + betaMeso1/270.65 * (h - 51000)), -gN/(betaMeso1 * R));
    }
    else if((h >= 71000) && (h <= 80000))
    {
        isa.p = 3.95642 * qPow((1 + betaMeso2/214.65 * (h - 71000)), -gN/(betaMeso2 * R));
    }
    else
    {
        isa.p = -1;
    }

    //Calculate air density
    if((h >= -2000) && (h <= 80000))
    {
        isa.rho = isa.p / (R * isa.T);
    }
    else
    {
        isa.rho = -1;
    }

    //Calculate sound velocity
    if((h >= -2000) && (h <= 80000))
    {
        isa.a = qSqrt(kappa * R * isa.T);
    }
    else
    {
        isa.a = -1;
    }

    return isa;
}

double ATCMath::crossoverAltitude(double CAS, double M)
{
    ISA isa = atmosISA(0);

    double kappa = 1.4;
    double a0 = isa.a;
    double t0 = isa.T;
    double R = 287.05287;
    double betaTrop = -0.0065;
    double g0 = 9.80665;

    double deltaTrans = (qPow((1 + (kappa-1)/2 * qPow(CAS/a0, 2)), kappa/(kappa - 1)) - 1) / (qPow((1 + (kappa-1)/2 * qPow(M, 2)), kappa/(kappa-1)) - 1);
    double thetaTrans = qPow(deltaTrans, -1 * betaTrop * R / g0);
    double HpTrans = 1000 / 6.5 * t0 * (1 - thetaTrans);

    return HpTrans;
}

double ATCMath::normalizeAngle(double angle, ATC::AngularUnits unitType)
{
    double normAngle;

    if(unitType == ATC::Rad)
    {
        int k = qFloor(qFabs(angle) / (2 * ATCConst::PI));

        if(angle > 0)
        {
            normAngle = angle - k * 2 * ATCConst::PI;
        }
        else
        {
            normAngle = angle + (k + 1) * 2 * ATCConst::PI;
        }
    }
    else
    {
        int k = qFloor(qFabs(angle) / 360);

        if(angle > 0)
        {
            normAngle = angle - k * 360;
        }
        else
        {
            normAngle = angle + (k + 1) * 360;
        }
    }

    return normAngle;
}
