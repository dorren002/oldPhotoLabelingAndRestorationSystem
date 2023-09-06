#include "qimageshowwidget.h"
#include <opencv2\opencv.hpp>
#include <QPainter>
#include <QDebug>
#include <iostream>

using namespace cv;
using namespace std;

QImageShowWidget::QImageShowWidget(QWidget* parent) : QWidget(parent)
{
    //填充背景色
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);
}

QImageShowWidget::~QImageShowWidget()
{
}

bool QImageShowWidget::LoadImage(const char* imagePath)
{
    //从文件中读取成灰度图像
    Mat img = imread(imagePath);
    if (img.empty())
    {
        fprintf(stderr, "Can not load image %s\n", imagePath);
        return false;
    }

    Release();

    winWidth = rect().width();
    winHeight = rect().height();
    size_t winBufNum = (size_t)winWidth * winHeight * winBandNum;
    winBuf = new uchar[winBufNum];
    memset(winBuf, 255, winBufNum * sizeof(uchar));

    for (int ri = 0; ri < img.rows; ++ri)
    {
        for (int ci = 0; ci < img.cols; ++ci)
        {
            for (int bi = 0; bi < winBandNum; bi++)
            {
                size_t m = (size_t)winWidth * winBandNum * ri + winBandNum * ci + bi;
                size_t n = (size_t)img.cols * winBandNum * ri + winBandNum * ci + bi;
                winBuf[m] = img.data[n];
            }
        }
    }

    update();

    return true;
}

//重新实现paintEvent
void QImageShowWidget::paintEvent(QPaintEvent*)
{
    if (!winBuf)
    {
        return;
    }

    QImage::Format imgFomat = QImage::Format_RGB888;

    QPainter painter(this);
    QImage qImg(winBuf, winWidth, winHeight, winWidth * winBandNum, imgFomat);
    painter.drawPixmap(0, 0, QPixmap::fromImage(qImg));
}

void QImageShowWidget::Release()
{
    if (winBuf)
    {
        delete[] winBuf;
        winBuf = nullptr;
    }
}
