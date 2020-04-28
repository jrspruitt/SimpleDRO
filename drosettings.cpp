#include "drosettings.h"

DROSettings::DROSettings(QString settingsPath)
    :QAppBaseSettings (settingsPath)
{
}

DROSettings::~DROSettings()
{

}

void DROSettings::setHwInfSerialName(QString name)
{
    setKeyValue(SIMPLEDRO_CONFIG, "hwInfSerialName", name);
}

QStringList DROSettings::axisNames()
{
    return QStringList({XAXIS_NAME, YAXIS_NAME, ZAXIS_NAME});
}

QString DROSettings::getHwInfSerialName()
{
    return getValue(SIMPLEDRO_CONFIG, "hwInfSerialName", "").toString();
}

void DROSettings::setHwInfSerialBaudRate(int baudRate)
{
    setKeyValue(SIMPLEDRO_CONFIG, "hwInfBaudRate", baudRate);
}

int DROSettings::getHwInfSerialBaudRate()
{
    return getValue(SIMPLEDRO_CONFIG, "hwInfBaudRate", 115200).toInt();
}

void DROSettings::setAxisEnabled(QString axisName, bool enabled)
{
    setKeyValue(SIMPLEDRO_CONFIG, QString("%1Enabled").arg(axisName), enabled);
}

bool DROSettings::getAxisEnabled(QString axisName)
{
    return getValue(SIMPLEDRO_CONFIG, QString("%1Enabled").arg(axisName), false).toBool();
}
