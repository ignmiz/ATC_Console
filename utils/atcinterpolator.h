#ifndef ATCINTERPOLATOR_H
#define ATCINTERPOLATOR_H

#include "mkl/mkl_df.h"

#include <QMessageBox>
#include <QVector>
#include <QDebug>

//Class for linear interpolation of scalar function with saturation or linear extrapolation

enum class ExType
{
    None, Saturation, Linear
};


class ATCInterpolator
{
public:
    explicit ATCInterpolator(QVector<double> &x, QVector<double> &y, ExType type);
    ~ATCInterpolator();

    void interpolate(QVector<double> &sites, QVector<double> &results);

private:
    QVector<double> xBreakpoints;
    QVector<double> yValues;

    double *bc = NULL;
    double *ic = NULL;
    double *scoeff = NULL;

    DFTaskPtr task;

    bool constructedCorrectly = false;

    struct UserParams
    {
        QVector<double> *breakpoints;
        QVector<double> *values;
        ExType type;
    } userParams;

    bool validateConstructionArgs();
    void errorMessage(QString s, bool &valid);

    static int leftExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params);
    static int rightExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params);
};

#endif // ATCINTERPOLATOR_H
