#include "axis.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
Axis::Axis(QString axisName, DROSettings *settings, QObject *parent) : QObject(parent)
{
    this->_axisName = axisName;
    this->settings = settings;
    setDiameterMode(settings->getAxisDiameterMode(axisName));
    setRevDirection(settings->getAxisRevDirection(axisName));
    setSiUnits(settings->getUiUnits());
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
    lcdReadout->setMinimumWidth(200);
    lcdReadout->setMinimumHeight(75);
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
 * Unit conversions and properties.
 * On change convert offset and zero, let next update
 * deal with value.
 */
void Axis::setHardwareSiUnits(bool isHardwareSiUnits)
{
    if ( _isHardwareSiUnits != isHardwareSiUnits ) {
        if ( isHardwareSiUnits ) {
            _offset *= 25.4;
            _zero *= 25.4;

        } else {
            _offset /= 25.4;
            _zero /= 25.4;
        }

        _isHardwareSiUnits = isHardwareSiUnits;
    }
}

bool Axis::getHardwareSiUnits()
{
    return _isHardwareSiUnits;
}

void Axis::setSiUnits(bool isSiUnits)
{
    if ( _isSiUnits != isSiUnits ) {
        if ( isSiUnits ) {
            _offset *= 25.4;
            _zero *= 25.4;

        } else {
            _offset /= 25.4;
            _zero /= 25.4;
        }

        _isSiUnits = isSiUnits;
    }
}

bool Axis::getSiUnits()
{
    return _isSiUnits;
}

double Axis::unitConversion(double value)
{
    if ( getSiUnits() == getHardwareSiUnits() )
        return value;

    if ( ! getHardwareSiUnits() )
        return value * 25.4;
    else
         return value / 25.4;
}

/*
 * Value settings.
 */

void Axis::setValue(double value)
{
    value = unitConversion(value);
    _absValue = value;

    if ( getSelected() )
        return;
    else if ( getDisabled() )
        return;

    _value = value - _zero + _offset;

    _value *= _diaMode;
    _value *= _direction;

    if ( getSiUnits() )
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
    value = unitConversion(value);
    _zero = getAbsValue() - (value / _diaMode * _direction);
}

double Axis::getZero()
{
    return _zero;
}

void Axis::setOffset(double value)
{
    value = unitConversion(value);
    _offset = value;
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
    setOffset(0);
    setZero(0);
}

void Axis::handleAxisSelect()
{
    setSelected(!_isSelected);
    emit selectClicked(getName());
}
