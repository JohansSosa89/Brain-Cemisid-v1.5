#include"mainwindow.h"
#include <QApplication>
#include<QDesktopWidget>


int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    int x, y;
    MainWindow w;
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    x = (screenWidth - 700) / 2;
    y = (screenHeight - 700) / 2;
    w.move(x, y);
    w.show();
    return a.exec();
}
