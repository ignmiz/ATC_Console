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

    void maximizeWindow();
    void minimizeWindow();
    bool isMaximized();
    bool isMouseOnTitleBar(QPoint mousePosition);

private slots:
    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

    void on_buttonClose_clicked();
    void on_buttonMinMax_clicked();
    void on_buttonClose_pressed();
    void on_buttonMinMax_pressed();
    void on_buttonClose_released();
    void on_buttonMinMax_released();

private:
    Ui::MainMenu *ui;

    void mainMenuSetup();

    bool maximizedFlag = true;
    bool flagStdButtonPressed = false;
    bool flagClickedOnTitleBar = false;
    QPoint mouseEventPosition;
    QPoint mouseDragPosition;
    void getMouseEventPosition();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MAINMENU_H
