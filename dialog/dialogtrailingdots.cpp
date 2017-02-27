#include "dialogtrailingdots.h"
#include "ui_dialogtrailingdots.h"

DialogTrailingDots::DialogTrailingDots(ATCSettings *settings, QWidget *parent) :
    ATCDialog(parent, "Trailing Dots", 230, 100),
    uiInner(new Ui::DialogTrailingDots),
    settings(settings)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->spinBoxCount->setValue(settings->TRAILING_COUNT);
}

DialogTrailingDots::~DialogTrailingDots()
{
    delete uiInner;
}

void DialogTrailingDots::on_buttonApply_clicked()
{
    settings->TRAILING_COUNT = uiInner->spinBoxCount->value();

    emit signalUpdateTrailingDots();
    emit closed();
    close();
}

void DialogTrailingDots::on_buttonCancel_clicked()
{
    emit closed();
    close();
}
