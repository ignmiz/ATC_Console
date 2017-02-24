
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

double ATCMath::min(double a, double b)
{
    return (a < b) ? a : b;
}

double ATCMath::max(double a, double b)
{
    return (a > b) ? a : b;
}

ISA ATCMath::atmosISA(double h)
{
    ISA isa;

    double betaTrop = ATCConst::betaTrop;
    double betaStrat1 = ATCConst::betaStrat1;
    double betaStrat2 = ATCConst::betaStrat2;
    double betaMeso1 = ATCConst::betaMeso1;
    double betaMeso2 = ATCConst::betaMeso2;

    double kappa = ATCConst::kappa;
    double R = ATCConst::R;

    double gN = ATCConst::g0;

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

double ATCMath::crossoverAltitude(double casMPS, double M)
{
    ISA isa = atmosISA(0);

    double kappa = 1.4;
    double a0 = isa.a;
    double t0 = isa.T;
    double R = 287.05287;
    double betaTrop = -0.0065;
    double g0 = 9.80665;

    double deltaTrans = (qPow((1 + (kappa-1)/2 * qPow(casMPS/a0, 2)), kappa/(kappa - 1)) - 1) / (qPow((1 + (kappa-1)/2 * qPow(M, 2)), kappa/(kappa-1)) - 1);
    double thetaTrans = qPow(deltaTrans, -1 * betaTrop * R / g0);
    double HpTrans = 1000 / 6.5 * t0 * (1 - thetaTrans);

    return HpTrans;
}

double ATCMath::normalizeAngle(double angle, ATC::AngularUnits unitType)
{
    double normalized;

    if(unitType == ATC::Rad)
    {
        int k = qFloor(qFabs(angle) / (2 * ATCConst::PI));

        if(angle > 0)
        {
            normalized = angle - k * 2 * ATCConst::PI;
        }
        else
        {
            normalized = angle + (k + 1) * 2 * ATCConst::PI;
        }
    }
    else
    {
        int k = qFloor(qFabs(angle) / 360);

        if(angle > 0)
        {
            normalized = angle - k * 360;
        }
        else
        {
            normalized = angle + (k + 1) * 360;
        }
    }

    return normalized;
}

double ATCMath::ESF(BADA::ClimbMode cm, BADA::AccelerationMode am, BADA::SpeedHoldMode shm, BADA::TroposphereMode trm, double mach, double temp, double dTemp)
{
    double betaTrop = ATCConst::betaTrop;
    double kappa = ATCConst::kappa;
    double R = ATCConst::R;
    double g0 = ATCConst::g0;

    double ESF;

    if(cm == BADA::Level)
    {
        ESF = 0;
    }
    else if(cm == BADA::Descend)
    {
        if(am == BADA::Constant)
        {
            if(shm == BADA::CAS)
            {
                if(trm == BADA::Low)
                {
                    ESF = qPow(1 + (kappa*R*betaTrop)/(2*g0) * qPow(mach, 2) * (temp - dTemp)/temp + qPow(1 + (kappa-1)/2 * qPow(mach, 2), -1/(kappa-1)) * (qPow(1 + (kappa-1)/2 * qPow(mach, 2), kappa/(kappa-1)) -1), -1);
                }
                else //High
                {
                    ESF = qPow(1 + qPow(1 + (kappa-1)/2 * qPow(mach, 2), -1/(kappa-1)) * (qPow(1 + (kappa-1)/2 * qPow(mach, 2), kappa/(kappa-1)) - 1), -1);
                }
            }
            else
            {
                if(trm == BADA::Low)
                {
                    ESF = qPow(1 + (kappa*R*betaTrop)/(2*g0) * qPow(mach, 2) * (temp - dTemp)/temp, -1);
                }
                else //High
                {
                    ESF = 1;
                }
            }
        }
        else if(am == BADA::Decelerate)
        {
            ESF = 0.3;
        }
        else //Accelerate
        {
            ESF = 1.7;
        }
    }
    else //Climb
    {
        if(am == BADA::Constant)
        {
            if(shm == BADA::CAS)
            {
                if(trm == BADA::Low)
                {
                    ESF = qPow(1 + (kappa*R*betaTrop)/(2*g0) * qPow(mach, 2) * (temp - dTemp)/temp + qPow(1 + (kappa-1)/2 * qPow(mach, 2), -1/(kappa-1)) * (qPow(1 + (kappa-1)/2 * qPow(mach, 2), kappa/(kappa-1)) -1), -1);
                }
                else //High
                {
                    ESF = qPow(1 + qPow(1 + (kappa-1)/2 * qPow(mach, 2), -1/(kappa-1)) * (qPow(1 + (kappa-1)/2 * qPow(mach, 2), kappa/(kappa-1)) - 1), -1);
                }
            }
            else
            {
                if(trm == BADA::Low)
                {
                    ESF = qPow(1 + (kappa*R*betaTrop)/(2*g0) * qPow(mach, 2) * (temp - dTemp)/temp, -1);
                }
                else //High
                {
                    ESF = 1;
                }
            }
        }
        else if(am == BADA::Decelerate)
        {
            ESF = 1.7;
        }
        else //Accelerate
        {
            ESF = 0.3;
        }
    }

    return ESF;
}

double ATCMath::pathAngle(double thrust, double drag, double ESF, double m)
{
    return qAsin((thrust - drag) * ESF / (m * ATCConst::g0));
}

double ATCMath::bankAngle(double k1, double k2, double errorXTrack, double errorHdg, double bankLimitRad)
{
    double phi = k1 * errorXTrack + k2 * errorHdg;

    if(phi < - bankLimitRad)
    {
        phi = -bankLimitRad;
    }
    else if(phi > bankLimitRad)
    {
        phi = bankLimitRad;
    }

    return phi;
}

double ATCMath::DTA(double vMPS, double bankLimitRad, double dHdgRad, double flyOverDstM)
{
    double g = ATCConst::g0;

    double r = qPow(vMPS, 2) / (g * qTan(bankLimitRad));
    double DTA = r * qTan(qFabs(dHdgRad) / 2);

    //DTA limit due early turn at large heading changes
    if(DTA > 2 * r) DTA = flyOverDstM;

    return DTA;
}

void ATCMath::projectAcftPosOnPath(GeographicLib::Geodesic &geo, double fix1Lat, double fix1Lon, double fix2Lat, double fix2Lon, double acftLat, double acftLon, double acftHdg, double &xtrackError, double &headingError, double &dstToNext)
{
    //INPUT: Fix coordinates in degrees, aircraft coords & heading in radians, all distances in metres
    //OUTPUT: xtrackError, dstToNext: metres, headingError: radians

    using namespace GeographicLib;
    double R = ATCConst::WGS84_AVG_RADIUS;

    double dst1to2;         //Distance from Fix1 to Fix2
    double azimuth1to2;     //True heading from Fix1 to Fix2 @ Fix1
    double azimuth2to1;     //True heading from Fix2 to Fix1 + 180deg (inverse), not used in calculations, just for function compliance
    geo.Inverse(fix1Lat, fix1Lon, fix2Lat, fix2Lon, dst1to2, azimuth1to2, azimuth2to1);

    double dst1toAcft;      //Distance from Fix1 to Aircraft
    double azimuth1toAcft;  //True heading from Fix1 to Aircraft @ Fix1
    double azimuth2toAcft;  //True heading from Aircraft to Fix1 + 180deg (inverse), not used in calculations, just for function compliance
    geo.Inverse(fix1Lat, fix1Lon, ATCMath::rad2deg(acftLat), ATCMath::rad2deg(acftLon), dst1toAcft, azimuth1toAcft, azimuth2toAcft);

    double azimuthDiff = azimuth1to2 - azimuth1toAcft;

    xtrackError = R * qAsin(qSin(dst1toAcft / R) * qSin(ATCMath::deg2rad(azimuthDiff)));
    double dst1toProjectionBase = R * qAcos(qCos(dst1toAcft / R) / qCos(xtrackError / R));

    dstToNext = dst1to2 - dst1toProjectionBase;

    double projectionBaseLat;
    double projectionBaseLon;
    double azimuthProjectionBaseTo1;    //Forward azimuth
    geo.Direct(fix1Lat, fix1Lon, azimuth1to2, dst1toProjectionBase, projectionBaseLat, projectionBaseLon, azimuthProjectionBaseTo1);

    headingError = normalizeHdgChange(acftHdg - ATCMath::deg2rad(azimuthProjectionBaseTo1));
}

void ATCMath::sphericalRhumbIntersection(GeographicLib::Geodesic &geo, double rwyThrLat, double rwyThrLon, double rwyAzimuth, double acftLat, double acftLon, double acftHdg, double &dstAcftToIntersect, double &dstThrToIntersect)
{
    //INPUT: rwy coords in degrees, rwy azimuth in radians, aircraft coords & hdg in radians
    //OUTPUT: dstToIntersect, dstThrToIntersect: distances in metres

    using namespace GeographicLib;

    double dstThrToAcft;
    double azimuthThrToAcft;
    double azimuthAcftToThr; //Forward azimuth
    geo.Inverse(rwyThrLat, rwyThrLon, rad2deg(acftLat), rad2deg(acftLon), dstThrToAcft, azimuthThrToAcft, azimuthAcftToThr);

    double azimuthDiffThr = qFabs(normalizeHdgChange(deg2rad(azimuthThrToAcft) - normalizeAngle(rwyAzimuth + ATCConst::PI, ATC::Rad)));
    double azimuthDiffAcft = qFabs(normalizeHdgChange(acftHdg - normalizeAngle(deg2rad(azimuthAcftToThr) + ATCConst::PI, ATC::Rad)));   //+180 due forward azimuth
    double azimuthDiffIntersection = qFabs(normalizeHdgChange(acftHdg - rwyAzimuth));

    dstAcftToIntersect = dstThrToAcft / qSin(azimuthDiffIntersection) * qSin(azimuthDiffThr);
    dstThrToIntersect = dstThrToAcft / qSin(azimuthDiffIntersection) * qSin(azimuthDiffAcft);
}

double ATCMath::normalizeHdgChange(double dHdg)
{
    if(dHdg > ATCConst::PI)
    {
        dHdg = dHdg - 2 * ATCConst::PI;
    }
    else if(dHdg < -ATCConst::PI)
    {
        dHdg = dHdg + 2 * ATCConst::PI;
    }

    return dHdg;
}

double ATCMath::randomMass(int mMin, int mMax)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    return mMin + qrand() % (mMax - mMin + 1);
}

