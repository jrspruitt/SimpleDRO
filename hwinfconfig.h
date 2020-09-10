#ifndef HWINFSETTINGS_H
#define HWINFSETTINGS_H

#include "drosettings.h"
#ifdef DESKTOP
#include "dummyinf.h"
#else
#include "hardwareinf.h"
#endif
#include "QAppWidgets/qappwindow.h"
#include "QAppWidgets/qapplist.h"
#include <QObject>
#include <QButtonGroup>

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
    void                                open(QString *portName, int *baudRate, QString *error);
    void                                exitWindow();
    QString                             *strError;

private:
    DROSettings                         *settings;
    HardwareInf                         *hwInf;
    QString                             *portName;
    int                                 *baudRate;
    QString                             *error;
    QAppList                            *listSerialPorts;
    QAppList                            *listBaudRates;
    QButtonGroup                        *bgrpEnabled;
    QButtonGroup                        *bgrpRevDirection;
    QButtonGroup                        *bgrpDiameterMode;

private slots:
    void                                handleAxisEnabled(int id);
    void                                handleAxisRevDirection(int id);
    void                                handleAxisDiameterMode(int id);
};

#endif // HWINFSETTINGS_H
