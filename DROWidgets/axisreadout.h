#ifndef DROREADOUT_H
#define DROREADOUT_H

#include "drosettings.h"
#include <QObject>
#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QSpacerItem>
#include <QHBoxLayout>

class AxisReadout : public QWidget
{
    Q_OBJECT
public:
    explicit AxisReadout(QString axisName, DROSettings *settings, QWidget *parent = nullptr);
    void                setValue(double value);
    void                setUnits(const QString units);
    void                setOn(bool on);
    void                setColor(const QString color);
    const QString       colorRed = "red";
    const QString       colorGreen = "greenyellow";
    QPushButton         *btnZero;
    QPushButton         *btnShow;

private:
    DROSettings         *settings;
    QString             axisName;
    QString             axisLabel;
    void                createUi();
    QLCDNumber          *lcdReadout;
    QLabel              *lblAxisLabel;
    QLabel              *lblUnits;
    QPixmap             *pixActive;
    QImage              *imgActive;
    QPixmap             *pixInactive;
    QImage              *imgInactive;
    bool                isMetric = true;
    QGroupBox           *gbShowHide;
    QSpacerItem         *shSpacer;
    QSpacerItem         *noHideSpacer;
    QLabel              *lblOn;

private slots:
    void                handleShowHide();
};

#endif // DROREADOUT_H
