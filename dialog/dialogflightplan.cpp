#include "dialogflightplan.h"
#include "ui_dialogflightplan.h"

DialogFlightPlan::DialogFlightPlan(ATCFlight *flight, QWidget *parent) :
    ATCDialog(parent, "Flight Plan", 600, 650),
    flight(flight),
    uiInner(new Ui::DialogFlightPlan)
{
    uiInner->setupUi(this);
    windowSetup();


}

DialogFlightPlan::~DialogFlightPlan()
{
    delete uiInner;
}
