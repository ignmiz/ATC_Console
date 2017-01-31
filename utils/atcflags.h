#ifndef ATCFLAGS_H
#define ATCFLAGS_H

namespace ATC {

    enum ATCDialogType
    {
        TextConsole, Default
    };

    enum AircraftType
    {
        Civil, Military
    };

    enum EngineType
    {
        Jet, Turboprop, Piston
    };

    enum WakeCategory
    {
        J, H, M, L
    };

    enum FlightRules
    {
        IFR, VFR, SVFR
    };

    enum NavMode
    {
        Nav, Hdg
    };

    enum TagType
    {
        Short, Full
    };

    enum AngularUnits
    {
        Deg, Rad
    };

    enum SimCreationMode
    {
        New, Edit
    };
}

namespace BADA
{
    enum AccelerationMode
    {
        Accelerate, Decelerate, Constant
    };

    enum ClimbMode
    {
        Climb, Descend, Level
    };

    enum SpeedHoldMode
    {
        CAS, Mach
    };

    enum FlightPhase
    {
        UpperDescent, LowerDescent, Approach, Landing
    };

    enum ReducedPowerMode
    {
        On, Off
    };

    enum TroposphereMode
    {
        Low, High
    };
}


#endif // ATCFLAGS_H
