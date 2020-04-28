#ifndef HARDWAREINF_H
#define HARDWAREINF_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include "drosettings.h"

#define STATE_STOPPED           0
#define STATE_CONNECTING        1
#define STATE_RUNNING           2
#define STATE_ERROR             3
#define STATE_WAITING           4

#define RESP_SUCCESS            "OK"
#define RESP_FAIL               "NO"

class HardwareInf : public QThread
{
    Q_OBJECT

public:
    explicit                HardwareInf(DROSettings *settings, QObject *parent = nullptr);
    bool                    startHardware();  // Start thread using given serial port name.
    void                    stopHardware();                     // Stop thread from running.
    QStringList             getInterfaces();
    bool                    isInfAvailable(QString portName);
    void                    sendData(QString data);
    QString                 respData();
    bool                    waitToSend(int ms);
    bool                    waitForResp(int ms);
    QString                 getError();
    int                     getState();

private:
    void                    run() override;
    DROSettings             *settings;
    QSerialPort             *serial;
    int                     readTimeout = 50; //msec
    int                     writeTimeout = 100;
    QMutex                  m_mutex;
    QString                 m_txData = "";
    QString                 m_respData = "";
    QSerialPortInfo         *portInfo;
    QString                 _error = "";
    bool                    _stopHardware = false;
    bool                    _waitToSend = false;
    bool                    _waitForResp = false;
    int                     _curState = 0;
    void                    handleStateChange(int state);
    void                    handleAxisUpdate(QString data);

signals:
    void                    axisUpdate(QString name, bool on, double value, QString units); // Connect main app to this signal to get position data updates.
    void                    stateChange(int state);

};

#endif // HARDWAREINF_H
