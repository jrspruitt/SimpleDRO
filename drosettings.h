#ifndef SIMPLEDROSETTINGS_H
#define SIMPLEDROSETTINGS_H

#include <QObject>
#include "QAppWidgets/qappbasesettings.h"

#define UNITS_IMP               "in"
#define UNITS_SI                "mm"

#define SIMPLEDRO_CONFIG        "SimpleDRO"
#define XAXIS_NAME              "X"
#define YAXIS_NAME              "Y"
#define ZAXIS_NAME              "Z"

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
    void                    setAxisEnabled(QString axisName, bool enabled);
    bool                    getAxisEnabled(QString axisName);
    QStringList             axisNames();
    void                    setAxisRevDirection(QString axisName, bool isRev);
    bool                    getAxisRevDirection(QString axisName);
    void                    setAxisDiameterMode(QString axisName, bool isDiaMode);
    bool                    getAxisDiameterMode(QString axisName);
};

#endif // SIMPLEDROSETTINGS_H
