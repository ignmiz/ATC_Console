
#include "atcpaths.h"

ATCPaths::ATCPaths()
{
    QFile pathsFile(PATHS_FILE);

    if(!pathsFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ATCPaths: Failed to open path: " + PATHS_FILE);
        msgBox.exec();

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
            else if(stringList.at(0).trimmed() == "SCT")
            {
                SCT_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "ESE")
            {
                ESE_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "COMPANY")
            {
                COMPANY_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "COMPANY TEST")
            {
                COMPANY_TEST_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "BADA")
            {
                BADA_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "BADA TEST")
            {
                BADA_TEST_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "APF TEST")
            {
                APF_TEST_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "OPF TEST")
            {
                OPF_TEST_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "ROUTE")
            {
                ROUTE_PATH = stringList.at(1).trimmed();
            }
            else if(stringList.at(0).trimmed() == "ROUTE TEST")
            {
                ROUTE_TEST_PATH = stringList.at(1).trimmed();
            }
        }
    }

    pathsFile.close();
}

ATCPaths::~ATCPaths()
{

}
