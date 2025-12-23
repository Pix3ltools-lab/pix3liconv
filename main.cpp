#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("PNG/JPEG/SVG to ICO Converter");
    a.setApplicationVersion("1.0");
    a.setWindowIcon(QIcon(":/icons/resources/icons/app_icon.ico"));

    MainWindow w;
    w.show();
    return a.exec();
}