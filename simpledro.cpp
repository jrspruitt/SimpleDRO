#include "simpledro.h"
#include "drosettings.h"
#include "hwinfconfig.h"
#include "axis.h"
#include "DROWidgets/dronumkeypad.h"
#include "QAppWidgets/qappinfodialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLCDNumber>
#include <QLoggingCategory>
#include <QDebug>

SimpleDRO::SimpleDRO(QString skinName, QWidget *parent) :
    QMainWindow(parent)
{
    QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
    settings = new DROSettings(settingsPathCheck(""));
    settings->setSkin(skinName);

    setAttribute(Qt::WA_DeleteOnClose);
    setGeometry(QRect(0, 0, settings->screenWidth, settings->screenHeight));
    setFixedSize(settings->screenWidth, settings->screenHeight);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet(settings->getStyleSheet());

    axisReadouts = new QHash<QString, Axis *>;
    foreach ( const QString axisName, settings->axisNames() )
        axisReadouts->insert(axisName, new Axis(axisName));

    createUi();

    hwInf = new HardwareInf(settings);
    connect(hwInf, SIGNAL(axisUpdate(QString, bool, double, QString)), this, SLOT(updateDro(QString, bool, double, QString)));
    connect(hwInf, SIGNAL(stateChange(int)), this, SLOT(handleHwInfChange(int)));

    if ( hwInf->startHardware() )
        enableAxes();
}

SimpleDRO::~SimpleDRO()
{
}

void SimpleDRO::createUi()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    lblMessage = new QLabel("Welcome to Simple DRO.");
    mainLayout->addWidget(lblMessage);

    QHBoxLayout *droLayout = new QHBoxLayout();

    QVBoxLayout *readoutLayout = new QVBoxLayout();

    foreach ( const QString axisName, settings->axisNames() )
        readoutLayout->addWidget(axisReadouts->value(axisName)->axisReadout());

    QSpacerItem *readoutSpacer = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    readoutLayout->addItem(readoutSpacer);
    droLayout->addItem(readoutLayout);

    QSpacerItem *droSpacer = new QSpacerItem(10, 0, QSizePolicy::Maximum, QSizePolicy::Minimum);
    droLayout->addItem(droSpacer);

    DRONumKeypad *keypad = new DRONumKeypad();
    droLayout->addWidget(keypad);
    connect(keypad, SIGNAL(keyPressEnter(QString)), this, SLOT(handleKeyPressEnter(QString)));

    mainLayout->addItem(droLayout);
    QHBoxLayout *menuLayout = new QHBoxLayout();

    btnSiUnits = new QPushButton("Si Units");
    connect(btnSiUnits, SIGNAL(clicked()), this, SLOT(handleSiUnits()));
    menuLayout->addWidget(btnSiUnits);

    QPushButton *btnHwSettings = new QPushButton("Hardware Settings");
    connect(btnHwSettings, SIGNAL(clicked()), this, SLOT(handleHwConfig()));
    menuLayout->addWidget(btnHwSettings);

    QPushButton *btnClose = new QPushButton("Exit");
    connect(btnClose, SIGNAL(clicked()), this, SLOT(handleExit()));
    menuLayout->addWidget(btnClose);

    mainLayout->addItem(menuLayout);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void SimpleDRO::updateDro(QString name, bool on, double value, QString units)
{
    axisReadouts->value(name)->setDisabled(!on);

    if ( units.compare(UNITS_SI) == 0 && !isSiUnits )
        value = value / 25.4;
    else if ( units.compare(UNITS_IMP) == 0 && isSiUnits )
        value = 25.4 * value;

    axisReadouts->value(name)->setValue(value, isSiUnits);
}

void SimpleDRO::updateMessage(QString message )
{
    lblMessage->setText(message);
}

