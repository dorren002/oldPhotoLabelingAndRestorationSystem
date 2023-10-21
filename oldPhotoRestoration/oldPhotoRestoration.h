#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <string>

#include "ImageItem.h"
#include "ImageScene.h"
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
    ImageScene* scene;//图像显示
    ImageItem* imageItem;
    QGraphicsPixmapItem* maskItem;

    QImage img2QImg(cv::Mat& img);
    void updateImageItem();
    void updateMaskItem();
    bool openImageFile(std::string fname);
    void mousePressEvent(QMouseEvent* event);
    void updateMask();
};
