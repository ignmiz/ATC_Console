#include "dialogcolorpicker.h"

DialogColorPicker::DialogColorPicker(QWidget *parent) : QColorDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

DialogColorPicker::DialogColorPicker(const QColor &initial, QWidget *parent) : QColorDialog(initial, parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

DialogColorPicker::~DialogColorPicker()
{
    emit signalColorPickerClosed();
}
