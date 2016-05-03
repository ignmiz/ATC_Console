#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QMouseEvent>
#include <QCursor>
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

void MainMenu::maximizeWindow()
{
    ui->buttonMinMax->setText(QString::fromUtf8("▲"));
    maximizedFlag = true;
    resize(640, 480);
    ui->frameDialog->resize(640, 480);
}

void MainMenu::minimizeWindow()
{
    ui->buttonMinMax->setText(QString::fromUtf8("▼"));
    maximizedFlag = false;
    resize(640, 30);
    ui->frameDialog->resize(640, 30);
}

bool MainMenu::isMaximized()
{
    if(maximizedFlag)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool MainMenu::isMouseOnTitleBar(QPoint mousePosition)
{
    QPoint topLeftInGlobal = QWidget::mapToGlobal(this->rect().topLeft());
    QPoint topRightInGlobal = QWidget::mapToGlobal(this->rect().topRight());

    if((mousePosition.x() >= topLeftInGlobal.x())
            && (mousePosition.x() <= topRightInGlobal.x())
            && (mousePosition.y() >= topLeftInGlobal.y())
            && (mousePosition.y() <= (topLeftInGlobal.y() + 30)))
    {
        return true;
    }
    else
        return false;
}

void MainMenu::on_buttonClose_clicked()
{
    close();
}

void MainMenu::on_buttonMinMax_clicked()
{
    if(maximizedFlag)
    {
        minimizeWindow();
    }
    else
    {
        maximizeWindow();
    }
}

void MainMenu::on_buttonClose_pressed()
{
    flagStdButtonPressed = true;
}

void MainMenu::on_buttonMinMax_pressed()
{
    flagStdButtonPressed = true;
}

void MainMenu::on_buttonClose_released()
{
    flagStdButtonPressed = false;
}

void MainMenu::on_buttonMinMax_released()
{
    flagStdButtonPressed = false;
}

void MainMenu::mainMenuSetup()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainMenu::getMouseEventPosition()
{
    mouseEventPosition = QCursor::pos();
}

void MainMenu::mousePressEvent(QMouseEvent *event)
{
    getMouseEventPosition();
    if(isMouseOnTitleBar(mouseEventPosition))
    {
        flagClickedOnTitleBar = true;
    }

    if(event->button() == Qt::LeftButton)
    {
        mouseDragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainMenu::mouseMoveEvent(QMouseEvent *event)
{
    if(flagStdButtonPressed || !flagClickedOnTitleBar)
    {
        event->ignore();
    }
    else
    {
        if(event->buttons() & Qt::LeftButton)
        {
            move(event->globalPos() - mouseDragPosition);
            event->accept();
        }
        else
            event->ignore();
    }
}

void MainMenu::mouseReleaseEvent(QMouseEvent *event)
{
    flagStdButtonPressed = false;
    flagClickedOnTitleBar = false;
    event->accept();
}

void MainMenu::mouseDoubleClickEvent(QMouseEvent *event)
{
    getMouseEventPosition();
    if(isMouseOnTitleBar(mouseEventPosition))
    {
        if(maximizedFlag)
        {
            minimizeWindow();
        }
        else
        {
            maximizeWindow();
        }
        event->accept();
    }
    else
        event->ignore();
}


