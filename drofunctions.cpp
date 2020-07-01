#include "drofunctions.h"
#include "drosettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
DROFunctions::DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent) : QWidget(parent)
{
    this->settings = settings;
    this->axisReadouts = axisReadouts;
    funcList = new QHash<QString, funcData *>;
    funcNames = QStringList({FUNC_LOCATION, FUNC_HALF, FUNC_BOLT_CIRCLE});
    const QStringList msgLocList({"Go to location, and press select", "Go to second location and press select.", ""});
    funcList->insert(FUNC_LOCATION, getFuncData(2, &DROFunctions::handleLocations, msgLocList));
    QStringList msgHalfList({"Go to first location, and press axis selects.", "Go to second location and press axis selects.", ""});
    funcList->insert(FUNC_HALF, getFuncData(2, &DROFunctions::handleHalf, msgHalfList));
    bgrpFunctions = new QButtonGroup();
    createUi();

}

DROFunctions::funcData* DROFunctions::getFuncData(int listCount, funcHandler func , QStringList msgList)
{
    valuelist_t vals = valuelist_t();

    foreach ( const QString axisName, settings->axisNames() )
        vals.insert(axisName, new QList<double>()); //[static_cast<unsigned long>(listCount)]());

    funcData *retFuncData = new funcData({FUNC_LOCATION, func, listCount, vals, msgList, 0});
    return retFuncData;
}

void DROFunctions::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    foreach ( const QString funcName, funcNames ) {
        QPushButton *btn = new QPushButton(funcName);
        bgrpFunctions->addButton(btn, funcNames.indexOf(funcName));
        mainLayout->addWidget(btn);
    }
    QPushButton *btnReset = new QPushButton("Reset");
    connect(btnReset, SIGNAL(clicked()), this, SLOT(handleReset()));
    mainLayout->addWidget(btnReset);
    connect(bgrpFunctions, SIGNAL(buttonClicked(int)), this, SLOT(handleFunctionButtons(int)));
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
    handleFunctionConfig();
}

void DROFunctions::handleFunctionButtons(int id)
{
    if ( _curFunction.compare("") == 0 ) {
        _curFunction = funcNames.at(id);
        _selected = true;
        handleFunctionConfig();
    } else {
        emit message(QString("%1 Active, please clear to start another function.").arg(_curFunction));
    }

}
void DROFunctions::handleFunctionConfig()
{
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

}

void DROFunctions::handleLocations()
{

}

void DROFunctions::handleHalf()
{
    foreach ( const QString axisName, settings->axisNames() ) {
        if ( funcList->value(_curFunction)->argv.value(axisName)->size() > 0 ) {
            double arg1 = funcList->value(_curFunction)->argv.value(axisName)->at(0);
            double arg2 = funcList->value(_curFunction)->argv.value(axisName)->at(1);
            double offset = (arg1 + arg2) / 2.0;
            axisReadouts->value(axisName)->setZero(offset);
            funcList->value(_curFunction)->argv.value(axisName)->clear();
        }
    }

    funcList->value(_curFunction)->argIndex = 0;
    emit message("");
}

void DROFunctions::handleReset()
{
    _selected = false;
    _curFunction = "";
    _curInputValue = 0;
    emit message("");
}
