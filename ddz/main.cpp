#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("斗地主");
    QObject::connect(&w, &MainWindow::terminate, &a, &QApplication::quit);
    w.show();
    return a.exec();
}
