#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <string>
#include "ImageItem.h"

#include "ui_oldPhotoRestoration.h"

#define VOS_OK 1;
#define VOS_FAIL 0;

class oldPhotoRestoration : public QMainWindow
{
    Q_OBJECT

public:
    int imgSizeX, imgSizeY;
    cv::Mat srcImg;
    cv::Mat mask;
    oldPhotoRestoration(QWidget *parent = nullptr);
    ~oldPhotoRestoration();
    void actionFileClicked();

private:
    Ui::oldPhotoRestorationClass ui;
    QGraphicsScene* scene;//ÕºœÒœ‘ æ
    ImageItem* imageItem;

    void showOriginalImg();
    bool openImageFile(std::string fname);
    void mousePressEvent(QMouseEvent* event);
};
