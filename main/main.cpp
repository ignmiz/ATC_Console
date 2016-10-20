#include "mainwindow.h"

#include "atccompanyfactory.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    ATCCompanyFactory companyFactory("E:/Qt/ATC_Console/ATC_Console/data/company.txt");

    return a.exec();
}
