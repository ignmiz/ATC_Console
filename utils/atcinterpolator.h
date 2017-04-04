#ifndef ATCINTERPOLATOR_H
#define ATCINTERPOLATOR_H

#include "mkl/mkl_df.h"

#include <QVector>
#include <QDebug>

enum class ExType
{
    None, Saturation, Tangent
};

class ATCInterpolator
{
public:
    explicit ATCInterpolator();
    explicit ATCInterpolator(QVector<double> &x, QVector<double> &y, ExType type);
    explicit ATCInterpolator(const ATCInterpolator &other);
    ~ATCInterpolator();

    void setExtrapolationType(ExType t);
    ExType getExtrapolationType();

    bool isCorrect();

    void interpolate(QVector<double> &sites, QVector<double> &results);

    ATCInterpolator& operator=(ATCInterpolator const &other);

private:
    QVector<double> xBreakpoints;
    QVector<double> yValues;

    double *bc = NULL;
    double *ic = NULL;
    double *scoeff = NULL;

    DFTaskPtr task = nullptr;

    bool constructedCorrectly = false;

    struct UserParams
    {
        QVector<double> *breakpoints;
        QVector<double> *values;
        ExType type;
    } userParams;

    void initializeTask();

    bool validateConstructionArgs();
    void errorMessage(QString s, bool &valid);

    static int leftExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params);
    static int rightExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params);
};

#endif // ATCINTERPOLATOR_H
