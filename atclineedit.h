#ifndef ATCLINEEDIT_H
#define ATCLINEEDIT_H

#include <QLineEdit>

class ATCLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit ATCLineEdit(QWidget *parent = 0);
    ~ATCLineEdit();

signals:
    void focussed(bool hasFocus);

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

};

#endif // ATCLINEEDIT_H
