#include "atcsettings.h"

#include <QDir>
#include <QDebug>

ATCSettings::ATCSettings()
{
    assignDefaultPath();

    ARTCC_LOW_COLOR = qRgb(90, 230, 230);
    ARTCC_HIGH_COLOR =  qRgb(255, 255, 255);
    ARTCC_COLOR = qRgb(255, 0, 0);
}

ATCSettings::~ATCSettings()
{

}

void ATCSettings::assignDefaultPath()
{
    QString paths("E:/Qt/ATC_Console/ATC_Console/settings/paths.txt"); //HERE SHOULD BE RELATIVE INSTALLATION PATH TO paths.txt

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
