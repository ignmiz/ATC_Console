#include "dialogleaders.h"
#include "ui_dialogleaders.h"

DialogLeaders::DialogLeaders(QWidget *parent) :
    ATCDialog(parent, "Leader Lines", 250, 100),
    uiInner(new Ui::DialogLeaders)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogLeaders::~DialogLeaders()
{
    delete uiInner;
}
