#ifndef DROKEYPAD_H
#define DROKEYPAD_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLCDNumber>

#define MAX_DIGITS      10

#define KEY_DEC         "."
#define KEY_NEG         "+/-"
#define KEY_BAK         "Back"
#define KEY_CLR         "CLR"
#define KEY_ENT         "Enter"
/*
#define KEY_DIV         "/"
#define KEY_MUL         "*"
#define KEY_SUB         "-"
#define KEY_ADD         "+"
#define KEY_SIN         "Sin"
#define KEY_COS         "Cos"
#define KEY_TAN         "Tan"
#define KEY_SIN2        "Sin-1"
#define KEY_COS2        "Cos-1"
#define KEY_TAN2        "Tan-1"
*/
class DRONumKeypad : public QWidget
{
    Q_OBJECT
public:
    explicit DRONumKeypad(int w = 0, int h = 0, QWidget *parent = nullptr);
    QStringList         keyValues;
    QString             keypadValue;
    QString             getKeypadValue();
    void                setKeypadValue(QString value);

private:
    void                createUi();
    QGroupBox           *gbKeypad;
    QLCDNumber          *lcdKeypad;

signals:
    void                keyPressEnter();
    void                keyPressEnter(QString keypadValue);
    void                keyPressClear();

public slots:
    void                handleKeyPress();

};

#endif // DROKEYPAD_H
