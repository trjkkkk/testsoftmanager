#include "widget.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();


    Form f;

    f.show();

    return a.exec();
}
