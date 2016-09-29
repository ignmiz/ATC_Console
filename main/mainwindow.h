#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "dialogtextconsole.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"
#include "dialogsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isDialogTextConsoleVisible() const;

    bool getFlagDialogMainMenuExists() const;
    bool getFlagDialogSectorSetupExists() const;
    bool getFlagDialogSettingsExists() const;

    void setFlagDialogMainMenuExists(bool flagBool);
    void setFlagDialogSectorSetupExists(bool flagBool);
    void setFlagDialogSettingExists(bool flagBool);

    void setSituationalDisplayFocus();

private slots:
    void on_buttonMainMenu_clicked();
    void on_buttonSctSetup_clicked();
    void on_buttonClose_clicked();
    void on_buttonSettings_clicked();
    void on_buttonShowConsole_clicked();

    void dialogMainMenuClosed();
    void dialogSectorSetupClosed();
    void dialogSettingsClosed();

    void changeFocusToDisplay();

private:
    Ui::MainWindow *ui;
    DialogTextConsole *dialogTextConsole;
    DialogMainMenu *dialogMainMenu;
    DialogSectorSetup *dialogSectorSetup;
    DialogSettings *dialogSettings;

    bool flagDialogMainMenuExists = false;
    bool flagDialogSectorSetupExists = false;
    bool flagDialogSettingsExists = false;

    void mainWindowSetup();
};

#endif // MAINWINDOW_H
