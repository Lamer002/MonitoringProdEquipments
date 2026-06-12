#include "mainwindow.h"

#include <QApplication>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
