#include "dialogflightcreator.h"
#include "ui_dialogflightcreator.h"

DialogFlightCreator::DialogFlightCreator(QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 600, 700),
    uiInner(new Ui::DialogFlightCreator)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogFlightCreator::~DialogFlightCreator()
{
    delete uiInner;
}
