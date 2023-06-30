#include "jcal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Jcal w;
    w.show();
    return a.exec();
}
