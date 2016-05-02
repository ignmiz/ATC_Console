#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "textconsolewindow.h"
#include "mainmenu.h"

#include <QDesktopWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    txtConsole = new TextConsoleWindow(this);

    mainWindowSetup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonMainMenu_clicked()
{
    MainMenu *mainMenu = new MainMenu(this);
    mainMenu->show();
}

void MainWindow::on_buttonShowConsole_clicked()
{
    if(txtConsole->isHidden())
    {
        if(!(txtConsole->isMaximized()))
        {
            txtConsole->maximizeConsole();
        }
        txtConsole->setWindowFlags(Qt::FramelessWindowHint);
        txtConsole->show();
        txtConsole->setConsoleInputFocus();
    }
    else
    {
        txtConsole->hide();
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


