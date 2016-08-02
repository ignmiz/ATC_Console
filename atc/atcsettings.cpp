#include "atcsettings.h"

#include <QDir>
#include <QDebug>

ATCSettings::ATCSettings()
{
    assignDefaultPath();
    loadSettings(SETTINGS_DFLT_PATH);
//    loadSettings("E:/Qt/ATC_Console/ATC_Console/config/user/user1.txt");
//    loadSettings("E:/Qt/ATC_Console/ATC_Console/config/user/user2.txt");
}

ATCSettings::~ATCSettings()
{

}

void ATCSettings::assignDefaultPath()
{
    QString paths("E:/Qt/ATC_Console/ATC_Console/config/paths.txt"); //HERE SHOULD BE RELATIVE INSTALLATION PATH TO paths.txt

    QFile pathsFile(paths);

    if(!pathsFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening paths.txt file...";
        return;
    }

    QTextStream stream(&pathsFile);
    while(!stream.atEnd())
    {
        QString textLine = stream.readLine();
        textLine = textLine.trimmed();

        QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

        if(stringList.at(0).trimmed() == "DEFAULT")
        {
            SETTINGS_DFLT_PATH = stringList.at(1).trimmed();
        }
    }

    pathsFile.close();
}

void ATCSettings::loadSettings(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening file";
        return;
    }

    bool flagINFO = false;
    bool flagARTCCLow = false;
    bool flagARTCCHigh = false;
    bool flagARTCC = false;
    bool flagVOR = false;
    bool flagNDB = false;
    bool flagFixes = false;
    bool flagAirport = false;
    bool flagRunway = false;
    bool flagSTAR = false;
    bool flagSID = false;
    bool flagLowAirway = false;
    bool flagHighAirway = false;

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString textLine = stream.readLine();
        textLine = textLine.trimmed();

        if(!textLine.isEmpty())
        {
            QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

            if(textLine.contains("[INFO]", Qt::CaseInsensitive))
            {
                flagINFO = true;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = true;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = true;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = true;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(flagINFO)
            {
                if(stringList.at(0).trimmed() == "NAME")
                {
                    SETTINGS_NAME = stringList.at(1).trimmed();
                }
            }
            else if(flagARTCCLow)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_LOW_COLOR = colorFromString(stringList.at(1));
                }
            }
            else if(flagARTCCHigh)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_HIGH_COLOR = colorFromString(stringList.at(1));
                }
            }
            else if(flagARTCC)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_COLOR = colorFromString(stringList.at(1));
                }
            }
        }
    }
}

QRgb ATCSettings::colorFromString(QString string)
{
    QStringList list = string.trimmed().split(",", QString::SkipEmptyParts);
    int red = list.at(0).trimmed().toInt();
    int green = list.at(1).trimmed().toInt();
    int blue = list.at(2).trimmed().toInt();

    return qRgb(red, green, blue);
}
