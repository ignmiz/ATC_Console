#include "sectorsetup.h"
#include "ui_sectorsetup.h"

SectorSetup::SectorSetup(QWidget *parent) :
    ATCDialog(parent, "Sector Setup", 600, 800),
    ui(new Ui::SectorSetup)
{
    windowSetup();
}

SectorSetup::~SectorSetup()
{
    delete ui;
}
