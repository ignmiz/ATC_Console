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

}

void DialogFlight::on_buttonCreateFlight_clicked()
{

}

void DialogFlight::on_buttonReady_clicked()
{

}

void DialogFlight::on_buttonCancel_clicked()
{

}
