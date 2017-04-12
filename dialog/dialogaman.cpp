#include "dialogaman.h"
#include "ui_dialogaman.h"

DialogAman::DialogAman(QWidget *parent) :
    ATCDialog(parent, "Approach Manager", 700, 900),
    uiInner(new Ui::DialogAman)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogAman::~DialogAman()
{
    delete uiInner;
}
