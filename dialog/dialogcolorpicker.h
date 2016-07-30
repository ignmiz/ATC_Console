#ifndef DIALOGCOLORPICKER_H
#define DIALOGCOLORPICKER_H

#include <QColorDialog>


class DialogColorPicker : public QColorDialog
{
    Q_OBJECT

public:
    explicit DialogColorPicker(QWidget *parent = 0);
    explicit DialogColorPicker(const QColor &initial, QWidget *parent = 0);
    ~DialogColorPicker();

signals:
    void signalColorPickerClosed();
};

#endif // DIALOGCOLORPICKER_H
