#include "mainwindow.h"

#include "atccompanyfactory.h"
#include "atcaircrafttypefactory.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    ATCCompanyFactory companyFactory("E:/Qt/ATC_Console/ATC_Console/data/company.txt");
    ATCAircraftTypeFactory typeFactory("E:/Magisterka/BADA13.3/files");

    return a.exec();
}
