#ifndef ATCDIALOG_H
#define ATCDIALOG_H

#include "atcmousehandler.h"
#include <QDialog>

namespace Ui {
class ATCDialog;
}

class ATCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ATCDialog(QWidget *parent = 0, QString title = "Title", unsigned int width = 400,
                       unsigned int height = 300, bool deleteOnClose = true);
    ~ATCDialog();

    void maximizeWindow();
    void minimizeWindow();
    bool isMaximized();
    bool isMouseOnTitleBar(QPoint mousePosition);

private:
    Ui::ATCDialog *ui;
    unsigned int windowWidth;
    unsigned int windowHeight;
    QString windowTitle;
    bool flagDeleteOnClose;

    bool maximizedFlag = true;
    bool flagClickedOnTitleBar = false;
    QPoint mouseEventPosition;
    QPoint mouseDragPosition;
    void getMouseEventPosition();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

protected:
    bool flagStdButtonPressed = false;
    void windowSetup();
};

#endif // ATCDIALOG_H