double ATCMath::recalculateSpeed(double vRef, double m, double mRef)
{
    return vRef * sqrt(m / mRef);
}

double ATCMath::recalculateReductionFactor(double Cred, double m, double mMin, double mMax)
{
    return 1 - Cred * (mMax - m) / (mMax - mMin);
}

double ATCMath::nominalSpeedCL(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vStallTO, double vCL1, double vCL2, double machCL, double a, double rho, double p)
{
    double Vnom;
    double CVmin = ATCConst::C_V_MIN;

    if(engine == ATC::Jet)
    {
        if(shm == BADA::Mach)
        {
            Vnom = machCL;
        }
        else if((currentAltM >= 3048) && (shm == BADA::CAS)) //Above 10000ft
        {
            Vnom = vCL2;
        }
        else if((currentAltM >= 1828.8) && (currentAltM < 3048)) //6000ft - 10000ft
        {
            Vnom = ATCMath::min(vCL1, 250);
        }
        else if((currentAltM >= 1524) && (currentAltM < 1828.8)) //5000ft - 6000ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL5;
        }
        else if((currentAltM >= 1219.2) && (currentAltM < 1524)) //4000ft - 5000ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL4;
        }
        else if((currentAltM >= 914.4) && (currentAltM < 1219.2)) //3000ft - 4000ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL3;
        }
        else if((currentAltM >= 457.2) && (currentAltM < 914.4)) //1500ft - 3000ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL2;
        }
        else if(currentAltM < 457.2) //Below 1500ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL1;
        }
    }
    else
    {
        if(shm == BADA::Mach)
        {
            Vnom = machCL;
        }
        else if((currentAltM >= 3048) && (shm == BADA::CAS)) //Above 10000ft
        {
            Vnom = vCL2;
        }
        else if((currentAltM >= 457.2) && (currentAltM < 3048)) //1500ft - 10000ft
        {
            Vnom = ATCMath::min(vCL1, 250);
        }
        else if((currentAltM >= 304.8) && (currentAltM < 457.2)) //1000ft - 1500ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL8;
        }
        else if((currentAltM >= 152.4) && (currentAltM < 304.8)) //500ft - 1000ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL7;
        }
        else if(currentAltM < 152.4) //Below 500ft
        {
            Vnom = CVmin * vStallTO + ATCConst::V_CL6;
        }
    }

    return ATCMath::nominalSpeed2tas(Vnom, shm, a, rho, p);
}

