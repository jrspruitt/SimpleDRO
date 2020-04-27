#include "hardwareinf.h"
#include "drosettings.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMutexLocker>
#include <QTimer>
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

    foreach (const QSerialPortInfo &inf, QSerialPortInfo::availablePorts())
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

void HardwareInf::processUpdate(QString data)
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

    emit positionUpdate(name, on, value, units);
}

void HardwareInf::run()
{
    QString rawData = "";
    while( !_stopHardware ) {
        switch ( _curState ) {
        case STATE_CONNECTING:
            if ( serial->open(QIODevice::ReadWrite) )
                handleStateChange(STATE_RUNNING);
            else
                _error = serial->errorString();
            break;

        case STATE_RUNNING:

            if ( m_txData.length() != 0 ) {
                m_mutex.lock();
                serial->write(m_txData.toUtf8());
                serial->waitForBytesWritten(100);
                m_txData = "";
                _waitToSend = false;
                m_mutex.unlock();
            }

            if ( serial->waitForReadyRead(readTimeout) ) {
                rawData = serial->readAll();
                QStringList axisData(rawData.split("|"));

                foreach ( const QString data, axisData ) {
                    if ( data.trimmed().isEmpty() )
                            continue;

                    if ( data.compare("NO") != 0 && data.compare("OK") != 0)
                        processUpdate(data);
                }
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
            serial->flush();

            break;

        case STATE_WAITING:
            if ( isInfAvailable(settings->getHwInfSerialName()))
                startHardware();

        }// switch
    }// while
    handleStateChange(STATE_STOPPED);

}

void HardwareInf::handleStateChange(int state)
{
    _curState = state;
    emit stateChange(_curState);
}
