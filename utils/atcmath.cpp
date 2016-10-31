
#include "atcmath.h"

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
    return varM * 2.9342723;
}

double ATCMath::ft2m(double varFT)
{
    return varFT * 0.3408;
}

double ATCMath::mps2kt(double varMPS)
{
    return varMPS * 1.94384449;
}

double ATCMath::kt2mps(double varKT)
{
    return varKT * 0.51444444;
}

double ATCMath::tempISA(double h)
{
    double betaTrop = -0.0065;
    double betaStrat1 = 0.001;
    double betaStrat2 = 0.0028;
    double betaMeso1 = -0.0028;
    double betaMeso2 = -0.002;

    if((h >= -2000) && (h < 11000))
    {
        return 288.15 + betaTrop * (h - 0);
    }
    else if((h >= 11000) && (h < 20000))
    {
        return 216.65;
    }
    else if((h >= 20000) && (h < 32000))
    {
        return 216.65 + betaStrat1 * (h - 20000);
    }
    else if((h >= 32000) && (h < 47000))
    {
        return 228.65 + betaStrat2 * (h - 32000);
    }
    else if((h >= 47000) && (h < 51000))
    {
        return 270.65;
    }
    else if((h >= 51000) && (h < 71000))
    {
        return 270.65 + betaMeso1 * (h - 51000);
    }
    else if((h >= 71000) && (h <= 80000))
    {
        return 214.65 + betaMeso2 * (h - 71000);
    }
    else
    {
        return -1;
    }
}
