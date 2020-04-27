#include "hwinfconfig.h"
#include "drosettings.h"
#include "hardwareinf.h"
#include "QAppWidgets/qapplist.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>

HwInfConfig::HwInfConfig(DROSettings *settings, HardwareInf *hwInf, QWidget *parent)
    :QAppWindow(parent, settings)
{
    this->settings = settings;
    this->hwInf = hwInf;
    this->strError = new QString("");
}

void HwInfConfig::exitWindow() {
    if ( listSerialPorts->currentRow() > 0 ) {
        *portName = listSerialPorts->currentItem()->text();

    } else {
        *portName = PORTNAME_NOTSET;
        *strError = QString(PORTNAME_NOTSET_MSG);
    }

    if ( listBaudRates->currentRow() > 0 ) {
        *baudRate = listBaudRates->currentItem()->text().toInt();

    } else {
        *baudRate = BAUDRATE_NOTSET;
        *strError += QString(BAUDRATE_NOTSET_MSG);
    }

    hide();
    close();
}

QString HwInfConfig::open(QString *portName, int *baudRate, bool *enableUpdated) {
    this->portName = portName;
    this->baudRate = baudRate;
    this->enableUpdated = enableUpdated;
    *this->enableUpdated = false;

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *enableLayout = new QVBoxLayout();
    QLabel *lblEnableSection = new QLabel("Enable Axes");
    enableLayout->addWidget(lblEnableSection);

    foreach( const QString axisName, settings->axisAllNames() ) {
        QString btnText = settings->getAxisEnabled(axisName) ? "Enabled" : "Disabled";
        QPushButton *btnAxis = new QPushButton(QString("%1 %2").arg(axisName).arg(btnText));
        connect(btnAxis, SIGNAL(clicked()), this, SLOT(handleAxisEnabled()));
        enableLayout->addWidget(btnAxis);
    }

    mainLayout->addItem(enableLayout);

    QSpacerItem *mainLeftSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addItem(mainLeftSpacer);

    QVBoxLayout *listsLayout = new QVBoxLayout();
    QSpacerItem *listsTopSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    listsLayout->addItem(listsTopSpacer);

    QVBoxLayout *portNamesLayout = new QVBoxLayout();

    QLabel *lblSerialPort = new QLabel("Serial Port");
    portNamesLayout->addWidget(lblSerialPort);

    listSerialPorts = new QAppList(settings);
    QStringList portList = hwInf->getInterfaces();
    listSerialPorts->addItem(NOTSET);
    listSerialPorts->setCurrentRow(0);

    for ( int i = 0; i < portList.length(); i++) {
        listSerialPorts->addItem(portList[i]);

        if ( portList[i].compare(settings->getHwInfSerialName()) == 0 )
            listSerialPorts->setCurrentRow(i+1);
    }

    portNamesLayout->addWidget(listSerialPorts);
    listsLayout->addItem(portNamesLayout);

    QSpacerItem *listsMidSpacer = new QSpacerItem(5, 50, QSizePolicy::Expanding, QSizePolicy::Expanding);
    listsLayout->addItem(listsMidSpacer);

    QVBoxLayout *baudRatesLayout = new QVBoxLayout();
    QLabel *lblbaudRates = new QLabel("Baud Rates");
    baudRatesLayout->addWidget(lblbaudRates);

    listBaudRates = new QAppList(settings);
    listBaudRates->addItem(NOTSET);
    listBaudRates->setCurrentRow(0);

    QList<int> baudRates = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 46800, 500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000};
    for ( int i = 0; i < baudRates.length(); i++ ) {
        listBaudRates->addItem(QString::number(baudRates[i]));

        if ( baudRates[i] == settings->getHwInfSerialBaudRate() )
            listBaudRates->setCurrentRow(i+1);
    }

    baudRatesLayout->addWidget(listBaudRates);
    listsLayout->addItem(baudRatesLayout);

    QSpacerItem *listsBotSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    listsLayout->addItem(listsBotSpacer);

    mainLayout->addItem(listsLayout);
    QSpacerItem *mainRightSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addItem(mainRightSpacer);
    showWindow(mainLayout, "Hardware Settings");
    exec();

    return *strError;
}

void HwInfConfig::handleAxisEnabled()
{
    QPushButton *btn = dynamic_cast< QPushButton *>(QObject::sender());

    foreach ( const QString axisName, settings->axisAllNames() ){
        QString btnEn = QString("%1 %2").arg(axisName).arg("Enabled");
        QString btnDis = QString("%1 %2").arg(axisName).arg("Disabled");

        if ( btn->text().compare(btnEn) == 0 ) {
            btn->setText(btnDis);
            settings->setAxisEnabled(axisName, false);
            *enableUpdated = true;
            break;

        } else if ( btn->text().compare(btnDis) == 0 ) {
            btn->setText(btnEn);
            settings->setAxisEnabled(axisName, true);
            *enableUpdated = true;
            break;
        }

    }
}
