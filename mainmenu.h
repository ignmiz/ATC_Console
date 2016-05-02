#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

private slots:
    void on_buttonOK_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::MainMenu *ui;

    void mainMenuSetup();
};

#endif // MAINMENU_H
