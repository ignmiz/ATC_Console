#ifndef ATCMATH_H
#define ATCMATH_H

class ATCMath
{
public:
    explicit ATCMath();
    ~ATCMath();

    static double m2nm(double varM);
    static double nm2m(double varNM);

    static double m2ft(double varM);
    static double ft2m(double varFT);

    static double mps2kt(double varMPS);
    static double kt2mps(double varKT);

    static double cas2tas();
    static double cas2mach();

    static double tas2cas();
    static double tas2mach();

    static double aISA(double h);
    static double rhoISA(double h);
    static double pISA(double h);
    static double tempISA(double h);

private:

};

#endif // ATCMATH_H
