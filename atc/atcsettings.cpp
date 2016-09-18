#include "atcsettings.h"

#include <QDir>
#include <QDebug>

ATCSettings::ATCSettings()
{
    assignPaths();
    loadInitialSymbology(SYMBOLOGY_DFLT_PATH);
}

ATCSettings::~ATCSettings()
{

}

void ATCSettings::assignPaths()
{
    QFile pathsFile(PATHS_FILE);

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

        if(!textLine.isEmpty())
        {
            QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

            if(stringList.at(0).trimmed() == "DEFAULT SYMBOLOGY")
            {
                SYMBOLOGY_DFLT_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "EXPORT SYMBOLOGY")
            {
                SYMBOLOGY_EXPORT_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "DEFAULT DISPLAY")
            {
                DISPLAY_DFLT_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "EXPORT DISPLAY")
            {
                DISPLAY_EXPORT_PATH = stringList.at(1).trimmed();
            }
        }
    }

    pathsFile.close();
}

void ATCSettings::exportDefaultPathsFile()
{
    QFile pathsFile(PATHS_FILE);

    if(!pathsFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error while opening " + PATHS_FILE + " file";
        return;
    }

    QTextStream out(&pathsFile);

    out << "DEFAULT SYMBOLOGY = " << SYMBOLOGY_DFLT_PATH << endl;
    out << "EXPORT SYMBOLOGY = " << SYMBOLOGY_EXPORT_PATH << endl;
    out << "DEFAULT DISPLAY = " << DISPLAY_DFLT_PATH << endl;
    out << "EXPORT DISPLAY = " << DISPLAY_EXPORT_PATH << endl;

    pathsFile.close();
}

void ATCSettings::loadInitialSymbology(QString path)
{
    interpretSymbologyFile(path);

    SYMBOLOGY_ACTIVE_PATH = path;
}

void ATCSettings::interpretSymbologyFile(QString path)
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
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = true;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = true;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = true;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = true;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = true;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = true;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = true;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
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
                flagLowAirway = true;
                flagHighAirway = false;
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
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
                flagHighAirway = true;
            }
            else if(flagINFO)
            {
                if(stringList.at(0).trimmed() == "NAME")
                {
                    SYMBOLOGY_NAME = stringList.at(1).trimmed();
                }
            }
            else if(flagARTCCLow)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_LOW_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    ARTCC_LOW_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagARTCCHigh)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_HIGH_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    ARTCC_HIGH_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagARTCC)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ARTCC_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    ARTCC_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagVOR)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    VOR_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    VOR_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "SIDE LENGTH")
                {
                    VOR_SIDE_LENGTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    VOR_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DX")
                {
                    VOR_LABEL_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DY")
                {
                    VOR_LABEL_DY = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagNDB)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    NDB_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    NDB_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "SYMBOL DIAMETER")
                {
                    NDB_SYMBOL_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    NDB_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DX")
                {
                    NDB_LABEL_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DY")
                {
                    NDB_LABEL_DY = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagFixes)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    FIX_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    FIX_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "SIDE LENGTH")
                {
                    FIX_SIDE_LENGTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    FIX_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DX")
                {
                    FIX_LABEL_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DY")
                {
                    FIX_LABEL_DY = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagAirport)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    AIRPORT_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    AIRPORT_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "SYMBOL DIAMETER")
                {
                    AIRPORT_SYMBOL_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    AIRPORT_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DX")
                {
                    AIRPORT_LABEL_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DY")
                {
                    AIRPORT_LABEL_DY = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagRunway)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    RUNWAY_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    RUNWAY_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "CENTRELINE LENGTH")
                {
                    RUNWAY_CENTELINE_LENGTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "FIRST TICK DISTANCE")
                {
                    TICK_FIRST_DISTANCE = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "TICK SEPARATION")
                {
                    TICK_SEPARATION = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "FIRST MAJOR TICK AT")
                {
                    TICK_FIRST_MAJOR_AT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MAJOR TICK SEPARATION")
                {
                    TICK_MAJOR_SEPARATION = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MINOR TICK LENGTH")
                {
                    TICK_MINOR_LENGTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MAJOR TICK LENGTH")
                {
                    TICK_MAJOR_LENGTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagSTAR)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    STAR_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    STAR_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagSID)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    SID_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    SID_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagLowAirway)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    AIRWAY_LOW_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    AIRWAY_LOW_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flagHighAirway)
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    AIRWAY_HIGH_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    AIRWAY_HIGH_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
            }
        }
    }

    file.close();
}

