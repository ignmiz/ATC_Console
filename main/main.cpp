#include "mainwindow.h"

#include "atcflightfactory.h"
#include "atcpaths.h"
#include "atcsimulation.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ATCPaths paths;
    ATCFlightFactory *flightFactory = new ATCFlightFactory(paths);

    ATCSimulation *simulation = new ATCSimulation();

    MainWindow mainWindow(flightFactory, simulation);
    mainWindow.show();    

    return a.exec();
}
