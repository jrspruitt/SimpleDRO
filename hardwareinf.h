#ifndef HARDWAREINF_H
#define HARDWAREINF_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include "drosettings.h"

class HardwareInf : public QThread
{
    Q_OBJECT

public:
    explicit                HardwareInf(DROSettings *settings, QObject *parent = nullptr);
    void                    startHardware(const QString port = "", int baudRate = 0);  // Start thread using given serial port name.
    void                    stopHardware();                     // Stop thread from running.
    QStringList             getInterfaces();
    void                    sendData(QString data);
    bool                    waitToSend = false;

private:
    void                    run() override;
    DROSettings             *settings;
    QSerialPort             *serial;
    int                     readTimeout = 50; //msec
    bool                    _stopHardware = false;
    QMutex                  m_mutex;
    QString                 m_txData = "";
    void                    processUpdate(QString data);

signals:
    void                    positionUpdate(QString name, bool on, double value, QString units); // Connect main app to this signal to get position data updates.
};

#endif // HARDWAREINF_H
