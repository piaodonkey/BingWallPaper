//#include <QApplication>
#include "bing.h"
#include <QCoreApplication>


int main(int argc, char *argv[])
{

    QCoreApplication app(argc,argv);
    Bing bing;
    bing.execute();
    app.exec();
    return 0;
}
