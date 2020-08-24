#include "drofunctions.h"
#include "drosettings.h"
#include "dronumkeypad.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

DROFunctions::DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent) : QWidget(parent)
{
    this->settings = settings;
    this->axisReadouts = axisReadouts;
    bgrpFunctions = new QButtonGroup();
    _funcTempValues = new QList<QHash <QString, double>>; // x, y, z
    createUi();

}

void DROFunctions::createUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *funcLayout = new QVBoxLayout();
    foreach ( const QString funcName, funcNames ) {
        QPushButton *btn = new QPushButton(funcName);
        bgrpFunctions->addButton(btn, funcNames.indexOf(funcName));
        funcLayout->addWidget(btn);
    }

    connect(bgrpFunctions, SIGNAL(buttonClicked(int)), this, SLOT(handleStartFunc(int)));
    mainLayout->addItem(funcLayout);

    keypad = new DRONumKeypad();
    mainLayout->addWidget(keypad);
    connect(keypad, SIGNAL(keyPressEnter()), this, SLOT(handleConfigFunc()));
    connect(keypad, SIGNAL(keyPressClear()), this, SLOT(handleStopFunc()));

    setLayout(mainLayout);
    show();
}

/*
 * Handle function activation button presses, if none are active,
 * activate, else warn a function is currently active.
 * -1 is the manual input function and is default behavior.
 */
void DROFunctions::handleStartFunc(int id)
{
    if ( _curFunction < 0 ) {
        _curFunction = id;
        handleConfigFunc();
    } else {
        emit message(QString("%1 Active, please clear to start another function.").arg(_curFunction));
    }
}

/*
 * Function clean up or clr has been pressed.
 */
void DROFunctions::handleStopFunc(QString msg)
{
    _curFunction = -1;
    _curIndex = 0;
    _funcTempValues->clear();
    emit message(msg);
}

/*
 * Handle function. Redirect to the currently selected functions
 * handler. Values are from functionList enum.
 */
void DROFunctions::handleConfigFunc()
{
    switch ( _curFunction ) {
    case func_default:
        handleFuncDefault();
        break;
    case func_half:
        handleFuncHalf();
        break;
    case func_set_offset:
        handleFuncSetOffset();
        break;
    case func_get_offset:
        handleFuncGetOffset();
        break;
    }
}

/*
 * Default function handler. This is special as it is the
 * default behavior for the application.
 * Usage: Select Axis, input number in keypad, hit enter, the axis
 * is now set to that value. Multiple select possible.
 */
void DROFunctions::handleFuncDefault()
{
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( axisReadouts->value(axisName)->getSelected() ) {
            axisReadouts->value(axisName)->setZero(keypad->getKeypadValue().toDouble());
            axisReadouts->value(axisName)->setSelected(false);
        }
    }
}

/*
 * 1/2 Function.
 * Usage: Go to first location, select axxis and hit enter. Go
 * to next location, and repeat, the axis values will now be 1/2
 * the distance from the mid point.
 */
void DROFunctions::handleFuncHalf()
{
    switch (_curIndex ) {
    case 0:
        emit message("Go to first location, select axis then enter.");
        _curIndex++;
        break;

    case 1:
        foreach ( const QString axisName, settings->axisNames() ) {
            if ( axisReadouts->value(axisName)->getSelected() ) {
                QHash<QString, double> tmpVal;
                tmpVal.insert(axisName, axisReadouts->value(axisName)->getAbsValue());
                _funcTempValues->insert(0, tmpVal);
                axisReadouts->value(axisName)->setSelected(false);
            }
        }

        emit message("Go to second location, select axis then enter.");
        _curIndex++;
        break;

    case 2:
        foreach ( const QString axisName, settings->axisNames() ) {
            if ( axisReadouts->value(axisName)->getSelected() ) {
                double arg1 = _funcTempValues->at(0).value(axisName);
                double arg2 = axisReadouts->value(axisName)->getAbsValue();
                double offset = (arg1 + arg2) / 2.0;
                axisReadouts->value(axisName)->setZero(offset);
                axisReadouts->value(axisName)->setSelected(false);
            }
        }

        handleStopFunc();
        break;
    }
}

/*
 * Set Locations, uses config file to store entered locations
 * for recall at a later time.
 * Usage: Enter slot number, hit enter. Enter a value and hit enter.
 * TODO: Figure out a way this would actually work, as multiple
 * axes will be needed. Maybe just work thru the axes if x then x, if y then y, if z then z.
 */
void DROFunctions::handleFuncSetOffset()
{
    switch (_curIndex ) {
    case 0:
        emit message("Select offset slot number 0 to clear.");
        _curIndex++;
        break;

    case 1:
        _curOffsetSlot = keypad->getKeypadValue().toInt();
        // Slot 0 reserved for clearing offsets.
        if ( _curOffsetSlot == 0 ) {
            emit message("Clearing offset.");
            foreach ( const QString axisName, settings->axisNames() ) {
                if ( axisReadouts->value(axisName)->getSelected() ) {
                    settings->setAxisOffset(axisName, _curOffsetSlot, 0.0);
                }
            }

            handleStopFunc();
            break;
        }

        emit message("Select axes, enter value, and hit enter.");
        _curIndex++;
        break;

    case 2:
        bool wasSelected = false;

        if ( keypad->getKeypadValue() == "" ) {
            emit message("No value provided.");
            break;
        }

        foreach ( const QString axisName, settings->axisNames() ) {
            if ( axisReadouts->value(axisName)->getSelected() ) {
                wasSelected = true;
                settings->setAxisOffset(axisName, _curOffsetSlot, keypad->getKeypadValue().toDouble());
                axisReadouts->value(axisName)->setSelected(false);
            }
        }

        if ( ! wasSelected ) {
            emit message("No axes were selected.");
            break;
        }

        _curOffsetSlot = 0;
        handleStopFunc();
        break;
    }
}

void DROFunctions::handleFuncGetOffset()
{
    switch (_curIndex ) {
    case 0:
        emit message("Select offset slot number and hit enter.");
        _curIndex++;
        break;

    case 1:
        _curOffsetSlot = keypad->getKeypadValue().toInt();

        foreach ( const QString axisName, settings->axisNames() ) {
            if ( ! axisReadouts->value(axisName)->getDisabled() ) {
                double tmpVal = settings->getAxisOffset(axisName, keypad->getKeypadValue().toDouble());

                if ( tmpVal == 123456789.10 )
                    emit message(QString("No value set for %1").arg(axisName));
                else
                    axisReadouts->value(axisName)->setOffset(tmpVal);
            }
        }

        _curOffsetSlot = 0;
        handleStopFunc();
        break;
    }
}
