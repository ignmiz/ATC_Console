#ifndef ATCABSTRACTPROCEDURE_H
#define ATCABSTRACTPROCEDURE_H

#include <QString>
#include <QVector>

class ATCAbstractProcedure
{
public:
    explicit ATCAbstractProcedure(QString procedureName, QString airportCode, QString runway);
    virtual ~ATCAbstractProcedure() = 0;

    QString getName();
    QString getAirport();
    QString getRunwayID();
    QString getFixName(int iterator);
    int getFixListSize();

    void setName(QString procedureName);
    void setAirport(QString airportCode);
    void setRunwayID(QString runway);
    void setFixList(QStringList &list);

    void appendFixName(QString fixName);

    void show();
    void hide();
    bool isVisible();
    void setFlagVisible(bool flag); //TEMPORARY

private:
    QString name;
    QString airport;
    QString runwayID;
    QStringList fixList;

    bool visible = false;
};

#endif // ATCABSTRACTPROCEDURE_H
