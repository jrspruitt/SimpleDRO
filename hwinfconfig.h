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

class HwInfConfig : public QAppWindow
{
    Q_OBJECT
public:
    HwInfConfig(DROSettings *settings, HardwareInf *hwInf, QWidget *parent = nullptr);
    using QAppWindow::open; // Turns off warning about overloading.
    QString                             open(QString *portName, int *baudRate, bool *enableUpdated);
    void                                exitWindow();
    QString                             *portName;
    int                                 *baudRate;
    bool                                *enableUpdated;
    QString                             *strError;

private:
    DROSettings                         *settings;
    HardwareInf                         *hwInf;
    QAppList                            *listSerialPorts;
    QAppList                            *listBaudRates;

private slots:
    void                                handleAxisEnabled();
};

#endif // HWINFSETTINGS_H
