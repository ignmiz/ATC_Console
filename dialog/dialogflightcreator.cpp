#include "dialogflightcreator.h"
#include "ui_dialogflightcreator.h"

DialogFlightCreator::DialogFlightCreator(ATCFlightFactory *flightFactory, QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 600, 700),
    uiInner(new Ui::DialogFlightCreator),
    flightFactory(flightFactory)
{
    uiInner->setupUi(this);
    windowSetup();

    for(int i = 0; i < flightFactory->getAircraftTypeFactory().getTypeVectorSize(); i++)
    {
        QString type = flightFactory->getAircraftTypeFactory().getType(i)->getAcType().ICAOcode;
        uiInner->comboBoxAcftType->addItem(type, Qt::DisplayRole);
    }
}

DialogFlightCreator::~DialogFlightCreator()
{
    delete uiInner;
}

void DialogFlightCreator::on_buttonOK_clicked()
{
    //TO BE IMPLEMENTED
}

void DialogFlightCreator::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogFlightCreator::on_buttonSetRoute_clicked()
{
    setRoute();
}

void DialogFlightCreator::on_buttonGetLocation_clicked()
{
    emit signalGetLocation();
    hide();
}

void DialogFlightCreator::on_buttonFillAll_clicked()
{
    setCallsign();
    setTAS();
    setRoute();
    setSquawk();
}

void DialogFlightCreator::setRoute()
{
    QString departure = uiInner->lineEditDeparture->text();
    QString destination = uiInner->lineEditDestination->text();

    ATCRoute *route = flightFactory->getRouteFactory().getRoute(departure, destination);

    if(route != nullptr)
    {
        QString strRoute = route->getRoute().at(0);

        for(int i = 1; i < route->getRoute().size(); i++)
        {
            strRoute = strRoute + " " + route->getRoute().at(i);
        }

        uiInner->textEditRoute->setText(strRoute);
    }
    else
    {
        uiInner->textEditRoute->setText("ROUTE NOT FOUND!");
    }
}

void DialogFlightCreator::setCallsign()
{
    QString company = flightFactory->getCompanyFactory().getCompany()->getCode();
    QString flightNumber = ATCFlightNumberFactory::getFlightNumber();
    QString callsign = company + flightNumber;

    uiInner->lineEditCallsign->setText(callsign);
}

void DialogFlightCreator::setSquawk()
{
    QString squawk  = ATCFlightFactory::generateSquawk();
    if((squawk != "7500") || (squawk != "7600") || (squawk != "7700"))
    {
        uiInner->lineEditSquawk->setText(squawk);
        uiInner->lineEditSquawkCurrent->setText(squawk);
    }
}

void DialogFlightCreator::setTAS()
{
    QString altitude = uiInner->lineEditAltitude->text();

    double alt;

    if(altitude.left(2) == "FL")
    {
        alt = altitude.right(altitude.size() - 2).toDouble() * 100;
    }
    else
    {
        alt = altitude.toDouble();
    }

    alt = ATCMath::ft2m(alt);

    ISA isa = ATCMath::atmosISA(alt);

    QString typeStr = uiInner->comboBoxAcftType->currentText();
    ATCAircraftType *type = flightFactory->getAircraftTypeFactory().getType(typeStr);

    double mach = type->getVelocity().M_CR_AV;
    double cas = type->getVelocity().V_CR2_AV;

    double crossoverAlt = ATCMath::crossoverAltitude(cas, mach);
    crossoverAlt = ATCMath::m2ft(crossoverAlt);

    int tas;

    if(alt >= crossoverAlt)
    {
        tas = qFabs(ATCMath::mps2kt(ATCMath::mach2tas(mach, isa.a)));
    }
    else
    {
        tas = qFabs(ATCMath::mps2kt(ATCMath::cas2tas(ATCMath::kt2mps(cas), isa.p, isa.rho)));
    }

    uiInner->lineEditTAS->setText(QString::number(tas));
}

void DialogFlightCreator::on_buttonSetCallsign_clicked()
{
    setCallsign();
}

void DialogFlightCreator::on_buttonSetSquawk_clicked()
{
    setSquawk();
}

void DialogFlightCreator::on_buttonSetTAS_clicked()
{
    setTAS();
}

void DialogFlightCreator::slotShowFlightCreator()
{
    show();
}

void DialogFlightCreator::slotDisplayClicked(double x, double y)
{
    //INVERSE FROM MERCATOR PROJECTION CALCULATION HERE

    uiInner->lineEditLongitude->setText(QString::number(x));
    uiInner->lineEditLatitude->setText(QString::number(y));
}
