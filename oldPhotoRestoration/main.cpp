#include "oldPhotoRestoration.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 应用程序对象
    oldPhotoRestoration w; //窗口对象
    w.show(); 

    //设置中文字体  
    a.setFont(QFont("Microsoft Yahei", 9));

    //设置中文编码
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec* codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
#endif


    return a.exec(); // 应用程序对象进入消息循环
}
    