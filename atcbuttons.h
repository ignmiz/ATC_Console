#ifndef ATCBUTTONDIALOG_H
#define ATCBUTTONDIALOG_H

#include <QPushButton>

class ATCButtonDialog : public QPushButton
{
    Q_OBJECT

public:
    explicit ATCButtonDialog(QWidget *parent = 0);
    ~ATCButtonDialog();
};


class ATCButtonMainWindow : public QPushButton
{
    Q_OBJECT

public:
    explicit ATCButtonMainWindow(QWidget *parent = 0);
    ~ATCButtonMainWindow();
};

#endif // ATCBUTTONDIALOG_H

//QPushButton
//{
//	color: #c8c8c8;
//	font: 15px;
//	background-color: #ffffff;
//	border-style: outset;
//	border-width: 2px;
//	border-color: #3e3e3e;
//}

//QPushButton::hover
//{
//	background-color: #2d2d2d;
//}

//QPushButton::pressed
//{
//	background-color: #3c3c3c;
//}

//QPushButton::hover#buttonSpacerBottom
//{
//	color: #ffffff;
//	background-color: #000000;
//	border-style: outset;
//	border-width: 2px;
//	border-color: #3e3e3e;
//}

//QPushButton::hover#buttonSpacerTop
//{
//	color: #ffffff;
//	background-color: #000000;
//	border-style: outset;
//	border-width: 2px;
//	border-color: #3e3e3e;
//}
