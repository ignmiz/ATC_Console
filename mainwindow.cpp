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
    situationalDisplaySetup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonMainMenu_clicked()
{
    if(!getFlagDialogMainMenuExists())
    {
        DialogMainMenu *dialogMainMenu = new DialogMainMenu(this);
        dialogMainMenu->show();
        setFlagDialogMainMenuExists(true);
    }
}

void MainWindow::on_buttonSctSetup_clicked()
{
    if(!getFlagDialogSectorSetupExists())
    {
        DialogSectorSetup *dialogSectorSetup = new DialogSectorSetup(this);
        dialogSectorSetup->show();
        setFlagDialogSectorSetupExists(true);
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

void MainWindow::situationalDisplaySetup()
{
//    ui->situationalDisplay->setDragMode(QGraphicsView::ScrollHandDrag);

    scene = new QGraphicsScene(this);
    ui->situationalDisplay->setScene(scene);

    QBrush brush(Qt::gray);

    QPen pen(Qt::green);
    pen.setWidth(3);

    QPen penLine(Qt::white);
    penLine.setWidth(5);

    rect1 = scene->addRect(-250, -250, 100, 100, pen, brush);
    rect2 = scene->addRect(-250, 150, 100, 100, pen, brush);
    rect3 = scene->addRect(150, 150, 100, 100, pen, brush);
    rect4 = scene->addRect(150, -250, 100, 100, pen, brush);

    lineH = scene->addLine(-25, 0, 25, 0, penLine);
    lineV = scene->addLine(0, -25, 0, 25, penLine);

//    rect->setFlag(QGraphicsItem::ItemIsMovable);
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
    QTimer::singleShot(0, ui->situationalDisplay, SLOT(setFocus()));
}