double ATCMath::nominalSpeedCR(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vCR1, double vCR2, double machCR, double a, double rho, double p)
{
    double Vnom;

    if(engine == ATC::Jet)
    {
        if(shm == BADA::Mach)
        {
            Vnom = machCR;
        }
        else if((currentAltM >= 4267.2) && (shm == BADA::CAS)) //Above 14000ft
        {
            Vnom = vCR2;
        }
        else if((currentAltM >= 1828.8) && (currentAltM < 4267.2)) //6000ft - 14000ft
        {
            Vnom = ATCMath::min(vCR1, 250);
        }
        else if((currentAltM >= 914.4) && (currentAltM < 1828.8)) //3000ft - 6000ft
        {
            Vnom = ATCMath::min(vCR1, 220);
        }
        else if(currentAltM < 914.4) //Below 3000ft
        {
            Vnom = ATCMath::min(vCR1, 170);
        }
    }
    else
    {
        if(shm == BADA::Mach)
        {
            Vnom = machCR;
        }
        else if((currentAltM >= 3048) && (shm == BADA::CAS)) //Above 10000ft
        {
            Vnom = vCR2;
        }
        else if((currentAltM >= 1828.8) && (currentAltM < 3048)) //6000ft - 10000ft
        {
            Vnom = ATCMath::min(vCR1, 250);
        }
        else if((currentAltM >= 914.4) && (currentAltM < 1828.8)) //3000ft - 6000ft
        {
            Vnom = ATCMath::min(vCR1, 180);
        }
        else if(currentAltM < 914.4) //Below 3000ft
        {
            Vnom = ATCMath::min(vCR1, 150);
        }
    }

    return ATCMath::nominalSpeed2tas(Vnom, shm, a, rho, p);
}

