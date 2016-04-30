#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "textconsolewindow.h"

#include <QDesktopWidget>
#include <QSqlDatabase>
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

void MainWindow::on_listButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QString dbName = "DRIVER={Microsoft Access Driver (*.accdb)};"
                     "FIL={MS Access};"
                     "DBQ=E:/Qt/ATC_Console/ATC_Console/AcftPerformance.accdb";

    db.setDatabaseName(dbName);

    qDebug() << QSqlDatabase::drivers();

    if(db.open())
    {
        qDebug() << "Open!";
        db.close();
    }
    else
        qDebug() << "Failed!";
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
}
