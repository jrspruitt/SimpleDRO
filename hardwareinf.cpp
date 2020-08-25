#include "hardwareinf.h"
#include "drosettings.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMutexLocker>
#include <QDebug>

HardwareInf::HardwareInf(DROSettings *settings, QObject *parent)
{
    this->settings = settings;
    serial = new QSerialPort;
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
    _stopHardware = true;
    const QMutexLocker locker(&m_mutex);
    QString portName = settings->getHwInfSerialName();
    int baudRate = settings->getHwInfSerialBaudRate();

    if ( portName.compare("") == 0 || baudRate == 0 ) {
        _error = "Interface not configured properly.";
        handleStateChange(STATE_ERROR);
        return false;
    }

    if ( !isInfAvailable(portName) ) {
        _error = "Hardware not available.\n Please plug in device.";
        _stopHardware = false;
        handleStateChange(STATE_WAITING);
        start();
        return false;
    }

    try {
        QSerialPortInfo serialPort(portName);
        serial->setPort(serialPort);
        serial->setBaudRate(baudRate);
        _stopHardware = false;
        handleStateChange(STATE_CONNECTING);
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

    foreach ( const QSerialPortInfo &inf, QSerialPortInfo::availablePorts() )
        portNames.append(inf.portName());

    return portNames;
}

bool HardwareInf::isInfAvailable(QString portName)
{
    QStringList infList = getInterfaces();
    for ( int i = 0; i < infList.length(); i++ ) {
        if ( portName.compare(infList[i]) == 0 )
            return true;
    }
    return false;
}

void HardwareInf::sendData(QString data)
{
    if ( isRunning() ) {
        m_mutex.lock();
        _waitToSend = true;
        m_txData = data;
        m_mutex.unlock();
    }
}

bool HardwareInf::waitToSend(int ms)
{
    if ( !_waitToSend )
        return true;

    struct timespec ts = {ms / 1000, (ms % 1000) * 1000 * 1000};
    nanosleep(&ts, nullptr);

    if ( !_waitToSend )
        return true;

    return false;
}

bool HardwareInf::waitForResp(int ms)
{
    if ( !_waitForResp )
        return true;

    struct timespec ts = {ms / 1000, (ms % 1000) * 1000 * 1000};
    nanosleep(&ts, nullptr);

    if ( !_waitForResp )
        return true;

    return false;
}

QString HardwareInf::respData()
{
    m_mutex.lock();
    QString resp = m_respData;
    m_mutex.unlock();
    _waitForResp = false;
    return resp;
}

void HardwareInf::run()
{
    QString rawData = "";

    while( !_stopHardware ) {
        switch ( _curState ) {
        case STATE_CONNECTING:
            if ( ! serial->open(QIODevice::ReadWrite) )
                handleStateChange(STATE_RUNNING);
            else
                _error = serial->errorString();
            break;

        case STATE_RUNNING:
            if ( m_txData.length() != 0 ) {
                m_mutex.lock();
                serial->write(m_txData.toUtf8());
                serial->waitForBytesWritten(writeTimeout);
                m_txData = "";
                _waitToSend = false;
                _waitForResp = true;
                m_mutex.unlock();
            }

            if ( serial->waitForReadyRead(readTimeout) ) {
                rawData = serial->readAll();
                QStringList axisData(rawData.split("|"));

                foreach ( const QString data, axisData ) {
                    if ( data.trimmed().isEmpty() ) {
                            continue;

                    } else if ( data.compare(RESP_FAIL) == 0 ) {
                        m_mutex.lock();
                        m_respData = false;
                        _waitForResp = false;
                        m_mutex.unlock();

                    } else if ( data.compare(RESP_SUCCESS) == 0 ) {
                        m_mutex.lock();
                        m_respData = true;
                        _waitForResp = false;
                        m_mutex.unlock();

                    } else {
                        handleAxisUpdate(data);
                    }
                }
                serial->flush();

            } else {
                switch (serial->error() ) {
                    case QSerialPort::TimeoutError:
                        break;
                    case QSerialPort::ReadError:
                        _error = "Hardware not available.\n Please plug in device.";
                        serial->close();
                        handleStateChange(STATE_WAITING);
                        break;
                    default:
                        handleStateChange(STATE_ERROR);
                }
            }

            break;

        case STATE_WAITING:
            if ( isInfAvailable(settings->getHwInfSerialName()))
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
