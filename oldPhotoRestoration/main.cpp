#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 应用程序对象
    mainWindow w; //窗口对象
    w.show(); 


    return a.exec(); // 应用程序对象进入消息循环
}
    