#include "simpledro.h"
#include "drosettings.h"
#include "hwinfsettings.h"
#include "axis.h"
#include "DROWidgets/dronumkeypad.h"
#include "QAppWidgets/qappinfodialog.h"

#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLCDNumber>
#include <QDebug>

SimpleDRO::SimpleDRO(QString skinName, QWidget *parent) :
    QMainWindow(parent)
{
    settings = new DROSettings(settingsPathCheck(""));
    settings->setSkin(skinName);

    setAttribute(Qt::WA_DeleteOnClose);
    setGeometry(QRect(0, 0, settings->screenWidth, settings->screenHeight));
    setFixedSize(settings->screenWidth, settings->screenHeight);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet(settings->getStyleSheet());

    hwInf = new HardwareInf(settings);
    if ( settings->getHwInfSerialName().compare("") != 0 && settings->getHwInfSerialBaudRate() != 0 )
        hwInf->startHardware();

    connect(hwInf, SIGNAL(positionUpdate(QString, bool, double, QString)), this, SLOT(updateDro(QString, bool, double, QString)));
/*
    axisList = new QHash<QString, Axis *>;

    for ( int i = 0; i < axesNames.length(); i++ )
        axisList->insert(axesNames.at(i), new Axis(axesNames[i], hwInf, settings));
*/
    axisReadouts = new QHash<QString, Axis *>;
    foreach (const QString axisName, axisNames)
        axisReadouts->insert(axisName, new Axis(axisName));

    createUi();
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

    foreach ( const QString axisName, axisNames ) {
        readoutLayout->addWidget(axisReadouts->value(axisName)->axisReadout());
    }
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
    connect(btnHwSettings, SIGNAL(clicked()), this, SLOT(handleHwSettings()));
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
    if ( !on )
        return;

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

void SimpleDRO::handleHwSettings()
{
    QString *portName = new QString("");
    int *baudRate = new int(0);

    HwInfSettings *hwInfSettings = new HwInfSettings(settings, hwInf);
    QString error = hwInfSettings->open(portName, baudRate);
    qDebug() << error << error.compare("") << endl;

    if ( portName->compare(PORTNAME_NOTSET) != 0 && *baudRate != BAUDRATE_NOTSET && error.compare("") == 0 ) {
        settings->setHwInfSerialName(*portName);
        settings->setHwInfSerialBaudRate(*baudRate);
        hwInf->startHardware();

    } else {
        QAppInfoDialog *infoDialog = new QAppInfoDialog(settings);
        infoDialog->showWindow(error, "Error", 2);
    }
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
    qDebug() << "#######################" << endl;
    foreach ( const QString name, axisNames ) {
        if ( axisReadouts->value(name)->getSelected() ) {
            axisReadouts->value(name)->setOffset(value.toDouble());
            axisReadouts->value(name)->setSelected(false);
        }
    }
}

Q_NORETURN
void SimpleDRO::handleExit()
{
    exit(0);
}
