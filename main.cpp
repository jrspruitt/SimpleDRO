#include "simpledro.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QString skinName("default");
    QApplication a(argc, argv);
    if ( argc > 1 )
        skinName = argv[1];

    SimpleDRO w(skinName);
    w.show();

    return a.exec();
}
