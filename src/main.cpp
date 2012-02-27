#include <QtGui/QApplication>
#include "MainDialog.h"
#include "Settings.h"

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );
 
    Settings::getInstance().initialize();
 
    MainDialog w;
    w.show();
    return a.exec();
}
