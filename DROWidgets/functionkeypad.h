#ifndef FUNCTIONKEYPAD_H
#define FUNCTIONKEYPAD_H

#include "drosettings.h"

#include <QObject>
#include <QWidget>

class FunctionKeypad : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionKeypad(DROSettings *settings, QWidget *parent = nullptr);
    QWidget *               keypad();

private:
    DROSettings             *settings;

signals:

public slots:
};

#endif // FUNCTIONKEYPAD_H
