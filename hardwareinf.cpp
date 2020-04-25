#include "hardwareinf.h"
#include "drosettings.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMutexLocker>
#include <QtDebug>

HardwareInf::HardwareInf(DROSettings *settings, QObject *parent)
{
    this->settings = settings;
    serial = new QSerialPort;
}

void HardwareInf::startHardware(QString port, int baudRate) {
    _stopHardware = true;
    const QMutexLocker locker(&m_mutex);

    if ( port.compare("") == 0 )
        port = settings->getHwInfSerialName();

    if ( baudRate == 0 )
        baudRate = settings->getHwInfSerialBaudRate();

    QSerialPortInfo serialPort(port);
    serial->setPort(serialPort);
    serial->setBaudRate(baudRate);
    _stopHardware = false;
    if ( !isRunning() )
        start();
}

void HardwareInf::stopHardware() {
    _stopHardware = true;
}

QStringList HardwareInf::getInterfaces() {
    QStringList portNames;

    foreach (const QSerialPortInfo &inf, QSerialPortInfo::availablePorts())
        portNames.append(inf.portName());

    return portNames;
}

void HardwareInf::sendData(QString data){
    m_txData = data;
}

void HardwareInf::processUpdate(QString data) {
    qDebug() << data << endl;
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
    for ( QString aName: axisNames ) {
        if ( name.compare(aName) == 0 ){
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

void HardwareInf::run() {
    QString rawData = "";

    // Open serial port for reading.
    if ( !serial->open(QIODevice::ReadWrite) ) {
         qDebug() << "Serial Port Error:" << serial->error()  << endl;
         _stopHardware = true;
    }

    // Serial port read loop, emit signal when data read complete.
    m_txData = "";
    while( !_stopHardware ) {
        m_mutex.lock();
        if ( m_txData.length() != 0 ) {
            serial->write(m_txData.toUtf8());
            serial->waitForBytesWritten(100);
            m_txData = "";
        }
        m_mutex.unlock();
        if ( serial->waitForReadyRead(readTimeout) ) {
            rawData = serial->readAll();
            QStringList axisData(rawData.split("|"));

            foreach ( QString data, axisData ) {
                //qDebug() << data << endl;
                 if ( data.trimmed().isEmpty() )
                        continue;

                if ( data.compare("NO") != 0 && data.compare("OK") != 0) {
                    processUpdate(data);

                }
            }
            serial->flush();
        }
    }

    serial->close();
}
