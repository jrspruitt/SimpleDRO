#ifndef FUNCTIONKEYPAD_H
#define FUNCTIONKEYPAD_H

#include "drosettings.h"
#include "axis.h"
#include <QObject>
#include <QWidget>

#define FUNC_LOCATION       0
#define FUNC_HALF           1
#define FUNC_BOLT_CIRCLE    2

class DROFunctions : public QWidget
{
    Q_OBJECT
public:
    explicit DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent = nullptr);
    void                        createUi();
    void                        setSelected(bool selected);
    bool                        getSelected();
    void                        enterValue(double value);

private:
    DROSettings                 *settings;
    QHash<QString, Axis *>      *axisReadouts;
    QStringList                 getSelectedAxes();
    bool                        _selected = false;
    int                         _curFunction = -1;
signals:
    void                        message(QString msg);

public slots:
    void                        handleLocations();
};

#endif // FUNCTIONKEYPAD_H
