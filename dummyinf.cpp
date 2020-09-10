#include "dummyinf.h"
#include "drosettings.h"
#include <QObject>
#include <QMutexLocker>
#include <QDebug>

HardwareInf::HardwareInf(DROSettings *settings, QObject *parent)
{
    this->settings = settings;
    _error = "";
    _curState = STATE_STOPPED;
}

QString HardwareInf::getError()
{
    QString err = _error;
    _error = "";
    return err;
}

int HardwareInf::getState()
{
    return _curState;
}

bool HardwareInf::startHardware()
{
    _stopHardware = false;
    const QMutexLocker locker(&m_mutex);

    try {
        handleStateChange(STATE_CONNECTING);
        qDebug() << "Starting" << endl;
        qDebug() << _curState << endl;
        start();
        return true;

    } catch (...) {
        _error = "Could not config interface.";
        return false;
    }
}

void HardwareInf::stopHardware()
{
    _stopHardware = true;
}

QStringList HardwareInf::getInterfaces()
{
    QStringList portNames;
    portNames.append(settings->getHwInfSerialName());
    return portNames;
}

bool HardwareInf::isInfAvailable(QString portName)
{
    return true;
}

void HardwareInf::sendData(QString data)
{}

bool HardwareInf::waitToSend(int ms)
{
    return false;
}

bool HardwareInf::waitForResp(int ms)
{
    return true;
}

QString HardwareInf::respData()
{
    m_mutex.lock();
    QString resp = m_respData;
    m_mutex.unlock();
    return resp;
}

void HardwareInf::run()
{
    QString rawData = "";

    while( !_stopHardware ) {
        switch ( _curState ) {
        case STATE_CONNECTING:
            qDebug() << "CONNECTING" << endl;
            handleStateChange(STATE_RUNNING);
            break;

        case STATE_RUNNING:
        {
            QFile inpFile("input.debug");
            if ( ! inpFile.open(QIODevice::ReadOnly) ) {
                qDebug() << "Failed to open file." << endl;
                //handleStateChange(STATE_STOPPED);
            }

            QTextStream inpStream(&inpFile);
            QString rawData = inpStream.readLine();

            QStringList axisData(rawData.split("|"));

            foreach ( const QString data, axisData ) {
                handleAxisUpdate(data);
                }
            }
            break;

        case STATE_WAITING:
            startHardware();

        }// switch
    }// while
    handleStateChange(STATE_STOPPED);

}

void HardwareInf::handleAxisUpdate(QString data)
{
    bool nameOk = false;
    QString name = "";
    bool on = false;
    bool toDoubleOk;
    double value = 0;
    QString units = "";
    QStringList axisData(data.split(","));

    if (axisData.length() != 4 )
        return;

    name = axisData.at(0);
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( name.compare(axisName) == 0 ){
            nameOk = true;
            break;
        }
    }

    if ( !nameOk )
        return;

    if ( axisData.at(1).compare("on") == 0 || axisData.at(1).compare("off") == 0 )
        on = axisData.at(1).compare("on") == 0 ? true : false;
    else
        return;

    value = axisData.at(2).toDouble(&toDoubleOk);

    if ( !toDoubleOk )
        return;

    if ( axisData.at(3).compare("mm") == 0 || axisData.at(3).compare("in") == 0 )
        units = axisData.at(3);
    else
        return;

    emit axisUpdate(name, on, value, units);
}

void HardwareInf::handleStateChange(int state)
{
    _curState = state;
    emit stateChange(_curState);
}