double ATCMath::nominalSpeedDS(double currentAltM, BADA::SpeedHoldMode shm, ATC::EngineType engine, double vStallLD, double vDS1, double vDS2, double machDS, double a, double rho, double p)
{
    double Vnom;
    double CVmin = ATCConst::C_V_MIN;

    if((engine == ATC::Jet) || (engine == ATC::Turboprop))
    {
        if(shm == BADA::Mach)
        {
            Vnom = machDS;
        }
        else if((currentAltM >= 3048) && (shm == BADA::CAS)) //Above 10000ft
        {
            Vnom = vDS2;
        }
        else if((currentAltM >= 1828.8) && (currentAltM < 3048)) //6000ft - 10000ft
        {
            Vnom = ATCMath::min(vDS1, 250);
        }
        else if((currentAltM >= 914.4) && (currentAltM < 1828.8)) //3000ft - 6000ft
        {
            Vnom = ATCMath::min(vDS1, 220);
        }
        else if((currentAltM >= 609.6) && (currentAltM < 914.4)) //2000ft - 3000ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES4;
        }
        else if((currentAltM >= 457.2) && (currentAltM < 609.6)) //1500ft - 2000ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES3;
        }
        else if((currentAltM >= 304.8) && (currentAltM < 457.2)) //1000ft - 1500ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES2;
        }
        else if(currentAltM < 304.8) //Below 1000ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES1;
        }
    }
    else
    {
        if(shm == BADA::Mach)
        {
            Vnom = machDS;
        }
        else if((currentAltM >= 3048) && (shm == BADA::CAS)) //Above 10000ft
        {
            Vnom = vDS2;
        }
        else if((currentAltM >= 457.2) && (currentAltM < 3048)) //1500ft - 10000ft
        {
            Vnom = vDS1;
        }
        else if((currentAltM >= 304.8) && (currentAltM < 457.2)) //1000ft - 1500ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES7;
        }
        else if((currentAltM >= 152.4) && (currentAltM < 304.8)) //500ft - 1000ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES6;
        }
        else if(currentAltM < 152.4) //Below 500ft
        {
            Vnom = CVmin * vStallLD + ATCConst::V_DES5;
        }
    }

    return ATCMath::nominalSpeed2tas(Vnom, shm, a, rho, p);
}

