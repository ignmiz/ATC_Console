#include "textconsolewindow.h"
#include "ui_textconsolewindow.h"
#include "consolecommands.h"

#include <QDateTime>
#include <QHash>
#include <QTimer>
#include <QMouseEvent>
#include <QCursor>
#include <QDesktopWidget>
#include <QDebug>

TextConsoleWindow::TextConsoleWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextConsoleWindow)
{
    ui->setupUi(this);

//    consoleInputSetup();
    consoleDisplaySetup();

}

TextConsoleWindow::~TextConsoleWindow()
{
    delete ui;
}

void TextConsoleWindow::printToConsole(QString command)
{
    QDateTime timeStamp = QDateTime::currentDateTime();
    ui->consoleDisplay->append("[" + timeStamp.toString("hh:mm") + "] >> " + command);
}

void TextConsoleWindow::printToConsole(QStringList commandList) //DEBUG FCN
{
    foreach(QString command, commandList)
    {
        TextConsoleWindow::printToConsole(command);
    }
}

void TextConsoleWindow::setConsoleInputFocus()
{
    QTimer::singleShot(0, ui->consoleInput, SLOT(setFocus()));
}

void TextConsoleWindow::maximizeConsole()
{
    ui->minmaxButton->setText(QString::fromUtf8("▲"));
    maximizedFlag = true;
    ui->consoleInput->show();
    ui->consoleDisplay->show();
    resize(800, 600);
    setConsoleInputFocus();
}

void TextConsoleWindow::minimizeConsole()
{
    ui->minmaxButton->setText(QString::fromUtf8("▼"));
    maximizedFlag = false;
    ui->consoleInput->hide();
    ui->consoleDisplay->hide();
    resize(800, 32);
}

bool TextConsoleWindow::isMaximized()
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


bool TextConsoleWindow::isMouseOnTitleBar(QPoint mousePosition)
{
    QPoint topLeftInGlobal = QWidget::mapToGlobal(this->rect().topLeft());
    QPoint topRightInGlobal = QWidget::mapToGlobal(this->rect().topRight());

    if((mousePosition.x() >= topLeftInGlobal.x())
            && (mousePosition.x() <= topRightInGlobal.x())
            && (mousePosition.y() >= topLeftInGlobal.y())
            && (mousePosition.y() <= (topLeftInGlobal.y() + 32)))
    {
        return true;
    }
    else
        return false;
}

void TextConsoleWindow::on_consoleInput_returnPressed()
{
    QString commandString = getConsoleInputText();
    commandString = commandString.trimmed();

    if(!commandString.isEmpty())
    {
        clearConsoleInput();
        printToConsole(commandString);

        QString consoleReply = parseQuery(commandString);
        printToConsole(consoleReply);
    }
    else
    {
        clearConsoleInput();
        return;
    }
}

void TextConsoleWindow::on_closeButton_clicked()
{
    close();
}

void TextConsoleWindow::on_minmaxButton_clicked()
{
    if(maximizedFlag)
    {
        minimizeConsole();
    }
    else
    {
        maximizeConsole();
    }
}

void TextConsoleWindow::on_closeButton_pressed()
{
    flagStdButtonPressed = true;
}

void TextConsoleWindow::on_minmaxButton_pressed()
{
    flagStdButtonPressed = true;
}

void TextConsoleWindow::on_closeButton_released()
{
    flagStdButtonPressed = false;
}

void TextConsoleWindow::on_minmaxButton_released()
{
    flagStdButtonPressed = false;
}

//NOT NECESSARY FOR NOW
//void TextConsoleWindow::consoleInputSetup()
//{
//    QTimer::singleShot(0, ui->consoleInput, SLOT(setFocus()));
//}

void TextConsoleWindow::consoleDisplaySetup()
{
    ui->consoleDisplay->setReadOnly(true);
    ui->consoleDisplay->setFocusPolicy(Qt::NoFocus);
}

QString TextConsoleWindow::getConsoleInputText()
{
    return ui->consoleInput->text();
}

void TextConsoleWindow::clearConsoleInput()
{
    ui->consoleInput->clear();
}

QString TextConsoleWindow::parseQuery(QString query)
{
    QStringList queryList = query.split(" ", QString::SkipEmptyParts);
    unsigned int argumentCount = countElements(queryList);

    enum CommandRepresentation
    {
        help,
        text,
        cmpd,
        dflt
    };

    QHash<QString, CommandRepresentation> commandOptions;

    commandOptions.insert("help", help);
    commandOptions.insert("text", text);
    commandOptions.insert("compound", cmpd);

    int switchController = commandOptions.value(queryList.at(0), dflt);

    switch (switchController) {
        case help:
            return ConsoleCommands::helpCommand();
            break;

        case text:
            return ConsoleCommands::textCommand();
            break;

        case cmpd:
            return ConsoleCommands::compoundCommand(argumentCount, queryList);
            break;

        default:
            return ConsoleCommands::defaultCommand();
            break;
    }
}

unsigned int TextConsoleWindow::countElements(QStringList list)
{
    unsigned int elementCount = 0;

    foreach (QString element, list) {
        elementCount++;
    }

    return elementCount;
}

void TextConsoleWindow::getMouseEventPosition()
{
    mouseEventPosition = QCursor::pos();
}

void TextConsoleWindow::mousePressEvent(QMouseEvent *event)
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

void TextConsoleWindow::mouseMoveEvent(QMouseEvent *event)
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

void TextConsoleWindow::mouseReleaseEvent(QMouseEvent *event)
{
    flagStdButtonPressed = false;
    flagClickedOnTitleBar = false;
    event->accept();
}

void TextConsoleWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(maximizedFlag)
    {
        minimizeConsole();
    }
    else
    {
        maximizeConsole();
    }
    event->accept();
}



