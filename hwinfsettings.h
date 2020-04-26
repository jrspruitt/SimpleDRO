#ifndef HWINFSETTINGS_H
#define HWINFSETTINGS_H

#include "drosettings.h"
#include "hardwareinf.h"
#include "QAppWidgets/qappwindow.h"
#include "QAppWidgets/qapplist.h"
#include <QObject>
#include <QCheckBox>

#define NOTSET                  "Not Set"
#define PORTNAME_NOTSET         NOTSET
#define PORTNAME_NOTSET_MSG     " Port name not set."

#define BAUDRATE_NOTSET         0
#define BAUDRATE_NOTSET_MSG     " Baudrate not set."

class HwInfSettings : public QAppWindow
{
    Q_OBJECT
public:
    HwInfSettings(DROSettings *settings, HardwareInf *hwInf, QWidget *parent = nullptr);
    using QAppWindow::open; // Turns off warning about overloading.
    QString                             open(QString *portName, int *baudRate, bool *enableUpdated);
    void                                exitWindow();
    QString                             *portName;
    int                                 *baudRate;
    bool                                *enableUpdated;

private:
    DROSettings                         *settings;
    HardwareInf                         *hwInf;
    QHash<QString, QCheckBox *>         *cbList;
    QAppList                            *listSerialPorts;
    QAppList                            *listBaudRates;
    QString                             *error;

private slots:
    void                        handleAxisChecked(bool checked);
};

#endif // HWINFSETTINGS_H
