#include "dialogsectorsetup.h"
#include "ui_dialogsectorsetup.h"
#include "mainwindow.h"

DialogSectorSetup::DialogSectorSetup(QWidget *parent) :
    ATCDialog(parent, "Sector Setup", 600, 800),
    uiInner(new Ui::DialogSectorSetup)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogSectorSetup::~DialogSectorSetup()
{
    static_cast<MainWindow*>(getParentWindowAdress())->setFlagDialogSectorSetupExists(false);
    delete uiInner;
}