void ATCSettings::exportSymbology(QString path)
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
    out << "LINE WIDTH = " << ARTCC_LOW_LINE_WIDTH << endl;
    out << endl;

    out << "[ARTCC HIGH]" << endl;
    out << "COLOR = " << ARTCC_HIGH_COLOR.red() << ", " << ARTCC_HIGH_COLOR.green() << ", " << ARTCC_HIGH_COLOR.blue() << endl;
    out << "LINE WIDTH = " << ARTCC_HIGH_LINE_WIDTH << endl;
    out << endl;

    out << "[ARTCC]" << endl;
    out << "COLOR = " << ARTCC_COLOR.red() << ", " << ARTCC_COLOR.green() << ", " << ARTCC_COLOR.blue() << endl;
    out << "LINE WIDTH = " << ARTCC_LINE_WIDTH << endl;
    out << endl;

    out << "[VOR]" << endl;
    out << "COLOR = " << VOR_COLOR.red() << ", " << VOR_COLOR.green() << ", " << VOR_COLOR.blue() << endl;
    out << "LINE WIDTH = " << VOR_LINE_WIDTH << endl;
    out << "SIDE LENGTH = " << VOR_SIDE_LENGTH << endl;
    out << "LABEL HEIGHT = " << VOR_LABEL_HEIGHT << endl;
    out << "LABEL DX = " << VOR_LABEL_DX << endl;
    out << "LABEL DY = " << VOR_LABEL_DY << endl;
    out << endl;

    out << "[NDB]" << endl;
    out << "COLOR = " << NDB_COLOR.red() << ", " << NDB_COLOR.green() << ", " << NDB_COLOR.blue() << endl;
    out << "LINE WIDTH = " << NDB_LINE_WIDTH << endl;
    out << "SYMBOL DIAMETER = " << NDB_SYMBOL_DIA << endl;
    out << "LABEL HEIGHT = " << NDB_LABEL_HEIGHT << endl;
    out << "LABEL DX = " << NDB_LABEL_DX << endl;
    out << "LABEL DY = " << NDB_LABEL_DY << endl;
    out << endl;

    out << "[FIXES]" << endl;
    out << "COLOR = " << FIX_COLOR.red() << ", " << FIX_COLOR.green() << ", " << FIX_COLOR.blue() << endl;
    out << "LINE WIDTH = " << FIX_LINE_WIDTH << endl;
    out << "SIDE LENGTH = " << FIX_SIDE_LENGTH << endl;
    out << "LABEL HEIGHT = " << FIX_LABEL_HEIGHT << endl;
    out << "LABEL DX = " << FIX_LABEL_DX << endl;
    out << "LABEL DY = " << FIX_LABEL_DY << endl;
    out << endl;

    out << "[AIRPORT]" << endl;
    out << "COLOR = " << AIRPORT_COLOR.red() << ", " << AIRPORT_COLOR.green() << ", " << AIRPORT_COLOR.blue() << endl;
    out << "LINE WIDTH = " << AIRPORT_LINE_WIDTH << endl;
    out << "SYMBOL DIAMETER = " << AIRPORT_SYMBOL_DIA << endl;
    out << "LABEL HEIGHT = " << AIRPORT_LABEL_HEIGHT << endl;
    out << "LABEL DX = " << AIRPORT_LABEL_DX << endl;
    out << "LABEL DY = " << AIRPORT_LABEL_DY << endl;
    out << endl;

    out << "[RUNWAY]" << endl;
    out << "COLOR = " << RUNWAY_COLOR.red() << ", " << RUNWAY_COLOR.green() << ", " << RUNWAY_COLOR.blue() << endl;
    out << "LINE WIDTH = " << RUNWAY_LINE_WIDTH << endl;
    out << "CENTRELINE LENGTH = " << RUNWAY_CENTELINE_LENGTH << endl;
    out << "FIRST TICK DISTANCE = " << TICK_FIRST_DISTANCE << endl;
    out << "TICK SEPARATION = " << TICK_SEPARATION << endl;
    out << "FIRST MAJOR TICK AT = " << TICK_FIRST_MAJOR_AT << endl;
    out << "MAJOR TICK SEPARATION = " << TICK_MAJOR_SEPARATION << endl;
    out << "MINOR TICK LENGTH = " << TICK_MINOR_LENGTH << endl;
    out << "MAJOR TICK LENGTH = " << TICK_MAJOR_LENGTH << endl;
    out << endl;

    out << "[STAR]" << endl;
    out << "COLOR = " << STAR_COLOR.red() << ", " << STAR_COLOR.green() << ", " << STAR_COLOR.blue() << endl;
    out << "LINE WIDTH = " << STAR_LINE_WIDTH << endl;
    out << endl;

    out << "[SID]" << endl;
    out << "COLOR = " << SID_COLOR.red() << ", " << SID_COLOR.green() << ", " << SID_COLOR.blue() << endl;
    out << "LINE WIDTH = " << SID_LINE_WIDTH << endl;
    out << endl;

    out << "[LOW AIRWAY]" << endl;
    out << "COLOR = " << AIRWAY_LOW_COLOR.red() << ", " << AIRWAY_LOW_COLOR.green() << ", " << AIRWAY_LOW_COLOR.blue() << endl;
    out << "LINE WIDTH = " << AIRWAY_LOW_LINE_WIDTH << endl;
    out << endl;

    out << "[HIGH AIRWAY]" << endl;
    out << "COLOR = " << AIRWAY_HIGH_COLOR.red() << ", " << AIRWAY_HIGH_COLOR.green() << ", " << AIRWAY_HIGH_COLOR.blue() << endl;
    out << "LINE WIDTH = " << AIRWAY_HIGH_LINE_WIDTH << endl;

    file.close();
}

void ATCSettings::loadSymbology(QString path)
{
    interpretSymbologyFile(path);

    emit signalColorARTCCLow(ARTCC_LOW_COLOR);
    emit signalColorARTCCHigh(ARTCC_HIGH_COLOR);
    emit signalColorARTCC(ARTCC_COLOR);
    emit signalColorVOR(VOR_COLOR);
    emit signalColorNDB(NDB_COLOR);
    emit signalColorFix(FIX_COLOR);
    emit signalColorAirport(AIRPORT_COLOR);
    emit signalColorRunway(RUNWAY_COLOR);
    emit signalColorSTAR(STAR_COLOR);
    emit signalColorSID(SID_COLOR);
    emit signalColorAirwayLow(AIRWAY_LOW_COLOR);
    emit signalColorAirwayHigh(AIRWAY_HIGH_COLOR);

    emit signalApplySettings();

    SYMBOLOGY_ACTIVE_PATH = path;
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
