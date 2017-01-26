#ifndef ATCPATHS_H
#define ATCPATHS_H

#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

class ATCPaths
{
public:
    explicit ATCPaths();
    ~ATCPaths();

    void exportPaths();

    QString PATHS_FILE = "../../ATC_Console/ATC_Console/config/paths.txt";

    QString SYMBOLOGY_DFLT_PATH;
    QString SYMBOLOGY_EXPORT_PATH;

    QString DISPLAY_DFLT_PATH;
    QString DISPLAY_EXPORT_PATH;

    QString SCENARIO_EXPORT_PATH;

    QString SCT_PATH;
    QString ESE_PATH;

    QString COMPANY_PATH;
    QString COMPANY_TEST_PATH;

    QString BADA_PATH;
    QString BADA_TEST_PATH;
    QString APF_TEST_PATH;
    QString OPF_TEST_PATH;

    QString ROUTE_PATH;
    QString ROUTE_TEST_PATH;

};

#endif // ATCPATHS_H
