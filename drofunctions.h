#ifndef FUNCTIONKEYPAD_H
#define FUNCTIONKEYPAD_H

#include "drosettings.h"
#include "axis.h"
#include "dronumkeypad.h"
#include <QObject>
#include <QWidget>
#include <QButtonGroup>

#define FUNC_SETLOCATION    "Set Location"
#define FUNC_GETLOCATION    "Get Location"
#define FUNC_HALF           "1/2"
#define FUNC_BOLT_CIRCLE    "Bolt Circle"



class DROFunctions : public QWidget
{
    Q_OBJECT
/*
typedef QHash<QString, QList<double>*> valuelist_t;
typedef void (DROFunctions::*funcHandler)(void);
struct funcData {
    const QString       name;
    funcHandler         func;
    const int           argc;
    valuelist_t         argv;
    const QStringList   msglist;
    int                 argIndex;
};
*/
 enum keyInput {
     select,
     enter,
 };

 enum functionList {
     none = -1,
     func_set_loc,
     func_get_loc,
     func_half,
     func_bolt_circle,
 };

public:
    explicit DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent = nullptr);
    void                        createUi();
    void                        setSelected(bool selected);
    bool                        getSelected();
    void                        enterValue(double value);

private:
    DRONumKeypad                *keypad;
    DROSettings                 *settings;
    QHash<QString, Axis *>      *axisReadouts;
    QStringList                 funcNames;
    bool                        _selected = false;
    int                         _curFunction = -1;
    int                         _curIndex = 0;
    int                         _nextInput = 0;
    //QHash<int, funcData *>      *funcList;
    QHash<QString, double>      *_funcHalfValues;
    double                      _curInputValue;
    //funcData*                   getFuncData(QString name, int listCount, funcHandler func, QStringList msgList);
    QButtonGroup                *bgrpFunctions;

signals:
    void                        message(QString msg);

public slots:
    void                        handleFunctionButtons(int id);
    void                        handleFunction();
    void                        handleLocations();
    void                        handleFuncNone();
    void                        handleFuncHalf();
    void                        handleKeyPressEnter(QString value);
    void                        handleReset();
    //void                                handleAxisSelect(QString axisName);
};

#endif // FUNCTIONKEYPAD_H
