#include "atcsettings.h"

#include <QDir>
#include <QDebug>

ATCSettings::ATCSettings()
{
    assignPaths();
    loadInitialSettings(SETTINGS_DFLT_PATH);
}

ATCSettings::~ATCSettings()
{

}

void ATCSettings::assignPaths()
{
    QFile pathsFile(SETTINGS_PATHS_FILE);

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
        else if(stringList.at(0).trimmed() == "EXPORT")
        {
            SETTINGS_EXPORT_PATH = stringList.at(1).trimmed();
        }
    }

    pathsFile.close();
}

void ATCSettings::setDefaultPath(QString newPath)
{
    QFile pathsFile(SETTINGS_PATHS_FILE);

    if(!pathsFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error while opening " + SETTINGS_PATHS_FILE + " file";
        return;
    }

    QTextStream out(&pathsFile);

    out << "DEFAULT = " << newPath << endl;
    out << "EXPORT = " << SETTINGS_EXPORT_PATH << endl;

    pathsFile.close();

    SETTINGS_DFLT_PATH = newPath;
}

void ATCSettings::loadInitialSettings(QString path)
{
    interpretSettingsFile(path);

    SETTINGS_ACTIVE_PATH = path;
}

void ATCSettings::interpretSettingsFile(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " << path;
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

    file.close();
}

void ATCSettings::exportSettings(QString path)
{
    QStringList pathElements = path.split("/", QString::KeepEmptyParts);
    QString fileName = pathElements.at(pathElements.size() - 1).trimmed();
    QString nameWithoutExtension = fileName.split(".", QString::KeepEmptyParts).at(0).trimmed();

    QFile file(path);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " + path;
        return;
    }

    QTextStream out(&file);

    out << "[INFO]" << endl;
    out << "NAME = " << nameWithoutExtension << endl;
    out << endl;

    out << "[ARTCC LOW]" << endl;
    out << "COLOR = " << ARTCC_LOW_COLOR.red() << ", " << ARTCC_LOW_COLOR.green() << ", " << ARTCC_LOW_COLOR.blue() << endl;
    out << endl;

    out << "[ARTCC HIGH]" << endl;
    out << "COLOR = " << ARTCC_HIGH_COLOR.red() << ", " << ARTCC_HIGH_COLOR.green() << ", " << ARTCC_HIGH_COLOR.blue() << endl;
    out << endl;

    out << "[ARTCC]" << endl;
    out << "COLOR = " << ARTCC_COLOR.red() << ", " << ARTCC_COLOR.green() << ", " << ARTCC_COLOR.blue() << endl;

    file.close();
}

void ATCSettings::loadSettings(QString path)
{
    interpretSettingsFile(path);

    emit signalColorARTCCLow(ARTCC_LOW_COLOR);
    emit signalColorARTCCHigh(ARTCC_HIGH_COLOR);
    emit signalColorARTCC(ARTCC_COLOR);

    SETTINGS_ACTIVE_PATH = path;
}

bool ATCSettings::fileExists(QString path)
{
    QFileInfo checkFile(path);

    return (checkFile.exists() && checkFile.isFile());
}

QRgb ATCSettings::colorFromString(QString string)
{
    QStringList list = string.trimmed().split(",", QString::SkipEmptyParts);
    int red = list.at(0).trimmed().toInt();
    int green = list.at(1).trimmed().toInt();
    int blue = list.at(2).trimmed().toInt();

    return qRgb(red, green, blue);
}
