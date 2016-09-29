#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
//#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
