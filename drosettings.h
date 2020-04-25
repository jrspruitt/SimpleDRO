#ifndef SIMPLEDROSETTINGS_H
#define SIMPLEDROSETTINGS_H

#include <QObject>
#include "QAppWidgets/qappbasesettings.h"

#define UNITS_IMP               "in"
#define UNITS_SI                "mm"

#define SIMPLEDRO_CONFIG        "SimpleDRO"
#define AXIS_CNT                3
#define XAXIS_NAME              "X"
#define YAXIS_NAME              "Y"
#define ZAXIS_NAME              "Z"

const QStringList axisNames = {XAXIS_NAME, YAXIS_NAME, ZAXIS_NAME};

class DROSettings : public QAppBaseSettings
{
    Q_OBJECT
public:
    DROSettings(QString settingsPath="");
    ~DROSettings();
    QString                 getHwInfSerialName();
    void                    setHwInfSerialName(QString name);
    int                     getHwInfSerialBaudRate();
    void                    setHwInfSerialBaudRate(int baudRate);

    QString                 activeIconPath;
    QString                 inactiveIconPath;
};

#endif // SIMPLEDROSETTINGS_H