double ATCMath::nominalSpeed2tas(double vNom, BADA::SpeedHoldMode shm, double a, double rho, double p)
{
    double tasNomMPS;

    if(shm == BADA::CAS)
    {
        tasNomMPS = ATCMath::cas2tas(ATCMath::kt2mps(vNom), p, rho);
    }
    else
    {
        tasNomMPS = a * vNom;
    }

    return tasNomMPS;
}

double ATCMath::liftCoefficient(double m, double rho, double tasMPS, double S, double phiRad)
{
    return 2 * m * ATCConst::g0 / (rho * qPow(tasMPS, 2) * S * qCos(phiRad));
}

double ATCMath::dragCoefficient(double CL, double CD0, double CD2, double dCD0)
{
    return CD0 + dCD0 + CD2 * qPow(CL, 2);
}

double ATCMath::lift(double rho, double tasMPS, double S, double CL)
{
    return 0.5 * rho * qPow(tasMPS, 2) * S * CL;
}

double ATCMath::drag(double rho, double tasMPS, double S, double CD)
{
    return 0.5 * rho * qPow(tasMPS, 2) * S * CD;
}

double ATCMath::thrust(double tasMPS, double currentAltM, double drag, BADA::ClimbMode cm, BADA::AccelerationMode am, BADA::ReducedPowerMode rpm, ATC::EngineType engine, double CTc1, double CTc2, double CTc3, double CTdes, double Cpowred)
{
    double TMaxClimb;
    double Thrust;

    //Calculate TMaxClimb
    if(engine == ATC::Jet)
    {
        TMaxClimb = CTc1 * (1 - currentAltM / CTc2 + CTc3 * qPow(currentAltM, 2));
    }
    else if(engine == ATC::Turboprop)
    {
        TMaxClimb = (CTc1 / tasMPS) * (1 - currentAltM / CTc2) + CTc3;
    }
    else //Piston
    {
        TMaxClimb = CTc1 * (1 - currentAltM / CTc2) + CTc3;
    }

    //Calculate Thrust
    if((cm == BADA::Climb) && (rpm == BADA::Off))
    {
        Thrust = TMaxClimb;
    }
    else if((cm == BADA::Climb) && (rpm == BADA::On))
    {
        Thrust = TMaxClimb * Cpowred - drag * (1 - Cpowred);
    }
    else if((cm == BADA::Descend) || ((cm == BADA::Level) && (am == BADA::Decelerate)))
    {
        Thrust = CTdes * TMaxClimb;
    }
    else if((cm == BADA::Level) && (am == BADA::Accelerate))
    {
        Thrust = 0.95 * TMaxClimb;
    }
    else if((cm == BADA::Level) && (am == BADA::Constant))
    {
        Thrust = drag;
    }

    return Thrust;
}

double ATCMath::ellipsoidRadius(double currentAltM, double latRad)
{
    double e = ATCConst::WGS84_FIRST_ECCENTRICITY;

    return ATCConst::WGS84_A * qSqrt(1 - qPow(e, 2)) / qSqrt(qPow(qSin(latRad), 2) + (1 - qPow(e, 2)) * qPow(qCos(latRad), 2)) + currentAltM;
}

double ATCMath::stateLonDot(double vMPS, double trueHdgRad, double gammaRad, double latRad, double radiusM)
{
    return vMPS * qSin(trueHdgRad) * qCos(gammaRad) / (radiusM * qCos(latRad));
}

double ATCMath::stateLatDot(double vMPS, double trueHdgRad, double gammaRad, double radiusM)
{
    return vMPS * qCos(trueHdgRad) * qCos(gammaRad) / radiusM;
}

