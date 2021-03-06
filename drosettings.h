#ifndef SIMPLEDROSETTINGS_H
#define SIMPLEDROSETTINGS_H

#include <QObject>
#include "QAppWidgets/qappbasesettings.h"

#define UNITS_SI                "mm"
#define UNITS_IMP               "in"

#define DROFUNC_OFFSETS         "DROFunc_Offsets"
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
    void                    setUiUnits(bool isSi);
    bool                    getUiUnits();
    void                    setHwInfSerialName(QString name);
    QString                 getHwInfSerialName();
    void                    setHwInfSerialBaudRate(int baudRate);
    int                     getHwInfSerialBaudRate();
    void                    setAxisEnabled(QString axisName, bool enabled);
    bool                    getAxisEnabled(QString axisName);
    QStringList             axisNames();
    void                    setAxisRevDirection(QString axisName, bool isRev);
    bool                    getAxisRevDirection(QString axisName);
    void                    setAxisDiameterMode(QString axisName, bool isDiaMode);
    bool                    getAxisDiameterMode(QString axisName);
    void                    setAxisOffset(QString axisName, int offsetNum, double offset);
    double                  getAxisOffset(QString axisName, int offsetNum);
};

#endif // SIMPLEDROSETTINGS_H

