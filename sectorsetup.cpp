#include "sectorsetup.h"
#include "ui_sectorsetup.h"

SectorSetup::SectorSetup(QWidget *parent) :
    ATCDialog(parent, "Sector Setup", 600, 800),
    uiInner(new Ui::SectorSetup)
{
    uiInner->setupUi(this);
    windowSetup();
}

SectorSetup::~SectorSetup()
{
    delete uiInner;
}
