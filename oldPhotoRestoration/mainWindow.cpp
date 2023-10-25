﻿#include "mainWindow.h"

#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "util.h"

#include <QMouseEvent>
#include <QGraphicsPixmapItem>

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

mainWindow::mainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // TODO：窗口自适应
    setWindowTitle("Old");

    // ui美化
    ui.btnBack->setIcon(QIcon(":/mainWindow/image/back-fococlipping-standard.png"));
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &mainWindow::actionFileClicked);

    connect(ui.btnReset, SIGNAL(clicked()), this, SLOT(&mainWindow::resetMask));

    // 图像显示
    scene = new ImageScene();
    ui.labelingGraphicView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui.labelingGraphicView->setScene(scene);
    ui.labelingGraphicView->setMouseTracking(true);

    // 掩模操作类
    muHelper = new maskUpdater();
}

mainWindow::~mainWindow()
{}

void mainWindow::mousePressEvent(QMouseEvent* event)
{
    int globalX = event->x();
    int globalY = event->y();
    if (ui.labelingGraphicView->geometry().contains(QPoint(globalX, globalY))) {
        if (event->button() & Qt::LeftButton)
        {
            // 左键选择
            muHelper->updateMask(scene->cursorX, scene->cursorY, true);
            updateMaskItem();
        }
        else if (event->button() & Qt::RightButton)
        {
            // 右键取消
            muHelper->updateMask(scene->cursorX, scene->cursorY, false);
            updateMaskItem();
        }
    }
}

void mainWindow::actionFileClicked() 
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select Image"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        scene->backgroundImgPath = filename;
        string fname = filename.toLocal8Bit().constData();
        if (openImageFile(fname)) {
            // TODO 接分割模型
            updateMaskItem();
        }
    }
}

/*
    读取图像文件
*/
bool mainWindow::openImageFile(string fname)
{
    if (muHelper->updateSrcImg(fname)) {
        return VOS_OK;
    }
    else {
        QMessageBox::information(this, "警告", "图片读取失败，请检查图片路径！");
        return VOS_FAIL;
    }
}

void mainWindow::mat2QImage(Mat& mat, QImage* qImage)
{
    if (mat.channels() == 1) {
        *qImage = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols, QImage::Format_Grayscale8);
    }
    else {
        *qImage = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * mat.channels(), QImage::Format_RGB888);
    }
}



void mainWindow::updateMaskItem()
{
    QImage qimage;
    Mat mask;
    muHelper->getMask(&mask);
    mat2QImage(mask, &qimage);
    scene->updateForeImg(qimage);
}

void mainWindow::resetMask() {
    muHelper->resetMask();
    updateMaskItem();
}