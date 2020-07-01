#ifndef SIMPLEDRO_H
#define SIMPLEDRO_H

#include "hardwareinf.h"
#include "axis.h"
#include "drosettings.h"
#include "drofunctions.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class SimpleDRO : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleDRO(QString skinName = "default", QWidget *parent = nullptr);
    ~SimpleDRO();
    HardwareInf                         *hwInf;
    DROSettings                         *settings;
    QLabel                              *lblMessage;

private:
    void                                createUi();
    DROFunctions                        *funcKeypad;
    QHash<QString, Axis *>              *axisReadouts;
    QPushButton                         *btnSiUnits;
    bool                                isSiUnits = true;
    void                                enableHwInfAxes();
    void                                configureAxes();

public slots:
    void                                updateDro(QString name, bool on, double value, QString units);
    void                                updateMessage(QString message);

private slots:
    void                                handleHwConfig();
    void                                handleHwInfChange(int state);
    void                                handleSiUnits();
    void                                handleKeyPressEnter(QString value);
    void                                handleAxisSelect(QString axisName);
    void                                handleExit();
};

#endif // SIMPLEDRO_H
