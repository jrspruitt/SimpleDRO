#ifndef AXIS_H
#define AXIS_H

#include "drosettings.h"
#include "hardwareinf.h"
#include <QObject>
#include <QLCDNumber>
#include <QWidget>

#define XAXIS
class Axis : public QObject
{
    Q_OBJECT
public:
    explicit Axis(QString axisName, QObject *parent = nullptr);
    QWidget*        axisReadout();
    QString         getName();
    void            setValue(double value, bool isSiUnits);
    double          getValue();
    double          getAbsValue();
    void            setZero(double value);
    double          getZero();
    void            setOffset(double value);
    bool            getSelected();
    void            setSelected(bool selected);

private:
    QLCDNumber      *lcdReadout;
    QString         _axisName;
    double          _value;
    double          _absValue;
    double          _zero = 0;
    bool            _isSiUnits = true;
    bool            _isSelected = false;

signals:

private slots:
    void            handleAxisZero();
    void            handleAxisSelect();
};

#endif // AXIS_H
