#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("MonitoringProdEquipment");
    QCoreApplication::setApplicationName("MonitoringProdEquipment");
    app.setFont(QFont("Segoe UI", 10));

    QFile theme(":/styles/app.qss");
    if (theme.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app.setStyleSheet(QString::fromUtf8(theme.readAll()));
    }

    MainWindow window;
    window.show();
    return app.exec();
}
