#ifndef ATCDIALOG_H
#define ATCDIALOG_H

#include "atcmousehandler.h"
#include "atcflags.h"

#include <QDialog>

namespace Ui {
class ATCDialog;
}

class ATCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ATCDialog(QWidget *parent = 0, QString title = "Title", unsigned int width = 400,
                       unsigned int height = 300, bool deleteOnClose = true, ATC::ATCDialogType dialogtype = ATC::Default);
    ~ATCDialog() = 0;

    void maximizeWindow();
    void minimizeWindow();

    bool isMaximized() const;
    bool isMouseOnTitleBar(QPoint mousePosition) const;

    bool getFlagStdButtonPressed() const;
    bool getFlagClickedOnTitleBar() const;
    QPoint getMouseEventPosition() const;
    QPoint getMouseDragPosition() const;

    void setFlagMaximized(bool flagBool);
    void setFlagClickedOnTitleBar(bool flagBool);
    void setFlagStdButtonPressed(bool flagBool);
    void setMouseDragPosition(QMouseEvent *event);

signals:
    void closed();
    void changeFocusToDisplay();

private slots:
    void setSituationDisplayFocus();

private:
    Ui::ATCDialog *ui;
    ATC::ATCDialogType dialogType;

    unsigned int windowWidth;
    unsigned int windowHeight;
    QString windowTitle;
    bool flagDeleteOnClose;

    bool flagMaximized = true;
    bool flagClickedOnTitleBar = false;
    bool flagStdButtonPressed = false;
    QPoint mouseDragPosition;

protected:
    void windowSetup();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // ATCDIALOG_H
