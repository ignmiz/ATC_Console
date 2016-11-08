#include "dialogflight.h"
#include "ui_dialogflight.h"

DialogFlight::DialogFlight(QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 300, 240),
    uiInner(new Ui::DialogFlight)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogFlight::~DialogFlight()
{
    delete uiInner;
}

void DialogFlight::on_buttonCreateEmpty_clicked()
{
    emit signalConstructEmptyFlightCreator();
//    hide();
}

void DialogFlight::on_buttonCreateFlight_clicked()
{
    //TO BE IMPLEMENTED
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
