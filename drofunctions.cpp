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
    funcNames = QStringList({"Set Location", "Get Location", "Half", "Bolt Circle"});
    /*
    funcList = new QHash<int, funcData *>;
    funcNames = QStringList({FUNC_SETLOCATION, FUNC_HALF, FUNC_BOLT_CIRCLE});
    const QStringList msgLocList({"Go to location, and press select", "Go to second location and press select.", ""});
    funcList->insert(func_set_loc, getFuncData(FUNC_SETLOCATION, 2, &DROFunctions::handleLocations, msgLocList));
    QStringList msgHalfList({"Go to first location, and press axis selects.", "Go to second location and press axis selects.", ""});
    funcList->insert(func_half, getFuncData(FUNC_HALF, 2, &DROFunctions::handleHalf, msgHalfList));
    */
    bgrpFunctions = new QButtonGroup();
    createUi();

}

/*
DROFunctions::funcData* DROFunctions::getFuncData(QString name, int listCount, funcHandler func , QStringList msgList)
{
    valuelist_t vals = valuelist_t();

    foreach ( const QString axisName, settings->axisNames() )
        vals.insert(axisName, new QList<double>()); //[static_cast<unsigned long>(listCount)]());

    funcData *retFuncData = new funcData({name, func, listCount, vals, msgList, 0});
    return retFuncData;
}
*/
void DROFunctions::createUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *funcLayout = new QVBoxLayout();
    foreach ( const QString funcName, funcNames ) {
        QPushButton *btn = new QPushButton(funcName);
        bgrpFunctions->addButton(btn, funcNames.indexOf(funcName));
        funcLayout->addWidget(btn);
    }
    connect(bgrpFunctions, SIGNAL(buttonClicked(int)), this, SLOT(handleFunctionButtons(int)));

    QPushButton *btnReset = new QPushButton("Reset");
    connect(btnReset, SIGNAL(clicked()), this, SLOT(handleReset()));
    funcLayout->addWidget(btnReset);
    mainLayout->addItem(funcLayout);

    keypad = new DRONumKeypad();
    mainLayout->addWidget(keypad);
    connect(keypad, SIGNAL(keyPressEnter()), this, SLOT(handleFunction()));

    setLayout(mainLayout);
    show();
}

void DROFunctions::setSelected(bool selected)
{
    _selected = selected;
}

bool DROFunctions::getSelected()
{
    return _selected;
}

void DROFunctions::enterValue(double value)
{
    _curInputValue = value;
    handleFunction();
}

/*
 * Handle button presses, if none are active, activate, else warn
 * a function is currently active.
 */
void DROFunctions::handleFunctionButtons(int id)
{
    if ( _curFunction < 0 ) {
        _curFunction = id; //funcNames.at(id);
        _selected = true;
        handleFunction();
    } else {
        emit message(QString("%1 Active, please clear to start another function.").arg(_curFunction));
    }
}

void DROFunctions::handleFunction()
{
    switch ( _curFunction ) {
    case none:
        handleFuncNone();
        break;
    case func_set_loc:
        qDebug() << FUNC_SETLOCATION << endl;
        //func_set_location();
        break;
    case func_get_loc:
        qDebug() << FUNC_GETLOCATION << endl;
        //func_get_lacation();
        break;
    case func_half:
        qDebug() << FUNC_HALF << endl;
        handleFuncHalf();
        break;
    case func_bolt_circle:
        qDebug() << FUNC_BOLT_CIRCLE << endl;
        break;
    }

    /*
    int argIndex = funcList->value(_curFunction)->argIndex;
    int argCount = funcList->value(_curFunction)->argc;
    QString msg = funcList->value(_curFunction)->msglist.at(argIndex);

    foreach ( const QString axisName, settings->axisNames() ) {
        if ( axisReadouts->value(axisName)->getSelected() ) {
            funcList->value(_curFunction)->argv.value(axisName)->append(axisReadouts->value(axisName)->getAbsValue());
            axisReadouts->value(axisName)->setSelected(false);
        }
    }

    emit message(msg);
    funcList->value(_curFunction)->argIndex++;

    if ( argIndex >= argCount ) {
        _selected = false;
        (this->*funcList->value(_curFunction)->func)();
        _curFunction = "";
    }
*/
}

void DROFunctions::handleFuncNone()
{
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( axisReadouts->value(axisName)->getSelected() ) {
            axisReadouts->value(axisName)->setOffset(keypad->getKeypadValue().toDouble());
            axisReadouts->value(axisName)->setSelected(false);
        }
    }
    /*
    if ( getSelected() ) {
        enterValue(value.toDouble());

    } else {
        foreach ( const QString name, settings->axisNames() ) {
            if ( axisReadouts->value(name)->getSelected() ) {
                axisReadouts->value(name)->setOffset(value.toDouble());
                axisReadouts->value(name)->setSelected(false);
            }
        }
    }
    */
}

void DROFunctions::handleLocations()
{

}

void DROFunctions::handleFuncHalf()
{
    _funcHalfValues = new QHash <QString, double>; // x, y, z

    switch (_curIndex ) {
    case 0:
        emit message("Go to first location, and press axis selects.");
        _curIndex++;
        break;
    case 1:
        foreach ( const QString axisName, settings->axisNames() ) {
            if ( axisReadouts->value(axisName)->getSelected() ) {
                _funcHalfValues->insert(axisName, axisReadouts->value(axisName)->getAbsValue());
                qDebug() << "half" << _funcHalfValues->value(axisName) << endl;
                axisReadouts->value(axisName)->setSelected(false);
            }
        }
        emit message("Go to second location and press axis selects.");
        _curIndex++;
        break;
    case 2:
        foreach ( const QString axisName, settings->axisNames() ) {
            if ( axisReadouts->value(axisName)->getSelected() ) {
                double arg1 = _funcHalfValues->value(axisName);
                double arg2 = axisReadouts->value(axisName)->getAbsValue();
                double offset = (arg1 + arg2) / 2.0;
                qDebug() << arg1 << arg2 << endl;
                axisReadouts->value(axisName)->setZero(offset);
                axisReadouts->value(axisName)->setSelected(false);
            }
        }
        handleReset();
        break;
    }
/*
    {, "", ""}
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( funcList->value(_curFunction)->argv.value(axisName)->size() > 0 ) {
            double arg1 = funcList->value(_curFunction)->argv.value(axisName)->at(0);
            double arg2 = funcList->value(_curFunction)->argv.value(axisName)->at(1);
            double offset = (arg1 + arg2) / 2.0;
            axisReadouts->value(axisName)->setZero(offset);
            funcList->value(_curFunction)->argv.value(axisName)->clear();
        }
    }

    //funcList->value(_curFunction)->argIndex = 0;
    handleReset();
    */
}

void DROFunctions::handleKeyPressEnter(QString value)
{
    handleFunction();
}

void DROFunctions::handleReset()
{
    _selected = false;
    _curFunction = -1;
    _curInputValue = 0;
    _curIndex = 0;
    _nextInput = 0;
    emit message("");
}
/*
void SimpleDRO::handleAxisSelect(QString axisName)
{
    if ( droFunctions->getSelected() )
        droFunctions->enterValue(axisReadouts->value(axisName)->getValue());
}
*/
