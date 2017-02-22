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

    enum LeaderUnits
    {
        LeaderNM, LeaderMIN
    };
}

namespace BADA
{
    enum AccelerationMode
    {
        Accelerate, Decelerate, Constant, ErrorAM
    };

    enum ClimbMode
    {
        Climb, Descend, Level, ErrorCM
    };

    enum SpeedHoldMode
    {
        CAS, Mach, ErrorSHM
    };

    enum FlightPhase
    {
        UpperDescent, LowerDescent, Approach, Landing, ErrorFP
    };

    enum ReducedPowerMode
    {
        On, Off, ErrorRPM
    };

    enum TroposphereMode
    {
        Low, High, ErrorTRM
    };
}


#endif // ATCFLAGS_H
