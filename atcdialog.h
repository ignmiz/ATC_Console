#ifndef ATCDIALOG_H
#define ATCDIALOG_H

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

private slots:
    void on_buttonClose_clicked();
    void on_buttonMinMax_clicked();
    void on_buttonClose_pressed();
    void on_buttonMinMax_pressed();
    void on_buttonClose_released();
    void on_buttonMinMax_released();

private:
    Ui::ATCDialog *ui;
    unsigned int windowWidth;
    unsigned int windowHeight;
    QString windowTitle;
    bool flagDeleteOnClose;

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

    void windowSetup();
};

#endif // ATCDIALOG_H
