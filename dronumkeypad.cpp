#include "dronumkeypad.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpacerItem>
#include <QPushButton>
#include <QLCDNumber>
#include <QDebug>

DRONumKeypad::DRONumKeypad(int w, int h, QWidget *parent) : QWidget(parent)
{
    if ( w != 0 & h != 0 )
        setFixedSize(w, h);

    createUi();
}

QString DRONumKeypad::getKeypadValue()
{
    return keypadValue;
}

void DRONumKeypad::setKeypadValue(QString value)
{
    keypadValue = value;
    lcdKeypad->display(keypadValue);
}

void DRONumKeypad::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    lcdKeypad = new QLCDNumber();

    lcdKeypad->setDigitCount(MAX_DIGITS);
    lcdKeypad->setSegmentStyle(QLCDNumber::Flat);
    lcdKeypad->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    lcdKeypad->setMinimumHeight(40);
    lcdKeypad->display("");
    mainLayout->addWidget(lcdKeypad);

    //QVBoxLayout *keypadLayout = new QVBoxLayout();
    QStringList keyValues({"7", "8", "9", "4", "5", "6", "1", "2", "3", "0", KEY_DEC, KEY_NEG, KEY_BAK, KEY_CLR, KEY_ENT});
    int colLength = 4;
    int rowLength = keyValues.length() / colLength;
    int overFlow = keyValues.length() % colLength;

    for ( int i = 0; i < keyValues.length(); i+=rowLength ) {
        QHBoxLayout *row = new QHBoxLayout();
        row->setMargin(0);

        if ( i + overFlow >= keyValues.length() )
            rowLength = overFlow;

        for ( int j = 0; j < rowLength; j++ ){
            QPushButton *btn = new QPushButton(keyValues.at(i+j));
            btn->setFocusPolicy(Qt::NoFocus);
            btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            connect(btn, SIGNAL(clicked()), this, SLOT(handleKeyPress()));
            row->addWidget(btn);
        }

        mainLayout->addItem(row);
    }

    setLayout(mainLayout);
    show();
}

void DRONumKeypad::handleKeyPress()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(QObject::sender());
    int decOffset = 1;

    if ( btn->text().compare(KEY_BAK) == 0) {
        keypadValue = keypadValue.left(keypadValue.length()-1);

    } else if ( btn->text().compare(KEY_CLR) == 0) {
        emit keyPressClear();
        keypadValue = "";

    } else if ( btn->text().compare(KEY_ENT) == 0) {
        emit keyPressEnter();
        emit keyPressEnter(keypadValue);
        keypadValue = "";

    } else if ( btn->text().compare(KEY_DEC) == 0) {
        if ( !keypadValue.contains('.') )
            keypadValue = QString("%1.").arg(keypadValue);

    } else if ( btn->text().compare(KEY_NEG) == 0) {
        if ( keypadValue.left(1).compare("-") == 0 )
            keypadValue = keypadValue.right(keypadValue.length() - 1);
        else
            keypadValue = QString("-%1").arg(keypadValue);

    } else {
        keypadValue = QString("%1%2").arg(keypadValue).arg(btn->text());
    }
    if ( keypadValue.left(1).contains('-') )
        decOffset = 0;

    keypadValue = keypadValue.right(MAX_DIGITS - decOffset);
    setKeypadValue(keypadValue);
}
