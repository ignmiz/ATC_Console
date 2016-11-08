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

void DialogFlightCreator::on_buttonGetRoute_clicked()
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

void DialogFlightCreator::on_buttonGetLocation_clicked()
{
    //TO BE IMPLEMENTED
}

void DialogFlightCreator::on_buttonAutoFill_clicked()
{
    //TO BE IMPLEMENTED
}
