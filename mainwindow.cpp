#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"
#include "dialogtextconsole.h"

#include <QDesktopWidget>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogTextConsole = new DialogTextConsole(this);

    mainWindowSetup();
    setSituationalDisplayFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
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
        setFlagDialogMainMenuExists(true);
        setSituationalDisplayFocus();
    }
}

void MainWindow::on_buttonSctSetup_clicked()
{
    if(!getFlagDialogSectorSetupExists())
    {
        dialogSectorSetup = new DialogSectorSetup(this);
        dialogSectorSetup->show();
        setFlagDialogSectorSetupExists(true);
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

void MainWindow::setFlagDialogMainMenuExists(bool flagBool)
{
    flagDialogMainMenuExists = flagBool;
}

void MainWindow::setFlagDialogSectorSetupExists(bool flagBool)
{
    flagDialogSectorSetupExists = flagBool;
}

void MainWindow::setSituationalDisplayFocus()
{
//    QTimer::singleShot(0, ui->situationalDisplay, SLOT(setFocus()));
    ui->situationalDisplay->setFocus();
}
