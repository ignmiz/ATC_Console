QT += core gui positioning testlib

CONFIG += c++11

VPATH += ./main \
    ./mock \
    ./suits \
    ../custom \
    ../dialog \
    ../display \
    ../main \
    ../nav \
    ../settings \
    ../ui \
    ../utils

INCLUDEPATH += ./main \
    ./mock \
    ./suits \
    ../custom \
    ../dialog \
    ../display \
    ../main \
    ../nav \
    ../settings \
    ../ui \
    ../utils

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATCConsoleTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

#MAIN PROJECT HEADERS & SOURCES & FORMS GO HERE:

SOURCES += \
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
    atcsettingstableview.cpp \

HEADERS  += \
    mainwindow.h \
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
    atcsettingstableview.h \

FORMS    += mainwindow.ui \
    atcdialog.ui \
    dialogmainmenu.ui \
    dialogsectorsetup.ui \
    dialogtextconsole.ui \
    dialogsettings.ui

#MOCK SOURCES & HEADERS GO HERE:

SOURCES += mock_atcabstractairway.cpp \
    mock_atcabstractbeacon.cpp \
    mock_atcabstractfix.cpp \
    mock_atcabstractprocedure.cpp

HEADERS += mock_atcabstractairway.h \
    mock_atcabstractbeacon.h \
    mock_atcabstractfix.h \
    mock_atcabstractprocedure.h

#TEST SOURCES & HEADERS GO HERE:

SOURCES += main.cpp \
    test_atcabstractairway.cpp \
    test_atcabstractbeacon.cpp \
    test_atcabstractfix.cpp \
    test_atcabstractprocedure.cpp \
    \
    test_atcairport.cpp \
    test_atcairspace.cpp \
    test_atcairspacefix.cpp \
    test_atcairspacesector.cpp \
    test_atcairwayhigh.cpp

HEADERS += \
    test_atcabstractairway.h \
    test_atcabstractbeacon.h \
    test_atcabstractfix.h \
    test_atcabstractprocedure.h \
    \
    test_atcairport.h \
    test_atcairspace.h \
    test_atcairspacefix.h \
    test_atcairspacesector.h \
    test_atcairwayhigh.h
