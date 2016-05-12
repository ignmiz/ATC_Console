#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H

#include <QString>

class ATCConsoleCommands
{
public:
    ATCConsoleCommands();
    static QString helpCommand();
    static QString textCommand();
    static QString compoundCommand(int argumentCount, QStringList queryList);
    static QString defaultCommand();
};

#endif // CONSOLECOMMANDS_H
