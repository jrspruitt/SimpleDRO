#ifndef FUNCTIONKEYPAD_H
#define FUNCTIONKEYPAD_H

#include "drosettings.h"
#include "axis.h"
#include "dronumkeypad.h"
#include <QObject>
#include <QWidget>
#include <QButtonGroup>

#define FUNC_SET_OFFSET    "Set Location"
#define FUNC_GET_OFFSET    "Get Location"
#define FUNC_HALF           "1/2"
#define FUNC_BOLT_CIRCLE    "Bolt Circle"



class DROFunctions : public QWidget
{
    Q_OBJECT

 enum functionList {
     func_default = -1,
     func_half,
     func_set_offset,
     func_get_offset,
 };

public:
    explicit DROFunctions(DROSettings *settings, QHash<QString, Axis *> *axisReadouts, QWidget *parent = nullptr);
    void                                createUi();

private:
    DRONumKeypad                        *keypad;
    DROSettings                         *settings;
    QHash<QString, Axis *>              *axisReadouts;
    QStringList                         funcNames = {"Half", "Set Offset", "Get Offset"};
    int                                 _curFunction = -1;
    int                                 _curIndex = 0;
    int                                 _curOffsetSlot = 0;
    QList<QHash<QString, double>>       *_funcTempValues;
    QButtonGroup                        *bgrpFunctions;

signals:
    void                                message(QString msg);

public slots:
    void                                handleStartFunc(int id);
    void                                handleStopFunc(QString msg="");

    void                                handleConfigFunc();
    void                                handleFuncDefault();
    void                                handleFuncHalf();
    void                                handleFuncSetOffset();
    void                                handleFuncGetOffset();
};

#endif // FUNCTIONKEYPAD_H