double ATCMath::stateHDot(double vMPS, double gammaRad)
{
    return vMPS * qSin(gammaRad);
}

double ATCMath::stateVDot(double thrust, double drag, double m, double ESF)
{
    return (thrust - drag) / m * (1 - ESF);
}

double ATCMath::stateHdgDot(double lift, double m, double vMPS, double bankRad)
{
    return lift / (m * vMPS) * qSin(bankRad);
}

BADA::ClimbMode ATCMath::assignCM(double currentAltM, double targetAltM)
{
    double diff = currentAltM - targetAltM;

    if(qFabs(diff) <= 1)
    {
        return BADA::Level;
    }
    else if(diff > 0)
    {
        return BADA::Descend;
    }
    else if(diff < 0)
    {
        return BADA::Climb;
    }

    return BADA::ErrorCM;
}

BADA::FlightPhase ATCMath::assignFP(double currentAltM, double altDesM, double tasMPS, double Vdes1KT, double p, double rho)
{
    double casKT = ATCMath::mps2kt(ATCMath::tas2cas(tasMPS, p, rho));

    double v1KT;
    double v2KT;

    (Vdes1KT > 220) ? (v1KT = 230) : (v1KT = Vdes1KT + 10);
    (Vdes1KT > 250) ? (v2KT = 260) : (v2KT = Vdes1KT + 10);

    if(currentAltM >= altDesM)
    {
        return BADA::UpperDescent;
    }
    else if((currentAltM < altDesM) && ((casKT >= v2KT) || (currentAltM >= 2438.4))) //8000ft
    {
        return BADA::LowerDescent;
    }
    else if(((casKT < v2KT) && (currentAltM < 2438.4)) && ((casKT >= v1KT) || (currentAltM >= 914.4)))
    {
        return BADA::Approach;
    }
    else if((casKT < v1KT) && (currentAltM < 914.4)) //3000ft
    {
        return BADA::Landing;
    }

    return BADA::ErrorFP;
}

BADA::ReducedPowerMode ATCMath::assignRPM(double currentAltM, double altMax)
{
    if(currentAltM >= 0.8 * altMax)
    {
        return BADA::Off;
    }
    else
    {
        return BADA::On;
    }

    return BADA::ErrorRPM;
}

BADA::SpeedHoldMode ATCMath::assignSHM(double currentAltM, BADA::ClimbMode cm, double xoverAltClbM, double xoverAltCrsM, double xoverAltDesM)
{
    if(((cm == BADA::Level) && (currentAltM >= xoverAltCrsM)) ||
       ((cm == BADA::Descend) && (currentAltM >= xoverAltDesM)) ||
       ((cm == BADA::Climb) && (currentAltM >= xoverAltClbM)))
    {
        return BADA::Mach;
    }
    else
    {
        return BADA::CAS;
    }

    return BADA::ErrorSHM;
}

BADA::TroposphereMode ATCMath::assignTRM(double currentAltM)
{
    if(currentAltM <= 11000)
    {
        return BADA::Low;
    }
    else
    {
        return BADA::High;
    }

    return BADA::ErrorTRM;
}

BADA::AccelerationMode ATCMath::assignAM(double tasMPS, double tasNomMPS)
{
    if(qFabs(tasMPS - tasNomMPS) <= 1)
    {
        return BADA::Constant;
    }
    else if(tasMPS - tasNomMPS > 1)
    {
        return BADA::Decelerate;
    }
    else if(tasMPS - tasNomMPS < -1)
    {
        return BADA::Accelerate;
    }

    return BADA::ErrorAM;
}

double ATCMath::mercatorProjectionLon(double longitudeDeg, double referenceLongitudeDeg, double scale)
{
    return scale * (longitudeDeg - referenceLongitudeDeg);
}

double ATCMath::mercatorProjectionLat(double latitudeDeg, double scale)
{
    double latitudeRad = deg2rad(latitudeDeg);
    return rad2deg(scale * qLn(qTan(ATCConst::PI / 4 + latitudeRad / 2) *
           qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeRad)) / (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeRad)),
                ATCConst::WGS84_FIRST_ECCENTRICITY / 2)));
}

