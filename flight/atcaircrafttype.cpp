
#include "atcaircrafttype.h"

ATCAircraftType::ATCAircraftType(QString OPFpath, QString APFpath) : OPFpath(OPFpath), APFpath(APFpath)
{
    interpretOPF(OPFpath);
    interpretAPF(APFpath);
}

ATCAircraftType::~ATCAircraftType()
{

}

AcType ATCAircraftType::getAcType()
{
    return actype;
}

Mass ATCAircraftType::getMass()
{
    return mass;
}

Envelope ATCAircraftType::getEnvelope()
{
    return envelope;
}

Buffeting ATCAircraftType::getBuffeting()
{
    return buffeting;
}

Aero ATCAircraftType::getAeroCR()
{
    return aeroCR;
}

Aero ATCAircraftType::getAeroIC()
{
    return aeroIC;
}

Aero ATCAircraftType::getAeroTO()
{
    return aeroTO;
}

Aero ATCAircraftType::getAeroAP()
{
    return aeroAP;
}

Aero ATCAircraftType::getAeroLD()
{
    return aeroLD;
}

Thrust ATCAircraftType::getThrust()
{
    return thrust;
}

Fuel ATCAircraftType::getFuel()
{
    return fuel;
}

Ground ATCAircraftType::getGround()
{
    return ground;
}

Velocity ATCAircraftType::getVelocity()
{
    return velocity;
}

double ATCAircraftType::getSurface()
{
    return S;
}

double ATCAircraftType::getCDldg()
{
    return dCD_ldg;
}

void ATCAircraftType::interpretOPF(QString OPFpath)
{
    QFile file(OPFpath);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ATCAircraftType: Failed to open path: " + OPFpath);
        msgBox.exec();
    }

    QTextStream stream(&file);
    int line = 0;

    while(!stream.atEnd())
    {
        QString textLine = stream.readLine().trimmed();

        if(textLine.left(2) == "CD")
        {
            line++;

            QStringList stringList = textLine.split(" ", QString::SkipEmptyParts);

            if(line == 1)
            {
                actype.ICAOcode = stringList.at(1).trimmed().left(4);
                actype.engineCount = stringList.at(2).trimmed().toInt();

                if(stringList.at(4).trimmed() == "Jet")
                {
                    actype.engineType = ATC::Jet;
                }
                else if(stringList.at(4).trimmed() == "Piston")
                {
                    actype.engineType = ATC::Piston;
                }
                else if(stringList.at(4).trimmed() == "Turboprop")
                {
                    actype.engineType = ATC::Turboprop;
                }

                if(stringList.at(5).trimmed() == "M")
                {
                    actype.wake = ATC::M;
                }
                else if(stringList.at(5).trimmed() == "H")
                {
                    actype.wake = ATC::H;
                }
                else if(stringList.at(5).trimmed() == "L")
                {
                    actype.wake = ATC::L;
                }
                else if(stringList.at(5).trimmed() == "J")
                {
                    actype.wake = ATC::J;
                }
            }
            else if(line == 2)
            {
                mass.ref = stringList.at(1).trimmed().toDouble();
                mass.min = stringList.at(2).trimmed().toDouble();
                mass.max = stringList.at(3).trimmed().toDouble();
                mass.payload = stringList.at(4).trimmed().toDouble();
                mass.gradient = stringList.at(5).trimmed().toDouble();
            }
            else if(line == 3)
            {
                envelope.VMO = stringList.at(1).trimmed().toDouble();
                envelope.MMO = stringList.at(2).trimmed().toDouble();
                envelope.h_max = stringList.at(3).trimmed().toDouble();
                envelope.h_max_MTOW = stringList.at(4).trimmed().toDouble();
                envelope.tempGradient = stringList.at(5).trimmed().toDouble();
            }
            else if(line == 4)
            {
                S = stringList.at(2).trimmed().toDouble();
                buffeting.Clbo = stringList.at(3).trimmed().toDouble();
                buffeting.K = stringList.at(4).trimmed().toDouble();
                buffeting.CM16 = stringList.at(5).trimmed().toDouble();
            }
            else if(line == 5)
            {
                aeroCR.V_stall = stringList.at(4).trimmed().toDouble();
                aeroCR.CD0 = stringList.at(5).trimmed().toDouble();
                aeroCR.CD2 = stringList.at(6).trimmed().toDouble();
            }
            else if(line == 6)
            {
                aeroIC.V_stall = stringList.at(4).trimmed().toDouble();
                aeroIC.CD0 = stringList.at(5).trimmed().toDouble();
                aeroIC.CD2 = stringList.at(6).trimmed().toDouble();
            }
            else if(line == 7)
            {
                aeroTO.V_stall = stringList.at(4).trimmed().toDouble();
                aeroTO.CD0 = stringList.at(5).trimmed().toDouble();
                aeroTO.CD2 = stringList.at(6).trimmed().toDouble();
            }
            else if(line == 8)
            {
                aeroAP.V_stall = stringList.at(4).trimmed().toDouble();
                aeroAP.CD0 = stringList.at(5).trimmed().toDouble();
                aeroAP.CD2 = stringList.at(6).trimmed().toDouble();
            }
            else if(line == 9)
            {
                aeroLD.V_stall = stringList.at(4).trimmed().toDouble();
                aeroLD.CD0 = stringList.at(5).trimmed().toDouble();
                aeroLD.CD2 = stringList.at(6).trimmed().toDouble();
            }
            else if(line == 13)
            {
                dCD_ldg = stringList.at(3).trimmed().toDouble();
            }
            else if(line == 16)
            {
                thrust.C_Tc1 = stringList.at(1).trimmed().toDouble();
                thrust.C_Tc2 = stringList.at(2).trimmed().toDouble();
                thrust.C_Tc3 = stringList.at(3).trimmed().toDouble();
                thrust.C_Tc4 = stringList.at(4).trimmed().toDouble();
                thrust.C_Tc5 = stringList.at(5).trimmed().toDouble();
            }
            else if(line == 17)
            {
                thrust.C_Tdes_low = stringList.at(1).trimmed().toDouble();
                thrust.C_Tdes_high = stringList.at(2).trimmed().toDouble();
                thrust.h_pdes = stringList.at(3).trimmed().toDouble();
                thrust.C_Tdes_app = stringList.at(4).trimmed().toDouble();
                thrust.C_Tdes_ldg = stringList.at(5).trimmed().toDouble();
            }
            else if(line == 19)
            {
                fuel.C_f1 = stringList.at(1).trimmed().toDouble();
                fuel.C_f2 = stringList.at(2).trimmed().toDouble();
            }
            else if(line == 20)
            {
                fuel.C_f3 = stringList.at(1).trimmed().toDouble();
                fuel.C_f4 = stringList.at(2).trimmed().toDouble();
            }
            else if(line == 21)
            {
                fuel.C_fcr = stringList.at(1).trimmed().toDouble();
            }
            else if(line == 22)
            {
                ground.TOL = stringList.at(1).trimmed().toDouble();
                ground.LDL = stringList.at(2).trimmed().toDouble();
                ground.span = stringList.at(3).trimmed().toDouble();
                ground.length = stringList.at(4).trimmed().toDouble();
            }
        }
    }

    file.close();
}

