#include "oldPhotoRestoration.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Ӧ�ó������
    oldPhotoRestoration w; //���ڶ���
    w.show(); 
    return a.exec(); // Ӧ�ó�����������Ϣѭ��
}
    