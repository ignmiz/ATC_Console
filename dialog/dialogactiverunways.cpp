#include "dialogactiverunways.h"
#include "ui_dialogactiverunways.h"

DialogActiveRunways::DialogActiveRunways(QWidget *parent) :
    ATCDialog(parent, "Active Runways", 600, 650),
    uiInner(new Ui::DialogActiveRunways)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogActiveRunways::~DialogActiveRunways()
{
    delete uiInner;
}

void DialogActiveRunways::on_buttonOK_clicked()
{

}

void DialogActiveRunways::on_buttonCancel_clicked()
{
    emit closed();
    close();
}
