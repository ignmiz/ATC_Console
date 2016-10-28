#include "mainwindow.h"

#include "atccompanyfactory.h"
#include "atcaircrafttypefactory.h"
#include "atcroutefactory.h"

#include "atcpaths.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    ATCPaths paths;

    ATCCompanyFactory companyFactory(paths.COMPANY_PATH);
    ATCAircraftTypeFactory typeFactory(paths.BADA_PATH);
    ATCRouteFactory routeFactory(paths.ROUTE_PATH);

    return a.exec();
}
