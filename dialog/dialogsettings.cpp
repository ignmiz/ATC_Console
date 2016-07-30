#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    ATCDialog(parent, "Settings", 500, 700),
    uiInner(new Ui::DialogSettings)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogSettings::~DialogSettings()
{
    delete uiInner;
}
