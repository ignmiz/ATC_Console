
#include "atcinterpolator.h"

ATCInterpolator::ATCInterpolator(QVector<double> &x, QVector<double> &y, ExType type) :
    xBreakpoints(x),
    yValues(y),
    userParams{&xBreakpoints, &yValues, type}
{
    if(!validateConstructionArgs()) return;

    //Create task
    MKL_INT nx = xBreakpoints.size();
    double *xArray = &xBreakpoints[0];
    MKL_INT xhint = DF_NON_UNIFORM_PARTITION;

    MKL_INT ny = 1;
    double *yArray = &yValues[0];
    MKL_INT yhint = DF_MATRIX_STORAGE_ROWS;

    dfdNewTask1D(&task, nx, xArray, xhint, ny, yArray, yhint);

    //Edit task parameters: linear interpolation
    MKL_INT s_order = DF_PP_LINEAR;
    MKL_INT s_type = DF_PP_DEFAULT;

    MKL_INT bc_type = DF_NO_BC;
    bc = NULL;

    MKL_INT ic_type = DF_NO_IC;
    ic = NULL;

    scoeff = new double[(nx - 1) * s_order];
    MKL_INT scoeffhint = DF_NO_HINT;

    dfdEditPPSpline1D(task, s_order, s_type, bc_type, bc, ic_type, ic, scoeff, scoeffhint);

    //Construct spline
    MKL_INT s_format = DF_PP_SPLINE;
    MKL_INT method = DF_METHOD_STD;

    dfdConstruct1D(task, s_format, method);

    //Set construction flag
    constructedCorrectly = true;
}

ATCInterpolator::~ATCInterpolator()
{
    if(constructedCorrectly)
    {
        if(task != nullptr) dfDeleteTask(&task);

        if(bc != NULL) delete bc;
        if(ic != NULL) delete ic;
        if(scoeff != NULL) delete[] scoeff;
    }
}

void ATCInterpolator::setExtrapolationType(ExType t)
{
    userParams.type = t;
}

ExType ATCInterpolator::getExtrapolationType()
{
    return userParams.type;
}

bool ATCInterpolator::isCorrect()
{
    return constructedCorrectly;
}

void ATCInterpolator::interpolate(QVector<double> &sites, QVector<double> &results)
{
    //results vector needs to be allocated according to DF_MATRIX_STORAGE_FUNCS_SITES_DERS (results[j][s][d]) directive
    //respective sizes: results[ny][nsite][nder]
    //member access: results[d + nder*(s + nsite*j)]

    //CAUTION! This function does not check for outliers when called with ExType::none due efficiency reasons.
    //         In such case results will be incorrect.

    //Check for object validity
    if(!constructedCorrectly) return;

    //Check for argument validity
    if(sites.size() != results.size())
    {
        QMessageBox msgBox;
        msgBox.setText("ATCInterpolator: Sites and Results vectors must be equal.\n"
                       "Object contruction failed.");
        msgBox.exec();
        return;
    }

    MKL_INT type = DF_INTERP;
    MKL_INT method = DF_METHOD_PP;

    MKL_INT nsite = sites.size();
    MKL_INT sitehint = DF_NON_UNIFORM_PARTITION;

    MKL_INT ndorder = 1;
    MKL_INT dorder[1] = {0};
    double *datahint = NULL;

    MKL_INT rhint = DF_MATRIX_STORAGE_FUNCS_SITES_DERS;

    MKL_INT *cell = NULL;

    if(userParams.type != ExType::None)
    {
        dfdInterpCallBack le_cb = &leftExtrapolation;
        dfdInterpCallBack re_cb = &rightExtrapolation;

        dfdInterpolateEx1D(task, type, method, nsite, &sites[0], sitehint, ndorder, dorder, datahint, &results[0], rhint, cell, le_cb,
                           &userParams, re_cb, &userParams, NULL, NULL, NULL, NULL);
    }
    else
    {
        dfdInterpolate1D(task, type, method, nsite, &sites[0], sitehint, ndorder, dorder, datahint, &results[0], rhint, cell);
    }
}

bool ATCInterpolator::validateConstructionArgs()
{
    bool isValid = true;

    if(xBreakpoints.isEmpty())
    {
        errorMessage("ATCInterpolator: Breakpoints vector is empty\n"
                     "Object contruction failed.", isValid);
    }
    else if(yValues.isEmpty())
    {
        errorMessage("ATCInterpolator: Values vector is empty.\n"
                     "Object contruction failed.", isValid);
    }
    else if(xBreakpoints.size() != yValues.size())
    {
        errorMessage("ATCInterpolator: Breakpoints and Values sizes must be equal.\n"
                     "Object contruction failed.", isValid);
    }
    else if((xBreakpoints.size() == 1) || (yValues.size() == 1))
    {
        errorMessage("ATCInterpolator: Cannot interpolate using single point.\n"
                     "Object contruction failed.", isValid);
    }

    for(int i = 0; i < xBreakpoints.size() - 1; i++)
    {
        if(xBreakpoints.at(i) >= xBreakpoints.at(i + 1))
        {
            errorMessage("ATCInterpolator: Breakpoints must be strictly ascending.\n"
                         "Object contruction failed.", isValid);
            break;
        }
    }

    return isValid;
}

void ATCInterpolator::errorMessage(QString s, bool &valid)
{
    QMessageBox msgBox;
    msgBox.setText(s);
    msgBox.exec();

    valid = false;
}

int ATCInterpolator::leftExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params)
{
    Q_UNUSED(cell)
    Q_UNUSED(library_params)

    UserParams *params = reinterpret_cast<UserParams*>(user_param);

    QVector<double> *breakpoints = params->breakpoints;
    QVector<double> *values = params->values;

    if(params->type == ExType::Saturation)
    {
        for(int i = 0; i < *n; i++)
        {
            r[i] = values->at(0);
        }

        return 0;
    }
    else if(params->type == ExType::Linear)
    {
        double dy_dx = (values->at(1) - values->at(0)) / (breakpoints->at(1) - breakpoints->at(0));

        for(int i = 0; i < *n; i++)
        {
            r[i] = values->at(0) - dy_dx * (breakpoints->at(0) - site[i]);
        }

        return 0;
    }

    return -1;
}

int ATCInterpolator::rightExtrapolation(MKL_INT64 *n, MKL_INT64 cell[], double site[], double r[], void *user_param, dfInterpCallBackLibraryParams *library_params)
{
    Q_UNUSED(cell)
    Q_UNUSED(library_params)

    UserParams *params = reinterpret_cast<UserParams*>(user_param);

    QVector<double> *breakpoints = params->breakpoints;
    QVector<double> *values = params->values;

    double bSize = breakpoints->size();
    double vSize = values->size();

    if(params->type == ExType::Saturation)
    {
        for(int i = 0; i < *n; i++)
        {
            r[i] = values->at(vSize - 1);
        }

        return 0;
    }
    else if(params->type == ExType::Linear)
    {
        double dy_dx = (values->at(vSize - 1) - values->at(vSize - 2)) / (breakpoints->at(bSize - 1) - breakpoints->at(bSize - 2));

        for(int i = 0; i < *n; i++)
        {
            r[i] = values->at(vSize - 1) + dy_dx * (site[i] - breakpoints->at(bSize - 1));
        }

        return 0;
    }

    return -1;
}


