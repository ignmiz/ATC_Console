#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(ATCFlightFactory *flightFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    flightFactory(flightFactory)
{
    ui->setupUi(this);
    dialogTextConsole = new DialogTextConsole(this);

    mainWindowSetup();
    setSituationalDisplayFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete flightFactory;
}

bool MainWindow::isDialogTextConsoleVisible() const
{
    return dialogTextConsole->isVisible();
}

void MainWindow::on_buttonMainMenu_clicked()
{
    if(!getFlagDialogMainMenuExists())
    {
        dialogMainMenu = new DialogMainMenu(this);
        dialogMainMenu->show();

        connect(dialogMainMenu, SIGNAL(closed()), this, SLOT(dialogMainMenuClosed()));
        connect(dialogMainMenu, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogMainMenuExists(true);
        setSituationalDisplayFocus();

        connect(dialogMainMenu, SIGNAL(signalConstructDialogFlight()), this, SLOT(slotConstructDialogFlight()));        
    }
}

void MainWindow::on_buttonSctSetup_clicked()
{
    if(!getFlagDialogSectorSetupExists())
    {
        dialogSectorSetup = new DialogSectorSetup(this);
        dialogSectorSetup->show();

        connect(dialogSectorSetup, SIGNAL(closed()), this, SLOT(dialogSectorSetupClosed()));
        connect(dialogSectorSetup, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogSectorSetupExists(true);
        setSituationalDisplayFocus();
    }
}

void MainWindow::on_buttonSettings_clicked()
{
    if(!getFlagDialogSettingsExists())
    {
        dialogSettings = new DialogSettings(ui->situationalDisplay ,this);
        dialogSettings->show();

        connect(dialogSettings, SIGNAL(closed()), this, SLOT(dialogSettingsClosed()));
        connect(dialogSettings, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogSettingExists(true);
        setSituationalDisplayFocus();
    }
}

void MainWindow::on_buttonShowConsole_clicked()
{
    if(dialogTextConsole->isHidden())
    {
        if(!(dialogTextConsole->isMaximized()))
            dialogTextConsole->maximizeWindow();

        dialogTextConsole->show();
        dialogTextConsole->raise();
        dialogTextConsole->setConsoleDisplayBottom();
        dialogTextConsole->setConsoleInputFocus();
    }
    else
    {
        dialogTextConsole->hide();
    }
}

void MainWindow::dialogMainMenuClosed()
{
    setFlagDialogMainMenuExists(false);
    disconnect(dialogMainMenu, SIGNAL(closed()), this, SLOT(dialogMainMenuClosed()));
    disconnect(dialogMainMenu, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

    disconnect(dialogMainMenu, SIGNAL(signalConstructDialogFlight()), this, SLOT(slotConstructDialogFlight()));
}

void MainWindow::dialogSectorSetupClosed()
{
    setFlagDialogSectorSetupExists(false);
    disconnect(dialogSectorSetup, SIGNAL(closed()), this, SLOT(dialogSectorSetupClosed()));
    disconnect(dialogSectorSetup, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));
}

void MainWindow::dialogSettingsClosed()
{
    setFlagDialogSettingExists(false);
    disconnect(dialogSettings, SIGNAL(closed()), this, SLOT(dialogSectorSetupClosed()));
    disconnect(dialogSettings, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));
}

void MainWindow::changeFocusToDisplay()
{
    setSituationalDisplayFocus();
}

void MainWindow::slotConstructDialogFlight()
{
    dialogMainMenu->hide();

    dialogFlight = new DialogFlight(this);
    dialogFlight->show();

    connect(dialogFlight, SIGNAL(closed()), this, SLOT(slotCloseDialogFlight()));
    connect(dialogFlight, SIGNAL(signalConstructFlightCreator()), this, SLOT(slotConstructFlightCreator()));
}

void MainWindow::slotCloseDialogFlight()
{
    disconnect(dialogFlight, SIGNAL(closed()), this, SLOT(slotCloseDialogFlight()));
    disconnect(dialogFlight, SIGNAL(signalConstructFlightCreator()), this, SLOT(slotConstructFlightCreator()));

    dialogMainMenu->show();
}

void MainWindow::slotConstructFlightCreator()
{
    dialogFlight->hide();

    dialogFlightCreator = new DialogFlightCreator(flightFactory, this);
    dialogFlightCreator->show();

    connect(dialogFlightCreator, SIGNAL(closed()), this, SLOT(slotCloseFlightCreator()));
}

void MainWindow::slotCloseFlightCreator()
{
    disconnect(dialogFlightCreator, SIGNAL(closed()), this, SLOT(slotCloseFlightCreator()));

    dialogFlight->show();
}

void MainWindow::on_buttonClose_clicked()
{
    close();
}

void MainWindow::mainWindowSetup()
{
    QDesktopWidget desktop;
    QRect mainScreenSize = desktop.screenGeometry(desktop.primaryScreen());

    int desktopWidth = mainScreenSize.width();
    int desktopHeight = mainScreenSize.height();

    this->setFixedSize(desktopWidth, desktopHeight);
    this->setWindowState(Qt::WindowFullScreen);

    ui->mainToolBar->hide();
    ui->statusBar->hide();
}

bool MainWindow::getFlagDialogMainMenuExists() const
{
    return flagDialogMainMenuExists;
}

bool MainWindow::getFlagDialogSectorSetupExists() const
{
    return flagDialogSectorSetupExists;
}

bool MainWindow::getFlagDialogSettingsExists() const
{
    return flagDialogSettingsExists;
}

void MainWindow::setFlagDialogMainMenuExists(bool flagBool)
{
    flagDialogMainMenuExists = flagBool;
}

void MainWindow::setFlagDialogSectorSetupExists(bool flagBool)
{
    flagDialogSectorSetupExists = flagBool;
}

void MainWindow::setFlagDialogSettingExists(bool flagBool)
{
    flagDialogSettingsExists = flagBool;
}

void MainWindow::setSituationalDisplayFocus()
{
    ui->situationalDisplay->setFocus();
}
