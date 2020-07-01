#ifndef FUNCTIONKEYPAD_H
#define FUNCTIONKEYPAD_H

#include "drosettings.h"
#include "axis.h"
#include <QObject>
#include <QWidget>
#include <QButtonGroup>

#define FUNC_LOCATION       "Locations"
#define FUNC_HALF           "1/2"
#define FUNC_BOLT_CIRCLE    "Bolt Circle"



class DROFunctions : public QWidget
{
    Q_OBJECT

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

public:
    explicit DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent = nullptr);
    void                        createUi();
    void                        setSelected(bool selected);
    bool                        getSelected();
    void                        enterValue(double value);

private:
    DROSettings                 *settings;
    QHash<QString, Axis *>      *axisReadouts;
    QStringList                 funcNames;
    bool                        _selected = false;
    QString                     _curFunction = "";
    int                         _curIndex = 0;
    QHash<QString, funcData *>  *funcList;
    double                      _curInputValue;
    funcData*                   getFuncData(int listCount, funcHandler func, QStringList msgList);
    QButtonGroup                *bgrpFunctions;

signals:
    void                        message(QString msg);

public slots:
    void                        handleFunctionButtons(int id);
    void                        handleFunctionConfig();
    void                        handleLocations();
    void                        handleHalf();
    void                        handleReset();
};

#endif // FUNCTIONKEYPAD_H
