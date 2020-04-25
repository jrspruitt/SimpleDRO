#include "drosettings.h"

DROSettings::DROSettings(QString settingsPath)
    :QAppBaseSettings (settingsPath)
{
    activeIconPath = QDir::cleanPath(skinCurrentDirPath + QDir::separator() + "active.png");
    inactiveIconPath = QDir::cleanPath(skinCurrentDirPath + QDir::separator() + "inactive.png");
}
DROSettings::~DROSettings(){

}
void DROSettings::setHwInfSerialName(QString name) {
    setKeyValue(SIMPLEDRO_CONFIG, "hwInfSerialName", name);
}

QString DROSettings::getHwInfSerialName() {
    return getValue(SIMPLEDRO_CONFIG, "hwInfSerialName", "").toString();
}

void DROSettings::setHwInfSerialBaudRate(int baudRate) {
    setKeyValue(SIMPLEDRO_CONFIG, "hwInfBaudRate", baudRate);
}

int DROSettings::getHwInfSerialBaudRate() {
    return getValue(SIMPLEDRO_CONFIG, "hwInfBaudRate", 115200).toInt();
}
