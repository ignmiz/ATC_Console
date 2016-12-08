#include "dialogflight.h"
#include "ui_dialogflight.h"

DialogFlight::DialogFlight(ATCAirspace *airspace, QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 300, 160),
    airspace(airspace),
    uiInner(new Ui::DialogFlight)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogFlight::~DialogFlight()
{
    delete uiInner;
}

void DialogFlight::on_buttonCreateFlight_clicked()
{
    emit signalConstructFlightCreator();
}

void DialogFlight::on_buttonReady_clicked()
{
    //TO BE IMPLEMENTED
}

void DialogFlight::on_buttonCancel_clicked()
{
    emit closed();
    close();
}
