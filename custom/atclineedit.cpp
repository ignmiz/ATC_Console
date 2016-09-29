#include "atclineedit.h"

ATCLineEdit::ATCLineEdit(QWidget *parent) : QLineEdit(parent)
{
}

ATCLineEdit::~ATCLineEdit()
{
}

void ATCLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    emit(focussed(true));
}

void ATCLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit(focussed(false));
}
