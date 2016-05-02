#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QDebug>

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    mainMenuSetup();
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_buttonOK_clicked()
{
    this->close();
}

void MainMenu::on_buttonCancel_clicked()
{
    this->close();
}

void MainMenu::mainMenuSetup()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
}


