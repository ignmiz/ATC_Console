#ifndef ATCABSTRACTPROCEDURESYMBOL_H
#define ATCABSTRACTPROCEDURESYMBOL_H

#include "atcairspacefix.h"

#include <QString>
#include <QVector>
#include <QGraphicsItem>

class ATCAbstractProcedureSymbol
{
public:
    explicit ATCAbstractProcedureSymbol(QString name);
    virtual ~ATCAbstractProcedureSymbol() = 0;

    QString getName();
    ATCAirspaceFix* getCoords1(int iterator);
    ATCAirspaceFix* getCoords2(int iterator);
    QGraphicsLineItem* getLine(int iterator);
    int getCoordsVectorSize();

    void setName(QString name);

    void appendCoords1(ATCAirspaceFix* coord);
    void appendCoords2(ATCAirspaceFix *coord);
    void appendLine(QGraphicsLineItem *line);

    void show();
    void hide();
    bool isVisible();
    void setFlagVisible(bool flag); //TEMPORARY

private:
    QString symbolName;
    QVector<ATCAirspaceFix*> coords1;
    QVector<ATCAirspaceFix*> coords2;
    QVector<QGraphicsLineItem*> lines;

    bool visible = false;
};

#endif // ATCABSTRACTPROCEDURESYMBOL_H
