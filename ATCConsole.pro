#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T19:35:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATCConsole
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    textconsolewindow.cpp \
    consolecommands.cpp \
    mainmenu.cpp \
    atcdialog.cpp \
    sectorsetup.cpp

HEADERS  += mainwindow.h \
    textconsolewindow.h \
    consolecommands.h \
    mainmenu.h \
    atcdialog.h \
    sectorsetup.h

FORMS    += mainwindow.ui \
    textconsolewindow.ui \
    mainmenu.ui \
    atcdialog.ui \
    sectorsetup.ui