void SimpleDRO::enableAxes()
{
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( hwInf->waitToSend(500) ) {
            if ( settings->getAxisEnabled(axisName) ) {
                hwInf->sendData(QString("#%1").arg(axisName.toLower()));
                hwInf->waitForResp(500);

                if ( hwInf->respData().compare(RESP_SUCCESS) )
                    axisReadouts->value(axisName)->show();
                else
                    qDebug() << "Failed response enabling." << axisName << endl;
            } else {
                hwInf->sendData(QString("#%1").arg(axisName.toUpper()));
                hwInf->waitForResp(500);

                if ( hwInf->respData().compare(RESP_SUCCESS) )
                    axisReadouts->value(axisName)->hide();
                else
                    qDebug() << "Failed Response disabling" << axisName << endl;
            }
        }
    }
}

void SimpleDRO::handleHwConfig()
{
    QString *portName = new QString("");
    int *baudRate = new int(0);
    bool *enableUpdated = new bool(false);

    HwInfConfig *hwInfConfig = new HwInfConfig(settings, hwInf);
    QString error = hwInfConfig->open(portName, baudRate, enableUpdated);

    QAppInfoDialog *infoDialog = new QAppInfoDialog(settings);
    QString status = "";
    QString msg = "";

    if ( error.isEmpty() ) {
        if ( portName->compare(PORTNAME_NOTSET) != 0 && *baudRate != BAUDRATE_NOTSET ) {
            status = "Updated";
            if ( settings->getHwInfSerialName().compare(*portName) != 0 || settings->getHwInfSerialName().compare(*portName) != 0 ) {
                hwInf->stopHardware();
                settings->setHwInfSerialName(*portName);
                settings->setHwInfSerialBaudRate(*baudRate);
                hwInf->startHardware();
                msg = "Hardware configuration set.\n";
            }

            if ( *enableUpdated ) {
                enableAxes();
                msg += "Axes configuration updated.";
            }

        } else {
            status = "Warning";
            msg = "Hardware not configured.";
        }

    } else {
        status = "Error";
        msg = error;
    }

    if ( msg.compare("") != 0 )
        infoDialog->showWindow(msg, status, 2);


}

void SimpleDRO::handleSiUnits()
{
    isSiUnits = !isSiUnits;
    if ( isSiUnits )
        btnSiUnits->setText("Si Units");
    else
        btnSiUnits->setText("Imp Units");
}

void SimpleDRO::handleKeyPressEnter(QString value)
{
    foreach ( const QString name, settings->axisNames() ) {
        if ( axisReadouts->value(name)->getSelected() ) {
            axisReadouts->value(name)->setOffset(value.toDouble());
            axisReadouts->value(name)->setSelected(false);
        }
    }
}

void SimpleDRO::handleHwInfChange(int state)
{
    QString status = "";
    QString msg = "";

    switch ( state ) {
        case STATE_ERROR:
            foreach ( const QString axisName, settings->axisNames() )
                axisReadouts->value(axisName)->setDisabled(true);
            status = "Error";
            msg = hwInf->getError();
            break;

        case STATE_CONNECTING:
            status="Connecting";
            msg = "Attempting to connect to hardware.";
            break;

        case STATE_RUNNING:
            status = "Connected";
            msg = "Hardware is running.";
            while(! hwInf->isRunning() ) continue;
            enableAxes();
            break;

        case STATE_STOPPED:
            foreach ( const QString axisName, settings->axisNames() )
                axisReadouts->value(axisName)->setDisabled(true);
            status = "Stopped";
            msg = "Hardware has stopped running.";
            break;

        case STATE_WAITING:
            foreach ( const QString axisName, settings->axisNames() )
                axisReadouts->value(axisName)->setDisabled(true);
            status = "Waiting";
            msg = hwInf->getError();
            break;
    }

    QAppInfoDialog *infoDialog = new QAppInfoDialog(settings);
    infoDialog->showWindow(msg, status, 2);
}

Q_NORETURN
void SimpleDRO::handleExit()
{
    exit(0);
}
