#include "dialogflightlist.h"
#include "ui_dialogflightlist.h"

DialogFlightList::DialogFlightList(QWidget *parent) :
    ATCDialog(parent, "Flight List", 500, 300),
    uiInner(new Ui::DialogFlightList)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogFlightList::~DialogFlightList()
{
    delete uiInner;
}
