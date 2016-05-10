#ifndef DIALOGTEXTCONSOLE_H
#define DIALOGTEXTCONSOLE_H

#include "atcdialog.h"

namespace Ui {
class DialogTextConsole;
}

class DialogTextConsole : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogTextConsole(QWidget *parent = 0);
    ~DialogTextConsole();

    void printToConsole(QString command);
    void printToConsole(QStringList compoundCommand);   //DEBUG FCN
    void setConsoleInputFocus();

private slots:
    ATC_MOUSE_HANDLER

    void on_consoleInput_returnPressed();

private:
    Ui::DialogTextConsole *uiInner;

    void consoleSetup();
    QString getConsoleInputText();
    void clearConsoleInput();
    QString parseQuery(QString query);

    unsigned int countElements(QStringList list);

    void mousePressEvent(QMouseEvent *event);
};

#endif // DIALOGTEXTCONSOLE_H
