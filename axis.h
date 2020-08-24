#ifndef AXIS_H
#define AXIS_H

#include "drosettings.h"
#include "hardwareinf.h"
#include <QObject>
#include <QLCDNumber>
#include <QWidget>
#include <QPushButton>

class Axis : public QObject
{
    Q_OBJECT
public:
    explicit Axis(QString axisName, QObject *parent = nullptr);
    QWidget*        axisReadout();
    QString         getName();
    void            setValue(double value);
    double          getValue();
    double          getAbsValue();
    void            setZero(double value);
    double          getZero();
    void            setOffset(double value);
    bool            getSelected();
    void            setSelected(bool selected);
    bool            getDisabled();
    void            setDisabled(bool disabled);
    void            setRevDirection(bool revDirection);
    void            setDiameterMode(bool isDiaMode);
    void            setSiUnits(bool isSiUnits);
    bool            getSiUnits();
    void            show();
    void            hide();

private:
    QLCDNumber      *lcdReadout;
    QPushButton     *btnZero;
    QPushButton     *btnSelect;
    QWidget         *mainWidget;
    QString         _axisName;
    double          _value = 0;
    double          _absValue = 0;
    double          _zero = 0;
    double          _offset = 0;
    bool            _isSiUnits = true;
    bool            _isSelected = false;
    bool            _isDisabled = true;
    int             _direction = 1;
    int             _diaMode = 1;

signals:
    void            selectClicked(QString axisName);

private slots:
    void            handleAxisZero();
    void            handleAxisSelect();
};

#endif // AXIS_H
