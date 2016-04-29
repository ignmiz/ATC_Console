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

    void maximizeConsole();
    void minimizeConsole();
    bool isMaximized();
    bool isMouseOnTitleBar(QPoint mousePosition);

private slots:
    void on_consoleInput_returnPressed();

    void on_closeButton_clicked();
    void on_minmaxButton_clicked();
    void on_closeButton_pressed();
    void on_minmaxButton_pressed();
    void on_minmaxButton_released();
    void on_closeButton_released();

private:
    Ui::TextConsoleWindow *ui;
    bool maximizedFlag = true;

//    void consoleInputSetup();
    void consoleDisplaySetup();
    QString getConsoleInputText();
    void clearConsoleInput();
    QString parseQuery(QString query);

    unsigned int countElements(QStringList list);

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
