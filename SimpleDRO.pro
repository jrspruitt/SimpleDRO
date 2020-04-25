#-------------------------------------------------
#
# Project created by QtCreator 2020-04-12T22:22:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleDRO
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        simpledro.cpp \
        hardwareinf.cpp \
        hwinfsettings.cpp \
        axis.cpp \
        QAppWidgets/QAppKeyboard/qappkeyboard.cpp \
        QAppWidgets/QAppKeyboard/qapplineedit.cpp \
        QAppWidgets/qappbasesettings.cpp \
        QAppWidgets/qappbasesettingseditor.cpp \
        QAppWidgets/qappfiledialog.cpp \
        QAppWidgets/qappinfodialog.cpp \
        QAppWidgets/qapplist.cpp \
        QAppWidgets/qappwindow.cpp \
    drosettings.cpp \
    DROWidgets/dronumkeypad.cpp

HEADERS += \
        simpledro.h \
        hardwareinf.h \
        hwinfsettings.h \
        axis.h \
        QAppWidgets/QAppKeyboard/qappkeyboard.h \
        QAppWidgets/QAppKeyboard/qapplineedit.h \
        QAppWidgets/qappbasesettings.h \
        QAppWidgets/qappbasesettingseditor.h \
        QAppWidgets/qappfiledialog.h \
        QAppWidgets/qappinfodialog.h \
        QAppWidgets/qapplist.h \
        QAppWidgets/qappwindow.h \
    drosettings.h \
    DROWidgets/dronumkeypad.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
