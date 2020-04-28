#include "functionkeypad.h"
#include "drosettings.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

FunctionKeypad::FunctionKeypad(DROSettings *settings, QWidget *parent) : QWidget(parent)
{
    this->settings = settings;

}

QWidget * FunctionKeypad::keypad()
{
    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);
    return mainWidget;
}
