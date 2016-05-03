#include "atcdialog.h"
#include "ui_atcdialog.h"

#include <QMouseEvent>
#include <QCursor>
#include <QDebug>

ATCDialog::ATCDialog(QWidget *parent, QString title, unsigned int width,
                     unsigned int height, bool deleteOnClose) :
    QDialog(parent),
    ui(new Ui::ATCDialog),
    windowTitle(title),
    windowWidth(width),
    windowHeight(height),
    flagDeleteOnClose(deleteOnClose)
{
    ui->setupUi(this);
    windowSetup();
}

ATCDialog::~ATCDialog()
{
    delete ui;
}

void ATCDialog::maximizeWindow()
{
    ui->buttonMinMax->setText(QString::fromUtf8("▲"));
    maximizedFlag = true;
    resize(windowWidth, windowHeight);
    ui->frameDialog->resize(windowWidth, windowHeight);
}

void ATCDialog::minimizeWindow()
{
    ui->buttonMinMax->setText(QString::fromUtf8("▼"));
    maximizedFlag = false;
    resize(windowWidth, 30);
    ui->frameDialog->resize(windowWidth, 30);
}

bool ATCDialog::isMaximized()
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

bool ATCDialog::isMouseOnTitleBar(QPoint mousePosition)
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

void ATCDialog::on_buttonClose_clicked()
{
    close();
}

void ATCDialog::on_buttonMinMax_clicked()
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

void ATCDialog::on_buttonClose_pressed()
{
    flagStdButtonPressed = true;
}

void ATCDialog::on_buttonMinMax_pressed()
{
    flagStdButtonPressed = true;
}

void ATCDialog::on_buttonClose_released()
{
    flagStdButtonPressed = false;
}

void ATCDialog::on_buttonMinMax_released()
{
    flagStdButtonPressed = false;
}

void ATCDialog::getMouseEventPosition()
{
    mouseEventPosition = QCursor::pos();
}

void ATCDialog::mousePressEvent(QMouseEvent *event)
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

void ATCDialog::mouseMoveEvent(QMouseEvent *event)
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

void ATCDialog::mouseReleaseEvent(QMouseEvent *event)
{
    flagStdButtonPressed = false;
    flagClickedOnTitleBar = false;
    event->accept();
}

void ATCDialog::mouseDoubleClickEvent(QMouseEvent *event)
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

void ATCDialog::windowSetup()
{
    if(flagDeleteOnClose)
        this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->resize(windowWidth, windowHeight);
    ui->frameDialog->resize(windowWidth, windowHeight);
    ui->frameTitleBar->resize(windowWidth, 30);

    ui->labelTitle->setGeometry(10, 0, windowWidth/2, 30);
    ui->labelTitle->setText(windowTitle);

    ui->buttonMinMax->move(windowWidth - 50, 5);
    ui->buttonClose->move(windowWidth - 30, 5);
}
