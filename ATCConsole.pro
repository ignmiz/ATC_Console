#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T19:35:12
#
#-------------------------------------------------

QT       += core gui positioning

CONFIG += c++14

CONFIG(debug): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic_d
CONFIG(release): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic

LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/mkl/lib/intel64_win" -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core
LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/compiler/lib/intel64_win" -llibiomp5md

VPATH += ./bin \
    ./custom \
    ./dialog \
    ./display \
    ./flight \
    ./include \
    ./lib \
    ./main \
    ./nav \
    ./settings \
    ./tools \
    ./ui \
    ./utils

INCLUDEPATH += ./bin \
    ./custom \
    ./dialog \
    ./display \
    ./flight \
    ./include \
    ./lib \
    ./main \
    ./nav \
    ./settings \
    ./tools \
    ./ui \
    ./utils

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATCConsole
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    atcdialog.cpp \
    dialogmainmenu.cpp \
    dialogsectorsetup.cpp \
    dialogtextconsole.cpp \
    atclineedit.cpp \
    atcbuttons.cpp \
    atcconsolecommands.cpp \
    atcsituationaldisplay.cpp \
    atcairspacefix.cpp \
    atcairspacesector.cpp \
    atcairspace.cpp \
    atcconst.cpp \
    atcnavfix.cpp \
    atcbeaconvor.cpp \
    atcbeaconndb.cpp \
    atcairport.cpp \
    atcrunway.cpp \
    atcrunwayextendedcentreline.cpp \
    atcabstractbeacon.cpp \
    atcabstractfix.cpp \
    atcabstractprocedure.cpp \
    atcproceduresid.cpp \
    atcprocedurestar.cpp \
    atcabstractproceduresymbol.cpp \
    atcproceduresidsymbol.cpp \
    atcprocedurestarsymbol.cpp \
    atcabstractairway.cpp \
    atcairwaylow.cpp \
    atcairwayhigh.cpp \
    atcabstractsector.cpp \
    atcsectorartcclow.cpp \
    atcsectorartcchigh.cpp \
    atcsectorartcc.cpp \
    dialogsettings.cpp \
    dialogcolorpicker.cpp \
    atcsettings.cpp \
    atctableviewsettings.cpp \
    atccompany.cpp \
    atccompanyfactory.cpp \
    atcaircrafttype.cpp \
    atcaircrafttypefactory.cpp \
    atcpaths.cpp \
    atcroute.cpp \
    atcroutefactory.cpp \
    atcflightplan.cpp \
    atcflightplanfactory.cpp \
    atcflightnumberfactory.cpp \
    atcmath.cpp \
    atcflight.cpp \
    atcflightfactory.cpp \
    dialogflight.cpp \
    dialogflightcreator.cpp \
    atcsimulation.cpp \
    atcflighttag.cpp \
    atctagrect.cpp \
    dialogaltitude.cpp \
    dialogspeed.cpp \
    dialogheading.cpp \
    dialogroute.cpp \
    dialoghandoff.cpp \
    dialogflightplan.cpp \
    atcrouteprediction.cpp \
    dialogactiverunways.cpp \
    atcactiverunways.cpp \
    atccombodelegate.cpp \
    atcclock.cpp \
    atcsimulationcontroller.cpp \
    atctagdiamond.cpp \
    dialogleaders.cpp \
    atcruler.cpp \
    atctrailingdot.cpp \
    dialog/dialogtrailingdots.cpp \
    utils/atcinterpolator.cpp \
    flight/atcabstractprofile.cpp \
    flight/atcprofileclimb.cpp \
    flight/atcprofiledescent.cpp \
    dialog/dialogflightlist.cpp \
    flight/atcpredictor.cpp \
    flight/atcpredictorcontroller.cpp \
    flight/atcprofilespeed.cpp

