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

signals:
    void signalHideSectorARTCCLow(QString);
    void signalHideSectorARTCCHigh(QString);
    void signalHideSectorARTCC(QString);
    void signalHideFix(QString);
    void signalHideNDB(QString);
    void signalHideVOR(QString);
    void signalHideAirport(QString);
    void signalHideSID(QString);
    void signalHideSTAR(QString);
    void signalHideAirwayLow(QString);
    void signalHideAirwayHigh(QString);

    void signalShowSectorARTCCLow(QString);
    void signalShowSectorARTCCHigh(QString);
    void signalShowSectorARTCC(QString);
    void signalShowFix(QString);
    void signalShowNDB(QString);
    void signalShowVOR(QString);
    void signalShowAirport(QString);
    void signalShowSID(QString);
    void signalShowSTAR(QString);
    void signalShowAirwayLow(QString);
    void signalShowAirwayHigh(QString);

private slots:
    ATC_MOUSE_HANDLER

    void slotColorPickerClosed();

    void slotUpdateTableColorARTCCLow(QColor color);
    void slotUpdateTableColorARTCCHigh(QColor color);
    void slotUpdateTableColorARTCC(QColor color);
    void slotUpdateTableColorVOR(QColor color);
    void slotUpdateTableColorNDB(QColor color);
    void slotUpdateTableColorFix(QColor color);
    void slotUpdateTableColorAirport(QColor color);
    void slotUpdateTableColorRunway(QColor color);
    void slotUpdateTableColorSTAR(QColor color);
    void slotUpdateTableColorSID(QColor color);
    void slotUpdateTableColorAirwayLow(QColor color);
    void slotUpdateTableColorAirwayHigh(QColor color);

    void onViewSymbologyClicked(const QModelIndex &index);
    void onViewDisplayClicked(const QModelIndex &index);

    void slotHeaderStateChanged(QStandardItem *item);

    void on_buttonSetDefaultSymbology_clicked();
    void on_buttonLoadSymbology_clicked();
    void on_buttonExportSymbology_clicked();

    void on_buttonSetDefaultDisplay_clicked();
    void on_buttonLoadDisplay_clicked();
    void on_buttonExportDisplay_clicked();

private:
    Ui::DialogSettings *uiInner;
    DialogColorPicker *dialogColorPicker = nullptr;
    ATCSituationalDisplay *situationalDisplay = nullptr;    
    QStandardItemModel *modelSymbology = nullptr;
    QStandardItemModel *modelDisplay = nullptr;

    int visibleSectorsARTCCLow = 0;
    int visibleSectorsARTCCHigh = 0;
    int visibleSectorsARTCC = 0;
    int visibleFixes = 0;
    int visibleNDBs = 0;
    int visibleVORs = 0;
    int visibleAirports = 0;
    int visibleSIDSymbols = 0;
    int visibleSTARSymbols = 0;
    int visibleAirwaysLow = 0;
    int visibleAirwaysHigh = 0;

    bool flagDialogColorPickerExists = false;

    void setupViewSymbology();
    void setupViewDisplay();

    void createModelSymbology();
    void createModelDisplay();

    void connectSlots();

    void constructColorPicker(QColor &initColor);
    QList<QStandardItem*> createSymbologyRow(QString text, QColor color);

    QList<QStandardItem*> createDisplayHeader(QString text);
    QList<QStandardItem*> createDisplayRow(QString text, bool checked, int &counter);

    template<class T> void populateModelDisplay(QString headerName, QVector<T*> const &vector, int &counter, QStandardItemModel *model);

    void decrementChildCounter(int &counter, QStandardItem *headerName, QStandardItem *headerCheckbox);
    void incrementChildCounter(int &counter, QStandardItem *headerName, QStandardItem *headerCheckbox);
};

#endif // DIALOGSETTINGS_H
