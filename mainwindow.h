#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "dialogtextconsole.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool getFlagDialogMainMenuExists() const;
    bool getFlagDialogSectorSetupExists() const;

    void setFlagDialogMainMenuExists(bool flagBool);
    void setFlagDialogSectorSetupExists(bool flagBool);
    void setSituationalDisplayFocus();

private slots:
    void on_buttonMainMenu_clicked();
    void on_buttonSctSetup_clicked();
    void on_buttonClose_clicked();
    void on_buttonShowConsole_clicked();

private:
    Ui::MainWindow *ui;
    DialogTextConsole *dialogTextConsole;
    DialogMainMenu *dialogMainMenu;
    DialogSectorSetup *dialogSectorSetup;

    QGraphicsScene *scene;
    QGraphicsRectItem *rect1, *rect2, *rect3, *rect4;
    QGraphicsLineItem *lineH, *lineV;

    bool flagDialogMainMenuExists = false;
    bool flagDialogSectorSetupExists = false;

    void mainWindowSetup();
    void situationalDisplaySetup();
};

#endif // MAINWINDOW_H
