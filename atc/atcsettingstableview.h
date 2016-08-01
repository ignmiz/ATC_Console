#ifndef ATCSETTINGSTABLEVIEW_H
#define ATCSETTINGSTABLEVIEW_H

#include <QTableView>

class ATCSettingsTableView : public QTableView
{
public:
    explicit ATCSettingsTableView(QWidget *parent = 0);
    ~ATCSettingsTableView();
};

#endif // ATCSETTINGSTABLEVIEW_H
