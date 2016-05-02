#ifndef TEXTCONSOLEWINDOW_H
#define TEXTCONSOLEWINDOW_H

#include <QDialog>

namespace Ui {
class TextConsoleWindow;
}

class TextConsoleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TextConsoleWindow(QWidget *parent = 0);
    ~TextConsoleWindow();

    void printToConsole(QString command);
    void printToConsole(QStringList compoundCommand);   //DEBUG FCN

    void setConsoleInputFocus();

    void maximizeWindow();
    void minimizeWindow();
    bool isMaximized();
    bool isMouseOnTitleBar(QPoint mousePosition);

private slots:
    void on_consoleInput_returnPressed();

    void on_buttonClose_clicked();
    void on_buttonMinMax_clicked();
    void on_buttonClose_pressed();
    void on_buttonMinMax_pressed();
    void on_buttonClose_released();
    void on_buttonMinMax_released();


private:
    Ui::TextConsoleWindow *ui;

//    void consoleInputSetup();
    void consoleDisplaySetup();
    QString getConsoleInputText();
    void clearConsoleInput();
    QString parseQuery(QString query);

    unsigned int countElements(QStringList list);

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

#endif // TEXTCONSOLEWINDOW_H
