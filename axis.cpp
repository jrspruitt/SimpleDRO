#include "axis.h"
#include "hardwareinf.h"
#include "drosettings.h"

#include <QHBoxLayout>
#include <QPushButton>

Axis::Axis(QString axisName, QObject *parent) : QObject(parent)
{
    this->_axisName = axisName;
    setZero(0);
    setOffset(0);
}

QString Axis::getName()
{
    return _axisName;
}
/*
 * Display widget of Axis Readout.
*/
QWidget * Axis::axisReadout()
{
    mainWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    lcdReadout = new QLCDNumber(mainWidget);
    lcdReadout->setDigitCount(7);
    lcdReadout->setSegmentStyle(QLCDNumber::Flat);
    lcdReadout->display("OFF");
    lcdReadout->setDisabled(true);
     mainLayout->addWidget(lcdReadout);

    btnZero = new QPushButton(QString("%1/0").arg(_axisName), mainWidget);
    btnZero->setFixedSize(40, 40);
    connect(btnZero, SIGNAL(clicked()), this, SLOT(handleAxisZero()));
    btnZero->setDisabled(true);
    mainLayout->addWidget(btnZero);

    btnSelect = new QPushButton(_axisName, mainWidget);
    btnSelect->setFixedSize(40, 40);
    connect(btnSelect, SIGNAL(clicked()), this, SLOT(handleAxisSelect()));
    btnSelect->setDisabled(true);
    mainLayout->addWidget(btnSelect);

    mainWidget->setLayout(mainLayout);
    mainWidget->hide();
    return mainWidget;
}

/*
 * Display and function mode settings.
 */
void Axis::show()
{
    mainWidget->show();
}
void Axis::hide()
{
    mainWidget->hide();
}

void Axis::setDisabled(bool disabled)
{
    if ( _isDisabled == disabled )
        return;

    _isDisabled = disabled;
    if ( _isDisabled )
        lcdReadout->display("OFF");

    lcdReadout->setDisabled(disabled);
    btnZero->setDisabled(disabled);
    btnSelect->setDisabled(disabled);
}

bool Axis::getDisabled()
{
    return _isDisabled;
}

void Axis::setSelected(bool selected)
{
    _isSelected = selected;
    if ( _isSelected )
        lcdReadout->display("SEL");
}

bool Axis::getSelected()
{
    return _isSelected;
}

/*
 * Value settings.
 */

void Axis::setValue(double value, bool isSiUnits)
{
    value = value * _direction;
    value = value * _diaMode;

    _absValue = value;

    if ( getSelected() )
        return;
    else if ( getDisabled() )
        return;

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
    setZero(getAbsValue() - value);
}

double Axis::getAbsValue()
{
    return _absValue;
}

/*
 * Settings for axis hardware.
 */

void Axis::setRevDirection(bool revDirection)
{
   if ( revDirection )
       _direction = -1;
   else
       _direction = 1;
}

void Axis::setDiameterMode(bool isDiaMode)
{
    if ( isDiaMode )
        _diaMode = 2;
    else
        _diaMode = 1;
}

/*
 * Handlers for events.
 */
void Axis::handleAxisZero()
{
    setZero(_absValue);
}

void Axis::handleAxisSelect()
{
    setSelected(!_isSelected);
}
