#include "axisreadout.h"
#include "drosettings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QString>
#include <QGroupBox>
#include <QSpacerItem>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

AxisReadout::AxisReadout(QString axisName, DROSettings *settings, QWidget *parent) : QWidget(parent)
{
    this->axisName = axisName;
    this->axisLabel = settings->getAxisLabel(axisName);
    this->settings = settings;

    pixActive = new QPixmap(settings->activeIconPath);
    pixInactive = new QPixmap(settings->inactiveIconPath);
    //setStyleSheet("*{border:1px solid black;}");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //setFixedSize(settings->screenWidth/2, settings->screenHeight/6);
    createUi();
}

void AxisReadout::createUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);

    QHBoxLayout *noHideLayout = new QHBoxLayout();
    noHideLayout->setMargin(0);

    btnShow = new QPushButton("^");
    btnShow->setFixedSize(20,20);
    btnShow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(btnShow, SIGNAL(clicked()), this, SLOT(handleShowHide()));
    noHideLayout->addWidget(btnShow);

    lblAxisLabel = new QLabel(axisLabel);
    lblAxisLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    noHideLayout->addWidget(lblAxisLabel);

    noHideSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    noHideLayout->addItem(noHideSpacer);
    mainLayout->addItem(noHideLayout);

    QHBoxLayout *showHideLayout = new QHBoxLayout();
    showHideLayout->setMargin(0);

    lcdReadout = new QLCDNumber();
    lcdReadout->setDigitCount(7);
    lcdReadout->setSegmentStyle(QLCDNumber::Flat);
    lcdReadout->setMinimumSize(200, 75);
    lcdReadout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setColor(colorGreen);
    showHideLayout->addWidget(lcdReadout);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setMargin(0);

    QHBoxLayout *unitsLayout = new QHBoxLayout();
    unitsLayout->setMargin(0);

    lblUnits = new QLabel(UNITS_SI);
    lblUnits->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    unitsLayout->addWidget(lblUnits);

    infoLayout->addItem(unitsLayout);
    showHideLayout->addItem(infoLayout);

    btnZero = new QPushButton("Zero");
    btnZero->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    showHideLayout->addWidget(btnZero);

    lblOn = new QLabel();
    lblOn->setFixedSize(16, 16);
    lblOn->setPixmap(*pixInactive);
    showHideLayout->addWidget(lblOn);

    gbShowHide = new QGroupBox();
    gbShowHide->setLayout(showHideLayout);
    gbShowHide->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(gbShowHide);

    shSpacer = new QSpacerItem(5, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addItem(shSpacer);

    setLayout(mainLayout);
    show();

    // Inverted to trigger show in handleShowHide()
    if ( settings->getAxisEnabled(axisName) )
        gbShowHide->hide();

    handleShowHide();
}

void AxisReadout::setValue(double value) {
    if ( isMetric )
        lcdReadout->display(QString::number(value, 'f', 2));
    else
        lcdReadout->display(QString::number(value, 'f', 4));
}

void AxisReadout::setUnits(QString units) {
    if ( units.compare(UNITS_IMP) == 0 ) {
        lblUnits->setText(UNITS_IMP);
        isMetric = false;

    } else if (units.compare(UNITS_SI) == 0) {
        lblUnits->setText(UNITS_SI);
        isMetric = true;

    } else {
        lblUnits->setText(UNITS_SI);
        isMetric = true;
        qDebug() << "Error setting units" << endl;
    }
}

void AxisReadout::setOn(bool on) {
    if ( on )
        lblOn->setPixmap(*pixActive);
    else
        lblOn->setPixmap(*pixInactive);
}

void AxisReadout::setColor(const QString color) {
    lcdReadout->setStyleSheet(QString("color:%1; background-color:black;").arg(color));
}

void AxisReadout::handleShowHide(){
    if ( gbShowHide->isHidden() ) {
        noHideSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Maximum);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gbShowHide->show();
        btnShow->setText("^");
        update();

    } else {
        noHideSpacer = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        gbShowHide->hide();
        btnShow->setText("v");
        update();
    }
}
