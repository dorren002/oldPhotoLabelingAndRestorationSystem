#include "oldPhotoRestoration.h"

#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"

#include <QMouseEvent>
#include <QGraphicsPixmapItem>

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

oldPhotoRestoration::oldPhotoRestoration(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // TODO：窗口自适应
    setWindowTitle("Old");

    // ui美化
    ui.btnBack->setIcon(QIcon(":/oldPhotoRestoration/image/back-fococlipping-standard.png"));
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &oldPhotoRestoration::actionFileClicked);

    // 图像显示
    scene = new ImageScene();
    ui.labelingGraphicView->setScene(scene);
    ui.labelingGraphicView->setMouseTracking(true);
}

oldPhotoRestoration::~oldPhotoRestoration()
{}

void oldPhotoRestoration::mousePressEvent(QMouseEvent* event)
{
    int globalX = event->x();
    int globalY = event->y();
    if (ui.labelingGraphicView->geometry().contains(QPoint(globalX, globalY))) {
        if (event->button() & Qt::LeftButton)
        {
            // 左键选择

        }
        else if (event->button() & Qt::RightButton)
        {
            // 右键取消

        }
    }
}

void oldPhotoRestoration::actionFileClicked() 
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
        string fname = filename.toLocal8Bit().constData();
        if (openImageFile(fname)) {
            // TODO 接分割模型
            mask = Mat::ones(cv::Size(imgSizeX, imgSizeY), CV_8UC1); ;
            updateImageItem();
            updateMaskItem();
        }
    }
}

/*
    读取图像文件
*/
bool oldPhotoRestoration::openImageFile(string fname)
{
    Mat newImg = imread(fname);
    if (newImg.empty())
    {
        QMessageBox::information(this, "警告", "图片读取失败，请检查图片路径！");
        return VOS_FAIL;
    }
    imgSizeX = 400;// newImg.rows;
    imgSizeY = 400;// newImg.cols;

    Size dsize(imgSizeX, imgSizeY);
    Mat resizeImg(imgSizeX, imgSizeY, 3);
    cv::resize(newImg, resizeImg, dsize);
    cvtColor(resizeImg, srcImg, COLOR_BGR2RGB);
    return VOS_OK;
}

void oldPhotoRestoration::updateMask()
{

}

QImage oldPhotoRestoration::img2QImg(Mat& img)
{
    QImage qImg = QImage((unsigned char*)(srcImg.data), imgSizeY,
        imgSizeX, imgSizeY * srcImg.channels(), QImage::Format_RGB888);
    return qImg;
}

void oldPhotoRestoration::updateImageItem()
{
    imageItem->setPixmap(QPixmap::fromImage(img2QImg(srcImg)));
}

void oldPhotoRestoration::updateMaskItem()
{
    maskItem->setPixmap(QPixmap::fromImage(img2QImg(mask)));
}