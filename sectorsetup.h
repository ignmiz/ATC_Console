#ifndef SECTORSETUP_H
#define SECTORSETUP_H

#include "atcdialog.h"

namespace Ui {
class SectorSetup;
}

class SectorSetup : public ATCDialog
{
    Q_OBJECT

public:
    explicit SectorSetup(QWidget *parent = 0);
    ~SectorSetup();

private slots:
    ATC_MOUSE_HANDLER

private:
    Ui::SectorSetup *uiInner;

};

#endif // SECTORSETUP_H
