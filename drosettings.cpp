#include "drosettings.h"
#include <QDebug>

DROSettings::DROSettings(QString settingsPath)
    :QAppBaseSettings (settingsPath)
{
    activeIconPath = QDir::cleanPath(skinCurrentDirPath + QDir::separator() + "active.png");
    inactiveIconPath = QDir::cleanPath(skinCurrentDirPath + QDir::separator() + "inactive.png");
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
    QStringList ls;
    if ( getAxisEnabled(XAXIS_NAME) )
        ls.append(XAXIS_NAME);

    if ( getAxisEnabled(YAXIS_NAME) )
        ls.append(YAXIS_NAME);

    if ( getAxisEnabled(ZAXIS_NAME) )
        ls.append(ZAXIS_NAME);

    return ls;
}

QStringList DROSettings::axisAllNames()
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
    qDebug() << axisName << enabled << endl;
    setKeyValue(SIMPLEDRO_CONFIG, QString("%1Enabled").arg(axisName), enabled);
}

bool DROSettings::getAxisEnabled(QString axisName)
{
    return getValue(SIMPLEDRO_CONFIG, QString("%1Enabled").arg(axisName), false).toBool();
}
