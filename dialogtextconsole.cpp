#include "dialogtextconsole.h"
#include "ui_dialogtextconsole.h"
#include "consolecommands.h"

#include <QDateTime>
#include <QTimer>
#include <QHash>

DialogTextConsole::DialogTextConsole(QWidget *parent) :
    ATCDialog(parent, "Text Console", 800, 600, false),
    uiInner(new Ui::DialogTextConsole)
{
    uiInner->setupUi(this);
    windowSetup();
    consoleDisplaySetup();

    this->hide();
}

DialogTextConsole::~DialogTextConsole()
{
    delete uiInner;
}

void DialogTextConsole::printToConsole(QString command)
{
    QDateTime timeStamp = QDateTime::currentDateTime();
    uiInner->consoleDisplay->append("[" + timeStamp.toString("hh:mm") + "] >> " + command);
}

void DialogTextConsole::printToConsole(QStringList commandList) //DEBUG FCN
{
    foreach(QString command, commandList)
    {
        DialogTextConsole::printToConsole(command);
    }
}

void DialogTextConsole::setConsoleInputFocus()
{
    QTimer::singleShot(0, uiInner->consoleInput, SLOT(setFocus()));
}

void DialogTextConsole::on_consoleInput_returnPressed()
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

void DialogTextConsole::consoleDisplaySetup()
{
    uiInner->consoleDisplay->setReadOnly(true);
    uiInner->consoleDisplay->setFocusPolicy(Qt::NoFocus);
}

QString DialogTextConsole::getConsoleInputText()
{
    return uiInner->consoleInput->text();
}

void DialogTextConsole::clearConsoleInput()
{
    uiInner->consoleInput->clear();
}

QString DialogTextConsole::parseQuery(QString query)
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

unsigned int DialogTextConsole::countElements(QStringList list)
{
    unsigned int elementCount = 0;

    foreach (QString element, list) {
        elementCount++;
    }

    return elementCount;
}
