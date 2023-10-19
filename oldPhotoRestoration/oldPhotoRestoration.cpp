#include "oldPhotoRestoration.h"
#include "ImageScene.h"


#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "qgraphicssceneevent.h"

#include <string>
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
    imageItem = new ImageItem(new QGraphicsPixmapItem);
    ui.labelingGraphicView->setMouseTracking(true);
}

oldPhotoRestoration::~oldPhotoRestoration()
{}

void oldPhotoRestoration::mousePressEvent(QMouseEvent* event)
{
    qDebug() << imageItem->posX << "," << imageItem->posY;
}

void oldPhotoRestoration::actionFileClicked() 
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        string fname;
        fname = filename.toStdString();
        if (openImageFile(fname)) {
            // TODO 接分割模型
            Mat newMask(cv::Size(imgSizeX, imgSizeY), CV_8UC1);
            mask = newMask;
            showOriginalImg();
        }
    }
}

/*
    读取图像文件
*/
bool oldPhotoRestoration::openImageFile(string fname)
{
    Mat newImg = cv::imread(fname);
    if (newImg.empty())
    {
        QMessageBox::information(this, "警告", "图片读取失败，请检查图片路径！");
        return VOS_FAIL;
    }
    srcImg = newImg;
    imgSizeX = 400;//newImg.rows;
    imgSizeY = 400;// newImg.cols;
    return VOS_OK;
}

/*
    第一次显示原图，读取图像时运行
*/
void oldPhotoRestoration::showOriginalImg()
{
    Mat imgResize, imgShow;
    Size dsize(400, 400);
    cv::resize(srcImg, imgResize, dsize);
    cvtColor(imgResize, imgShow, COLOR_BGR2RGB); // 图像格式转换

    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,
        imgShow.rows, imgShow.cols * imgShow.channels(), QImage::Format_RGB888);
    imageItem->setPixmap(QPixmap::fromImage(qImg));
    scene->addItem(imageItem);
}