void ATCAircraftType::interpretAPF(QString APFpath)
{
    QFile file(APFpath);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ATCAircraftType: Failed to open path: " + APFpath);
        msgBox.exec();
    }

    QTextStream stream(&file);

    while(!stream.atEnd())
    {
        QString textLine = stream.readLine().trimmed();

        if(textLine.left(2) == "CD")
        {
            QStringList stringList = textLine.split(" ", QString::SkipEmptyParts);

            if(stringList.at(2).trimmed() == "LO")
            {
                velocity.V_CL1_LO = stringList.at(3).trimmed().toDouble();
                velocity.V_CL2_LO = stringList.at(4).trimmed().toDouble();
                velocity.M_CL_LO = stringList.at(5).trimmed().toDouble() / 100;

                velocity.V_CR1_LO = stringList.at(6).trimmed().toDouble();
                velocity.V_CR2_LO = stringList.at(7).trimmed().toDouble();
                velocity.M_CR_LO = stringList.at(8).trimmed().toDouble() / 100;

                velocity.M_DS_LO = stringList.at(9).trimmed().toDouble() / 100;
                velocity.V_DS2_LO = stringList.at(10).trimmed().toDouble();
                velocity.V_DS1_LO = stringList.at(11).trimmed().toDouble();
            }
            else if(stringList.at(2).trimmed() == "AV")
            {
                velocity.V_CL1_AV = stringList.at(3).trimmed().toDouble();
                velocity.V_CL2_AV = stringList.at(4).trimmed().toDouble();
                velocity.M_CL_AV = stringList.at(5).trimmed().toDouble() / 100;

                velocity.V_CR1_AV = stringList.at(6).trimmed().toDouble();
                velocity.V_CR2_AV = stringList.at(7).trimmed().toDouble();
                velocity.M_CR_AV = stringList.at(8).trimmed().toDouble() / 100;

                velocity.M_DS_AV = stringList.at(9).trimmed().toDouble() / 100;
                velocity.V_DS2_AV = stringList.at(10).trimmed().toDouble();
                velocity.V_DS1_AV = stringList.at(11).trimmed().toDouble();
            }
            else if(stringList.at(2).trimmed() == "HI")
            {
                velocity.V_CL1_HI = stringList.at(3).trimmed().toDouble();
                velocity.V_CL2_HI = stringList.at(4).trimmed().toDouble();
                velocity.M_CL_HI = stringList.at(5).trimmed().toDouble() / 100;

                velocity.V_CR1_HI = stringList.at(6).trimmed().toDouble();
                velocity.V_CR2_HI = stringList.at(7).trimmed().toDouble();
                velocity.M_CR_HI = stringList.at(8).trimmed().toDouble() / 100;

                velocity.M_DS_HI = stringList.at(9).trimmed().toDouble() / 100;
                velocity.V_DS2_HI = stringList.at(10).trimmed().toDouble();
                velocity.V_DS1_HI = stringList.at(11).trimmed().toDouble();
            }
        }
    }

    file.close();
}

