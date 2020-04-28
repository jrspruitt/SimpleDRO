#include "drofunctions.h"
#include "drosettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

DROFunctions::DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent) : QWidget(parent)
{
    this->settings = settings;
    this->axisReadouts = axisReadouts;
    createUi();
}

void DROFunctions::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QPushButton *btnHalf = new QPushButton("1/2");
    mainLayout->addWidget(btnHalf);
    QPushButton *btnBoltCircle = new QPushButton("Bolt Circle");
    mainLayout->addWidget(btnBoltCircle);
    QPushButton *btnLocations = new QPushButton("Locations");
    connect(btnLocations, SIGNAL(clicked()), this, SLOT(handleLocations()));
    mainLayout->addWidget(btnLocations);
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

QStringList DROFunctions::getSelectedAxes()
{
    QStringList selectedAxes;

    foreach ( const QString axisName, settings->axisNames() ) {
        if ( axisReadouts->value(axisName)->getSelected() )
            selectedAxes.append(axisName);
    }

    return selectedAxes;
}

void DROFunctions::enterValue(double value)
{
    int _inputValue;
    switch( _curFunction ) {
    case FUNC_LOCATION:
        // set value into var, call original function
        _inputValue = value;
        handleLocations();
        break;
    case FUNC_HALF:
        break;
    case FUNC_BOLT_CIRCLE:
        break;

    }

    setSelected(false);
}

void DROFunctions::handleLocations()
{
    /*
     * State machine, uses _inputValue to set current value
     * This will repeatedly get called until all values input
     * then calculations as necessary, set axis values as needed
     * or cancel. Which will reset all tracking variables
     * probably a good place for a struct
     */
    QStringList selAxes = getSelectedAxes();
    foreach ( const QString axisName, selAxes) {

    }
    setSelected(true);
    emit message("Please select location number.");
}
