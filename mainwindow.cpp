#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"
#include "dialogtextconsole.h"

#include <QDesktopWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogTextConsole = new DialogTextConsole(this);

    mainWindowSetup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonMainMenu_clicked()
{
    DialogMainMenu *dialogMainMenu = new DialogMainMenu(this);
    dialogMainMenu->show();
}

void MainWindow::on_buttonSctSetup_clicked()
{
    DialogSectorSetup *dialogSectorSetup = new DialogSectorSetup(this);
    dialogSectorSetup->show();
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
