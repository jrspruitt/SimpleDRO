#include "hwinfsettings.h"
#include "drosettings.h"
#include "hardwareinf.h"
#include "QAppWidgets/qapplist.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>

HwInfSettings::HwInfSettings(DROSettings *settings, HardwareInf *hwInf, QWidget *parent)
    :QAppWindow(parent, settings)
{
    this->settings = settings;
    this->hwInf = hwInf;
    error = new QString("");
}

void HwInfSettings::exitWindow() {
    if ( listSerialPorts->currentRow() > 0 ) {
        *portName = listSerialPorts->currentItem()->text();

    } else {
        *portName = PORTNAME_NOTSET;
        *error = PORTNAME_NOTSET_MSG;
    }

    if ( listBaudRates->currentRow() > 0 ) {
        *baudRate = listBaudRates->currentItem()->text().toInt();

    } else {
        *baudRate = BAUDRATE_NOTSET;
        *error += BAUDRATE_NOTSET_MSG;
    }

    hide();
    close();
}

QString HwInfSettings::open(QString *portName, int *baudRate, bool *enableUpdated) {
    this->portName = portName;
    this->baudRate = baudRate;
    this->enableUpdated = enableUpdated;
    *this->enableUpdated = false;

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *cbLayout = new QVBoxLayout();

    cbList = new QHash<QString, QCheckBox *>;
    foreach( const QString axisName, settings->axisNames() ) {
        QCheckBox *cbAxis = new QCheckBox(axisName);
        cbList->insert(axisName, cbAxis);
        connect(cbAxis, SIGNAL(clicked(bool)), this, SLOT(handleAxisChecked(bool)));
        cbLayout->addWidget(cbAxis);
    }

    mainLayout->addItem(cbLayout);

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
    return *error;
}

void HwInfSettings::handleAxisChecked(bool checked)
{
    QCheckBox *cbTmp = dynamic_cast< QCheckBox *>(QObject::sender());

    if ( *enableUpdated != true )
        *enableUpdated = false;

    foreach ( const QString axisName, settings->axisNames() ){
        if ( cbList->value(axisName) == cbTmp ) {
            *enableUpdated = true;
            settings->setAxisEnabled(axisName, checked);
        }
    }
}