HEADERS  += mainwindow.h \
    atcdialog.h \
    atcmousehandler.h \
    dialogmainmenu.h \
    dialogsectorsetup.h \
    dialogtextconsole.h \
    atclineedit.h \
    atcbuttons.h \
    atcconsolecommands.h \
    atcsituationaldisplay.h \
    atcflags.h \
    atcairspacefix.h \
    atcairspacesector.h \
    atcairspace.h \
    atcconst.h \
    atcnavfix.h \
    atcbeaconvor.h \
    atcbeaconndb.h \
    atcairport.h \
    atcrunway.h \
    atcrunwayextendedcentreline.h \
    atcabstractbeacon.h \
    atcabstractfix.h \
    atcabstractprocedure.h \
    atcproceduresid.h \
    atcprocedurestar.h \
    atcabstractproceduresymbol.h \
    atcproceduresidsymbol.h \
    atcprocedurestarsymbol.h \
    atcabstractairway.h \
    atcairwaylow.h \
    atcairwayhigh.h \
    atcabstractsector.h \
    atcsectorartcclow.h \
    atcsectorartcchigh.h \
    atcsectorartcc.h \
    dialogsettings.h \
    dialogcolorpicker.h \
    atcsettings.h \
    atctableviewsettings.h \
    atccompany.h \
    atccompanyfactory.h \
    atcaircrafttype.h \
    atcaircrafttypefactory.h \
    atcpaths.h \
    atcroute.h \
    atcroutefactory.h \
    atcflightplan.h \
    atcflightplanfactory.h \
    atcflightnumberfactory.h \
    atcmath.h \
    atcflight.h \
    atcflightfactory.h \
    dialogflight.h \
    dialogflightcreator.h \
    atcsimulation.h \
    atcflighttag.h \
    atctagrect.h \
    dialogaltitude.h \
    dialogspeed.h \
    dialogheading.h \
    dialogroute.h \
    dialoghandoff.h \
    dialogflightplan.h \
    atcrouteprediction.h \
    dialogactiverunways.h \
    atcactiverunways.h \
    atccombodelegate.h \
    atcclock.h \
    atcsimulationcontroller.h \
    include/GeographicLib/Accumulator.hpp \
    include/GeographicLib/AlbersEqualArea.hpp \
    include/GeographicLib/AzimuthalEquidistant.hpp \
    include/GeographicLib/CassiniSoldner.hpp \
    include/GeographicLib/CircularEngine.hpp \
    include/GeographicLib/Config.h \
    include/GeographicLib/Constants.hpp \
    include/GeographicLib/DMS.hpp \
    include/GeographicLib/Ellipsoid.hpp \
    include/GeographicLib/EllipticFunction.hpp \
    include/GeographicLib/GARS.hpp \
    include/GeographicLib/Geocentric.hpp \
    include/GeographicLib/GeoCoords.hpp \
    include/GeographicLib/Geodesic.hpp \
    include/GeographicLib/GeodesicExact.hpp \
    include/GeographicLib/GeodesicLine.hpp \
    include/GeographicLib/GeodesicLineExact.hpp \
    include/GeographicLib/Geohash.hpp \
    include/GeographicLib/Geoid.hpp \
    include/GeographicLib/Georef.hpp \
    include/GeographicLib/Gnomonic.hpp \
    include/GeographicLib/GravityCircle.hpp \
    include/GeographicLib/GravityModel.hpp \
    include/GeographicLib/LambertConformalConic.hpp \
    include/GeographicLib/LocalCartesian.hpp \
    include/GeographicLib/MagneticCircle.hpp \
    include/GeographicLib/MagneticModel.hpp \
    include/GeographicLib/Math.hpp \
    include/GeographicLib/MGRS.hpp \
    include/GeographicLib/NormalGravity.hpp \
    include/GeographicLib/OSGB.hpp \
    include/GeographicLib/PolarStereographic.hpp \
    include/GeographicLib/PolygonArea.hpp \
    include/GeographicLib/Rhumb.hpp \
    include/GeographicLib/SphericalEngine.hpp \
    include/GeographicLib/SphericalHarmonic.hpp \
    include/GeographicLib/SphericalHarmonic1.hpp \
    include/GeographicLib/SphericalHarmonic2.hpp \
    include/GeographicLib/TransverseMercator.hpp \
    include/GeographicLib/TransverseMercatorExact.hpp \
    include/GeographicLib/Utility.hpp \
    include/GeographicLib/UTMUPS.hpp \
    atctagdiamond.h \
    dialogleaders.h \
    atcruler.h \
    atctrailingdot.h \
    dialog/dialogtrailingdots.h \
    include/mkl/mkl.h \
    include/mkl/mkl_version.h \
    include/mkl/mkl_blas.h \
    include/mkl/mkl_cblas.h \
    include/mkl/mkl_lapack.h \
    include/mkl/mkl_spblas.h \
    include/mkl/mkl_trans.h \
    include/mkl/mkl_types.h \
    include/mkl/mkl_blacs.h \
    include/mkl/mkl_dss.h \
    include/mkl/mkl_lapacke.h \
    include/mkl/mkl_pardiso.h \
    include/mkl/mkl_sparse_handle.h \
    include/mkl/mkl_dfti.h \
    include/mkl/mkl_direct_call.h \
    include/mkl/mkl_dnn.h \
    include/mkl/mkl_poisson.h \
    include/mkl/mkl_rci.h \
    include/mkl/mkl_service.h \
    include/mkl/mkl_solvers_ee.h \
    include/mkl/mkl_trig_transforms.h \
    include/mkl/mkl_vml.h \
    include/mkl/mkl_vsl.h \
    utils/atcinterpolator.h \
    flight/atcabstractprofile.h \
    flight/atcprofileclimb.h \
    flight/atcprofiledescent.h \
    dialog/dialogflightlist.h \
    flight/atcpredictor.h \
    flight/atcpredictorcontroller.h \
    flight/atcprofilespeed.h

FORMS    += mainwindow.ui \
    atcdialog.ui \
    dialogmainmenu.ui \
    dialogsectorsetup.ui \
    dialogtextconsole.ui \
    dialogsettings.ui \
    dialogflight.ui \
    dialogflightcreator.ui \
    dialogaltitude.ui \
    dialogspeed.ui \
    dialogheading.ui \
    dialogroute.ui \
    dialoghandoff.ui \
    dialogflightplan.ui \
    dialogactiverunways.ui \
    dialogleaders.ui \
    ui/dialogtrailingdots.ui \
    ui/dialogflightlist.ui

DISTFILES += \
    resources/acft_cursor.png