double ATCMath::inverseMercatorLon(double mercatorX, double referenceLongitude, double scale)
{
    return mercatorX / scale + referenceLongitude;
}

double ATCMath::inverseMercatorLat(double mercatorY, double error, double scale)
{
    double t = qExp(-1 * deg2rad(mercatorY) / scale);
    double lat = ATCConst::PI / 2 - 2 * qAtan(t);
    double lat1;

    double change = error + 1;

    while(change > error)
    {
        lat1 = ATCConst::PI/2 - 2 * qAtan(t * qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(lat)) / (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(lat)), ATCConst::WGS84_FIRST_ECCENTRICITY / 2));
        change = qFabs(lat1 - lat) / lat;

        lat = lat1;
    }

    return rad2deg(lat);
}

double ATCMath::rotateX(double coordX, double coordY, double angleDeg)
{
    //Assuming Y axis is up, angle measured from X which is horizontal right, ccw positive
    double angleRad = deg2rad(angleDeg);
    return coordX * qCos(angleRad) - coordY * qSin(angleRad);
}

double ATCMath::rotateY(double coordX, double coordY, double angleDeg)
{
    //Assuming Y axis is up, angle measured from X which is horizontal right, ccw positive
    double angleRad = deg2rad(angleDeg);
    return coordX * qSin(angleRad) + coordY * qCos(angleRad);
}

double ATCMath::translateToLocalX(double coordX, double sectorCentreX, double scaleFactor)
{
    return (coordX - sectorCentreX) * scaleFactor;
}

double ATCMath::translateToLocalY(double coordY, double sectorCentreY, double scaleFactor)
{
    return (-1 * (coordY - sectorCentreY) * scaleFactor);
}

double ATCMath::translateFromLocalX(double localX, double sectorCentreX, double scaleFactor)
{
    return localX / scaleFactor + sectorCentreX;
}

double ATCMath::translateFromLocalY(double localY, double sectorCentreY, double scaleFactor)
{
    return -1 * localY / scaleFactor + sectorCentreY;
}

QPointF ATCMath::geo2local(double latRad, double lonRad, double avgDeclinationDeg, double sectorCentreX, double sectorCentreY, double scaleFactor, double scale, double refLon)
{
    double xProjected = mercatorProjectionLon(ATCMath::rad2deg(lonRad), refLon, scale);
    double yProjected = mercatorProjectionLat(ATCMath::rad2deg(latRad), scale);

    double xRotated = rotateX(xProjected, yProjected, avgDeclinationDeg);
    double yRotated = rotateY(xProjected, yProjected, avgDeclinationDeg);

    double xLocal = translateToLocalX(xRotated, sectorCentreX, scaleFactor);
    double yLocal = translateToLocalY(yRotated, sectorCentreY, scaleFactor);

    return QPointF(xLocal, yLocal);
}

QPointF ATCMath::local2geo(double x, double y, double avgDeclinationDeg, double sectorCentreX, double sectorCentreY, double scaleFactor, double scale, double refLon, double error)
{
    double xRotated = translateFromLocalX(x, sectorCentreX, scaleFactor);
    double yRotated = translateFromLocalY(y, sectorCentreY, scaleFactor);

    double xProjected = rotateX(xRotated, yRotated, -1 * avgDeclinationDeg);
    double yProjected = rotateY(xRotated, yRotated, -1 * avgDeclinationDeg);

    double lon = inverseMercatorLon(xProjected, refLon, scale);
    double lat = inverseMercatorLat(yProjected, error, scale);

    return QPointF(lon, lat);
}

QPointF ATCMath::rotatePoint(QPointF pt, double angle, ATC::AngularUnits units)
{
    //Assuming Y axis is up, angle measured from X which is horizontal right, ccw positive
    if(units == ATC::Deg) angle = ATCMath::deg2rad(angle);

    double x = pt.x() * qCos(angle) - pt.y() * qSin(angle);
    double y = pt.x() * qSin(angle) + pt.y() * qCos(angle);

    return(QPointF(x, y));
}

