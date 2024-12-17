#include "MyCOM.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyCOM w;
    w.show();
    return a.exec();
}
