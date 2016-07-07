#ifndef ATCABSTRACTPROCEDURESYMBOL_H
#define ATCABSTRACTPROCEDURESYMBOL_H

#include <QString>
#include <QVector>
#include <QGraphicsItem>

class ATCAbstractProcedureSymbol
{
public:
    explicit ATCAbstractProcedureSymbol(QString name);
    virtual ~ATCAbstractProcedureSymbol() = 0;

    QString getName();
    QGraphicsLineItem* getLine(int iterator);

    void setName(QString name);

    void appendLine(QGraphicsLineItem *line);

private:
    QString symbolName;
    QVector<QGraphicsLineItem*> lines;
};

#endif // ATCABSTRACTPROCEDURESYMBOL_H
