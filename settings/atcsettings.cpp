#include "atcsettings.h"

#include <QDir>

ATCSettings::ATCSettings(ATCPaths *p) : paths(p)
{
    loadInitialSymbology(paths->SYMBOLOGY_DFLT_PATH);
}

ATCSettings::~ATCSettings()
{

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
        QMessageBox msgBox;
        msgBox.setText("ATCSettings: Failed to open path: " + path);
        msgBox.exec();

        return;
    }

    QString flag;

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
                flag = "[INFO]";
            }
            else if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
                flag = "[ARTCC LOW]";
            }
            else if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
                flag = "[ARTCC HIGH]";
            }
            else if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
                flag = "[ARTCC]";
            }
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flag = "[VOR]";
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flag = "[NDB]";
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flag = "[FIXES]";
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flag = "[AIRPORT]";
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flag = "[RUNWAY]";
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
                flag = "[STAR]";
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
                flag = "[SID]";
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
                flag = "[LOW AIRWAY]";
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
                flag = "[HIGH AIRWAY]";
            }
            else if(textLine.contains("[TAG]", Qt::CaseInsensitive))
            {
                flag = "[TAG]";
            }
            else if(textLine.contains("[ROUTE]", Qt::CaseInsensitive))
            {
                flag = "[ROUTE]";
            }
            else if(textLine.contains("[RULER]", Qt::CaseInsensitive))
            {
                flag = "[RULER]";
            }
            else if(textLine.contains("[TRAILING DOTS]", Qt::CaseInsensitive))
            {
                flag = "[TRAILING DOTS]";
            }
            else if(textLine.contains("[AMAN]", Qt::CaseInsensitive))
            {
                flag = "[AMAN]";
            }
            else if(flag == "[INFO]")
            {
                if(stringList.at(0).trimmed() == "NAME")
                {
                    SYMBOLOGY_NAME = stringList.at(1).trimmed();
                }
            }
            else if(flag == "[ARTCC LOW]")
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
            else if(flag == "[ARTCC HIGH]")
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
            else if(flag == "[ARTCC]")
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
            else if(flag == "[VOR]")
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
            else if(flag == "[NDB]")
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
            else if(flag == "[FIXES]")
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
            else if(flag == "[AIRPORT]")
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
            else if(flag == "[RUNWAY]")
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
            else if(flag == "[STAR]")
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
            else if(flag == "[SID]")
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
            else if(flag == "[LOW AIRWAY]")
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
            else if(flag == "[HIGH AIRWAY]")
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
            else if(flag == "[TAG]")
            {
                if(stringList.at(0).trimmed() == "DIAMOND WIDTH")
                {
                    TAG_DIAMOND_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "DIAMOND DIAMETER")
                {
                    TAG_DIAMOND_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "DIAMOND LINE")
                {
                    TAG_DIAMOND_LINE = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LEADER LENGTH")
                {
                    TAG_LEADER_LENGTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LEADER WIDTH")
                {
                    TAG_LEADER_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LEADER UNIT")
                {
                    if(stringList.at(1).trimmed() == "NM")
                    {
                        TAG_LEADER_UNIT = ATC::LeaderNM;
                    }
                    else
                    {
                        TAG_LEADER_UNIT = ATC::LeaderMIN;
                    }
                }
                else if(stringList.at(0).trimmed() == "BOX WIDTH")
                {
                    TAG_BOX_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BOX HEIGHT")
                {
                    TAG_BOX_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BOX HEIGHT FULL")
                {
                    TAG_BOX_HEIGHT_FULL = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BOX DX")
                {
                    TAG_BOX_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BOX DY")
                {
                    TAG_BOX_DY = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BOX COLOR")
                {
                    TAG_BOX_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "CONNECTOR WIDTH")
                {
                    TAG_CONNECTOR_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    TAG_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL MARGINS")
                {
                    TAG_LABEL_MARGINS = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL COLOR")
                {
                    TAG_LABEL_COLOR = colorFromString(stringList.at(1));
                }
            }
            else if(flag == "[ROUTE]")
            {
                if(stringList.at(0).trimmed() == "COLOR")
                {
                    ROUTE_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    ROUTE_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    ROUTE_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DX")
                {
                    ROUTE_LABEL_DX = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL DY")
                {
                    ROUTE_LABEL_DY = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "TOC DIAMETER")
                {
                    ROUTE_TOC_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "TOC COLOR")
                {
                    ROUTE_TOC_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "TOD DIAMETER")
                {
                    ROUTE_TOD_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "TOD COLOR")
                {
                    ROUTE_TOD_COLOR = colorFromString(stringList.at(1));
                }
            }
            else if(flag == "[RULER]")
            {
                if(stringList.at(0).trimmed() == "END COLOR")
                {
                    RULER_END_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "END DIAMETER")
                {
                    RULER_END_DIA = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "END LINE WIDTH")
                {
                    RULER_END_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LINE COLOR")
                {
                    RULER_LINE_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LINE WIDTH")
                {
                    RULER_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "LABEL COLOR")
                {
                    RULER_LABEL_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "LABEL HEIGHT")
                {
                    RULER_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flag == "[TRAILING DOTS]")
            {
                if(stringList.at(0).trimmed() == "COUNT")
                {
                    TRAILING_COUNT = stringList.at(1).trimmed().toInt();
                }
                else if(stringList.at(0).trimmed() == "COLOR")
                {
                    TRAILING_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "DIAMETER")
                {
                    TRAILING_DIA = stringList.at(1).trimmed().toDouble();
                }
            }
            else if(flag == "[AMAN]")
            {
                if(stringList.at(0).trimmed() == "TIMELINE COLOR")
                {
                    AMAN_TIMELINE_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "TIMELINE LINE WIDTH")
                {
                    AMAN_TIMELINE_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "TIMELINE LABEL COLOR")
                {
                    AMAN_TIMELINE_LABEL_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "TIMELINE LABEL HEIGHT")
                {
                    AMAN_TIMELINE_LABEL_HEIGHT = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "HORIZON COLOR")
                {
                    AMAN_HORIZON_COLOR = colorFromString(stringList.at(1));
                }
                else if(stringList.at(0).trimmed() == "HORIZON LINE WIDTH")
                {
                    AMAN_HORIZON_LINE_WIDTH = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "BAR SPAN")
                {
                    AMAN_BAR_SPAN = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MAJOR INNER SPAN")
                {
                    AMAN_MAJOR_INNER_SPAN = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MAJOR OUTER SPAN")
                {
                    AMAN_MAJOR_OUTER_SPAN = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MINOR INNER SPAN")
                {
                    AMAN_MINOR_INNER_SPAN = stringList.at(1).trimmed().toDouble();
                }
                else if(stringList.at(0).trimmed() == "MINOR OUTER SPAN")
                {
                    AMAN_MINOR_OUTER_SPAN = stringList.at(1).trimmed().toDouble();
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
        QMessageBox msgBox;
        msgBox.setText("ATCSettings: Failed to open path: " + path);
        msgBox.exec();

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
    out << endl;

    out << "[TAG]" << endl;
    out << "DIAMOND WIDTH = " << TAG_DIAMOND_WIDTH << endl;
    out << "DIAMOND DIAMETER = " << TAG_DIAMOND_DIA << endl;
    out << "DIAMOND LINE = " << TAG_DIAMOND_LINE << endl;
    out << "LEADER LENGTH = " << TAG_LEADER_LENGTH << endl;
    out << "LEADER WIDTH = " << TAG_LEADER_WIDTH << endl;
    out << "BOX WIDTH = " << TAG_BOX_WIDTH << endl;
    out << "LEADER UNIT = " << ((TAG_LEADER_UNIT == ATC::LeaderNM) ? "NM" : "MIN") << endl;
    out << "BOX HEIGHT = " << TAG_BOX_HEIGHT << endl;
    out << "BOX HEIGHT FULL = " << TAG_BOX_HEIGHT_FULL << endl;
    out << "BOX DX = " << TAG_BOX_DX << endl;
    out << "BOX DY = " << TAG_BOX_DY << endl;
    out << "BOX COLOR = " << TAG_BOX_COLOR.red() << ", " << TAG_BOX_COLOR.green() << ", " << TAG_BOX_COLOR.blue() << endl;
    out << "CONNECTOR WIDTH = " << TAG_CONNECTOR_WIDTH << endl;
    out << "LABEL HEIGHT = " << TAG_LABEL_HEIGHT << endl;
    out << "LABEL MARGINS = " << TAG_LABEL_MARGINS << endl;
    out << "LABEL COLOR = " << TAG_LABEL_COLOR.red() << ", " << TAG_LABEL_COLOR.green() << ", " << TAG_LABEL_COLOR.blue() << endl;
    out << endl;

    out << "[ROUTE]" << endl;
    out << "COLOR = " << ROUTE_COLOR.red() << ", " << ROUTE_COLOR.green() << ", " << ROUTE_COLOR.blue() << endl;
    out << "LINE WIDTH = " << ROUTE_LINE_WIDTH << endl;
    out << "LABEL HEIGHT = " << ROUTE_LABEL_HEIGHT << endl;
    out << "LABEL DX = " << ROUTE_LABEL_DX << endl;
    out << "LABEL DY = " << ROUTE_LABEL_DY << endl;
    out << "TOC DIAMETER = " << ROUTE_TOC_DIA << endl;
    out << "TOC COLOR = " << ROUTE_TOC_COLOR.red() << ", " << ROUTE_TOC_COLOR.green() << ", " << ROUTE_TOC_COLOR.blue() << endl;
    out << "TOD DIAMETER = " << ROUTE_TOD_DIA << endl;
    out << "TOD COLOR = " << ROUTE_TOD_COLOR.red() << ", " << ROUTE_TOD_COLOR.green() << ", " << ROUTE_TOD_COLOR.blue() << endl;
    out << endl;

    out << "[RULER]" << endl;
    out << "END COLOR = " << RULER_END_COLOR.red() << ", " << RULER_END_COLOR.green() << ", " << RULER_END_COLOR.blue() << endl;
    out << "END DIAMETER = " << RULER_END_DIA << endl;
    out << "END LINE WIDTH = " << RULER_END_LINE_WIDTH << endl;
    out << "LINE COLOR = " << RULER_LINE_COLOR.red() << ", " << RULER_LINE_COLOR.green() << ", " << RULER_LINE_COLOR.blue() << endl;
    out << "LINE WIDTH = " << RULER_LINE_WIDTH << endl;
    out << "LABEL COLOR = " << RULER_LABEL_COLOR.red() << ", " << RULER_LABEL_COLOR.green() << ", " << RULER_LABEL_COLOR.blue() << endl;
    out << "LABEL HEIGHT = " << RULER_LABEL_HEIGHT << endl;
    out << endl;

    out << "[TRAILING DOTS]" << endl;
    out << "COUNT = " << TRAILING_COUNT << endl;
    out << "COLOR = " << TRAILING_COLOR.red() << ", " << TRAILING_COLOR.green() << ", " << TRAILING_COLOR.blue() << endl;
    out << "DIAMETER = " << TRAILING_DIA << endl;
    out << endl;

    out << "[AMAN]" << endl;
    out << "TIMELINE COLOR = " << AMAN_TIMELINE_COLOR.red() << ", " << AMAN_TIMELINE_COLOR.green() << ", " << AMAN_TIMELINE_COLOR.blue() << endl;
    out << "TIMELINE LINE WIDTH = " << AMAN_TIMELINE_LINE_WIDTH << endl;
    out << "TIMELINE LABEL COLOR = " << AMAN_TIMELINE_LABEL_COLOR.red() << ", " << AMAN_TIMELINE_LABEL_COLOR.green() << ", " << AMAN_TIMELINE_LABEL_COLOR.blue() << endl;
    out << "TIMELINE LABEL HEIGHT = " << AMAN_TIMELINE_LABEL_HEIGHT << endl;
    out << "HORIZON COLOR = " << AMAN_HORIZON_COLOR.red() << ", " << AMAN_HORIZON_COLOR.green() << ", " << AMAN_HORIZON_COLOR.blue() << endl;
    out << "HORIZON LINE WIDTH = " << AMAN_HORIZON_LINE_WIDTH << endl;
    out << "BAR SPAN = " << AMAN_BAR_SPAN << endl;
    out << "MAJOR INNER SPAN = " << AMAN_MAJOR_INNER_SPAN << endl;
    out << "MAJOR OUTER SPAN = " << AMAN_MAJOR_OUTER_SPAN << endl;
    out << "MINOR INNER SPAN = " << AMAN_MINOR_INNER_SPAN << endl;
    out << "MINOR OUTER SPAN = " << AMAN_MINOR_OUTER_SPAN << endl;

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
    emit signalColorCentreline(RUNWAY_COLOR);
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
