#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "dialogcolorpicker.h"
#include "atcdialog.h"
#include "atcsituationaldisplay.h"
#include "atcsettings.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QPalette>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(ATCSituationalDisplay *display, QWidget *parent = 0);
    ~DialogSettings();

private slots:
    ATC_MOUSE_HANDLER

    void slotColorPickerClosed();

    void slotUpdateTableColorARTCCLow(QColor color);
    void slotUpdateTableColorARTCCHigh(QColor color);
    void slotUpdateTableColorARTCC(QColor color);

    void onTableClicked(const QModelIndex &index);

    void on_buttonExportSettings_clicked();
    void on_buttonLoadSettings_clicked();
    void on_buttonSetDefault_clicked();

private:
    Ui::DialogSettings *uiInner;
    DialogColorPicker *dialogColorPicker = nullptr;
    ATCSituationalDisplay *situationalDisplay = nullptr;    
    QStandardItemModel *modelSymbology = nullptr;
    QStandardItemModel *modelDisplay = nullptr;

    bool flagDialogColorPickerExists = false;

    void setupViewSymbology();
    void setupViewDisplay();
    void createModelSymbology();
    void createModelDisplay();

    void connectSlots();

    void constructColorPicker(QColor &initColor);
    QList<QStandardItem*> createSymbologyRow(QString text, QColor color);

    QList<QStandardItem*> createDisplayHeader(QString text);
    QList<QStandardItem*> createDisplayRow(QString text, bool checked);

    template<class T> void populateTreeModel(QString headerName, QVector<T*> const &vector, QStandardItemModel *model);
};

#endif // DIALOGSETTINGS_H
