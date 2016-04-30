#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textconsolewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonShowConsole_clicked();
    void on_buttonClose_clicked();
    void on_listButton_clicked();

private:
    Ui::MainWindow *ui;
    TextConsoleWindow *txtConsole;
    void mainWindowSetup();
};

#endif // MAINWINDOW_H
