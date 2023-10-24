#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <string>

#include "ImageItem.h"
#include "ImageScene.h"
#include "ImageView.h"
#include "maskUpdater.h"
#include "ui_mainWindow.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();
    void actionFileClicked();
    void resetMask();

private:
    Ui::mainWindowClass ui;
    ImageScene* scene;
    maskUpdater* muHelper;

    void mat2QImage(cv::Mat& img, QImage* qImage);;
    void updateMaskItem();
    bool openImageFile(std::string fname);
    void mousePressEvent(QMouseEvent* event);
};
