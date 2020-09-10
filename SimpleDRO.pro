#-------------------------------------------------
#
# Project created by QtCreator 2020-04-25T11:45:46
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleDRO
TEMPLATE = app

SOURCES += \
        main.cpp \
        simpledro.cpp \
        axis.cpp \
        drosettings.cpp \
        dronumkeypad.cpp \
        hwinfconfig.cpp \
        drofunctions.cpp \
        QAppWidgets/QAppKeyboard/qappkeyboard.cpp \
        QAppWidgets/QAppKeyboard/qapplineedit.cpp \
        QAppWidgets/qappbasesettings.cpp \
        QAppWidgets/qappbasesettingseditor.cpp \
        QAppWidgets/qappfiledialog.cpp \
        QAppWidgets/qappinfodialog.cpp \
        QAppWidgets/qapplist.cpp \
        QAppWidgets/qappwindow.cpp \


HEADERS += \
        simpledro.h \
        axis.h \
        drosettings.h \
        dronumkeypad.h \
        hwinfconfig.h \
        drofunctions.h \
        QAppWidgets/QAppKeyboard/qappkeyboard.h \
        QAppWidgets/QAppKeyboard/qapplineedit.h \
        QAppWidgets/qappbasesettings.h \
        QAppWidgets/qappbasesettingseditor.h \
        QAppWidgets/qappfiledialog.h \
        QAppWidgets/qappinfodialog.h \
        QAppWidgets/qapplist.h \
        QAppWidgets/qappwindow.h \

DESKTOP {
DEFINES += DESKTOP
SOURCES += dummyinf.cpp
HEADERS += dummyinf.h
}

MINI210 {
DEFINES += MINI210
SOURCES += hardwareinf.cpp
HEADERS += hardwareinf.h
target.path = /root
INSTALLS += target
}
