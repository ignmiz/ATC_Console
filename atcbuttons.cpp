#include "atcbuttons.h"

ATCButtonDialog::ATCButtonDialog(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet(
                "ATCButtonDialog"
                "{"
                "   color: #c8c8c8;"
                "   font: 15px;"
                "   background-color: #000000;"
                "   border-style: outset;"
                "   border-width: 2px;"
                "   border-color: #3e3e3e;"
                "}"

                "ATCButtonDialog::hover"
                "{"
                "   background-color: #2d2d2d;"
                "}"

                "ATCButtonDialog::pressed"
                "{"
                "   background-color: #3c3c3c;"
                "}"
                );
}

ATCButtonDialog::~ATCButtonDialog()
{
}

ATCButtonMainWindow::ATCButtonMainWindow(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet(
                "ATCButtonMainWindow"
                "{"
                "   color: #c8c8c8;"
                "   font: 15px;"
                "   background-color: #000000;"
                "   border-style: outset;"
                "   border-width: 2px;"
                "   border-color: #3e3e3e;"
                "}"

                "ATCButtonMainWindow::hover"
                "{"
                "   background-color: #2d2d2d;"
                "}"

                "ATCButtonMainWindow::pressed"
                "{"
                "   background-color: #3c3c3c;"
                "}"
                );
}

ATCButtonMainWindow::~ATCButtonMainWindow()
{
}
