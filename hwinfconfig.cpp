#include "hwinfconfig.h"
#include "drosettings.h"
#include "hardwareinf.h"
#include "QAppWidgets/qapplist.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QSerialPortInfo>
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
        settings->setHwInfSerialName(PORTNAME_NOTSET);
        *error = QString("%1\n").arg(PORTNAME_NOTSET_MSG);
    }

    if ( listBaudRates->currentRow() > 0 ) {
        *baudRate = listBaudRates->currentItem()->text().toInt();

    } else {
        settings->setHwInfSerialBaudRate(BAUDRATE_NOTSET);
        *error += QString("%1").arg(BAUDRATE_NOTSET_MSG);
    }

    hide();
    close();
}

void HwInfConfig::open(QString *portName, int *baudRate, QString *error) {
    this->portName = portName;
    this->baudRate = baudRate;
    this->error = error;

    bgrpEnabled = new QButtonGroup();
    bgrpRevDirection = new QButtonGroup();
    bgrpDiameterMode = new QButtonGroup();

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *axisSettingsLayout = new QVBoxLayout();
    QLabel *lblEnableSection = new QLabel("Enable Axes");
    lblEnableSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    axisSettingsLayout->addWidget(lblEnableSection);

    foreach( const QString axisName, settings->axisNames() ) {
        int btnNum = settings->axisNames().indexOf(axisName);
        QVBoxLayout *axisLayout = new QVBoxLayout();
        axisLayout->setMargin(0);

        QLabel *lblName = new QLabel(QString("%1 Axis").arg(axisName));
        axisLayout->addWidget(lblName);

        QHBoxLayout *enableLayout = new QHBoxLayout();
        QLabel *lblEnable = new QLabel("Axis Active: ");
        enableLayout->addWidget(lblEnable);

        QString strAxisEnable = settings->getAxisEnabled(axisName) ? "Enabled" : "Disabled";
        QPushButton *btnAxisEnable = new QPushButton(strAxisEnable);
        bgrpEnabled->addButton(btnAxisEnable, btnNum);
        btnAxisEnable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btnAxisEnable->setMaximumWidth(150);
        enableLayout->addWidget(btnAxisEnable);
        axisLayout->addItem(enableLayout);

        QHBoxLayout *revLayout = new QHBoxLayout();
        QLabel *lblRev = new QLabel("Reverse Readout: ");
        revLayout->addWidget(lblRev);

        QString strAxisRevDir = settings->getAxisRevDirection(axisName) ? "Enabled" : "Disabled";
        QPushButton *btnAxisRevDir = new QPushButton(strAxisRevDir);
        bgrpRevDirection->addButton(btnAxisRevDir, btnNum);
        btnAxisRevDir->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btnAxisRevDir->setMaximumWidth(150);
        revLayout->addWidget(btnAxisRevDir);
        axisLayout->addItem(revLayout);

        QHBoxLayout *diaLayout = new QHBoxLayout();
        QLabel *lblDia = new QLabel("Diameter Mode: ");
        diaLayout->addWidget(lblDia);

        QString strAxisDiaMode = settings->getAxisDiameterMode(axisName) ? "Enabled" : "Disabled";
        QPushButton *btnAxisDiaMode = new QPushButton(strAxisDiaMode);
        bgrpDiameterMode->addButton(btnAxisDiaMode, btnNum);
        btnAxisDiaMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btnAxisDiaMode->setMaximumWidth(150);
        diaLayout->addWidget(btnAxisDiaMode);
        axisLayout->addItem(diaLayout);

        QGroupBox *gbAxis = new QGroupBox();
        gbAxis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        gbAxis->setLayout(axisLayout);
        axisSettingsLayout->addWidget(gbAxis);
    }

    mainLayout->addItem(axisSettingsLayout);

    connect(bgrpRevDirection, SIGNAL(buttonClicked(int)), this, SLOT(handleAxisRevDirection(int)));
    connect(bgrpEnabled, SIGNAL(buttonClicked(int)), this, SLOT(handleAxisEnabled(int)));
    connect(bgrpDiameterMode, SIGNAL(buttonClicked(int)), this, SLOT(handleAxisDiameterMode(int)));

    QSpacerItem *mainLeftSpacer = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
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

    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates();
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
    QSpacerItem *mainRightSpacer = new QSpacerItem(5, 20, QSizePolicy::Maximum, QSizePolicy::Expanding);
    mainLayout->addItem(mainRightSpacer);
    showWindow(mainLayout, "Hardware Settings");
    exec();

}

void HwInfConfig::handleAxisEnabled(int id)
{
    QString axisName = settings->axisNames().at(id);
    QAbstractButton *btn = bgrpEnabled->button(id);

    if ( btn->text().compare("Enabled") == 0 ) {
        btn->setText("Disabled");
        settings->setAxisEnabled(axisName, false);

    } else {
        btn->setText("Enabled");
        settings->setAxisEnabled(axisName, true);
    }
}

void HwInfConfig::handleAxisDiameterMode(int id)
{
    QString axisName = settings->axisNames().at(id);
    QAbstractButton *btn = bgrpDiameterMode->button(id);

    if ( btn->text().compare("Enabled") == 0 ) {
        btn->setText("Disabled");
        settings->setAxisDiameterMode(axisName, false);

    } else {
        btn->setText("Enabled");
        settings->setAxisDiameterMode(axisName, true);
    }
}
void HwInfConfig::handleAxisRevDirection(int id)
{
    QString axisName = settings->axisNames().at(id);
    QAbstractButton *btn = bgrpRevDirection->button(id);

    if ( btn->text().compare("Enabled") == 0 ) {
        btn->setText("Disabled");
        settings->setAxisRevDirection(axisName, false);

    } else {
        btn->setText("Enabled");
        settings->setAxisRevDirection(axisName, true);
    }
}
