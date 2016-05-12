#include "atcconsolecommands.h"
#include <QStringList>

ATCConsoleCommands::ATCConsoleCommands()
{

}

QString ATCConsoleCommands::helpCommand()
{
    return "Help command selected...";
}

QString ATCConsoleCommands::textCommand()
{
    return "Text command selected...";
}

QString ATCConsoleCommands::compoundCommand(int argumentCount, QStringList queryList)
{
    if((argumentCount == 2) && (queryList.at(1).toInt() || (queryList.at(1) == "0")))
    {
        if(queryList.at(1).toInt() > 0)
        {
            return "Compound command with parameter: positive";
        }
        else
        {
            if((queryList.at(1).toInt()) == 0)
            {
                return "Compound command with parameter: zero";
            }

            return "Compound command with parameter: negative";
        }
    }
    else
    {
        return "Command not found...";
    }
}

QString ATCConsoleCommands::defaultCommand()
{
    return "Command not found...";
}
