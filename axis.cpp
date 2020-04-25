#include "axis.h"
#include "hardwareinf.h"
#include "drosettings.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

Axis::Axis(QString axisName, QObject *parent) : QObject(parent)
{
    this->_axisName = axisName;
}

QWidget * Axis::axisReadout()
{
    QWidget *mainWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    lcdReadout = new QLCDNumber();
    lcdReadout->setDigitCount(7);
    lcdReadout->setSegmentStyle(QLCDNumber::Flat);
    mainLayout->addWidget(lcdReadout);

    QPushButton *btnZero = new QPushButton(QString("%1/0").arg(_axisName));
    btnZero->setFixedSize(40, 40);
    connect(btnZero, SIGNAL(clicked()), this, SLOT(handleAxisZero()));
    mainLayout->addWidget(btnZero);

    QPushButton *btnSelect = new QPushButton(_axisName);
    btnSelect->setFixedSize(40, 40);
    connect(btnSelect, SIGNAL(clicked()), this, SLOT(handleAxisSelect()));
    mainLayout->addWidget(btnSelect);

    mainWidget->setLayout(mainLayout);
    return mainWidget;
}

QString Axis::getName()
{
    return _axisName;
}

bool Axis::getSelected()
{
    return _isSelected;
}

void Axis::setSelected(bool selected)
{
    _isSelected = selected;
}

void Axis::setValue(double value, bool isSiUnits)
{
    if ( getSelected() ) {
        lcdReadout->display("SEL");
        return;
    }

    _absValue = value;
    _value = value - _zero;
    _isSiUnits = isSiUnits;

    if ( _isSiUnits )
        lcdReadout->display(QString::number(_value, 'f', 2));
    else
        lcdReadout->display(QString::number(_value, 'f', 4));
}

double Axis::getValue()
{
    return _value;
}

void Axis::setZero(double value)
{
    _zero = value;
}

double Axis::getZero()
{
    return _zero;
}

void Axis::setOffset(double value)
{
    qDebug() << value << endl;
    setZero(getAbsValue() - value);
}

double Axis::getAbsValue()
{
    return _absValue;
}

void Axis::handleAxisZero()
{
    setZero(_absValue);
}

void Axis::handleAxisSelect()
{
    setSelected(!_isSelected);
}
