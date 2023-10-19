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

    // TODO����������Ӧ
    setWindowTitle("Old");

    // ui����
    ui.btnBack->setIcon(QIcon(":/oldPhotoRestoration/image/back-fococlipping-standard.png"));
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &oldPhotoRestoration::actionFileClicked);

    // ͼ����ʾ
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
        tr("ѡ��ͼ��"),
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
            // TODO �ӷָ�ģ��
            Mat newMask(cv::Size(imgSizeX, imgSizeY), CV_8UC1);
            mask = newMask;
            showOriginalImg();
        }
    }
}

/*
    ��ȡͼ���ļ�
*/
bool oldPhotoRestoration::openImageFile(string fname)
{
    Mat newImg = cv::imread(fname);
    if (newImg.empty())
    {
        QMessageBox::information(this, "����", "ͼƬ��ȡʧ�ܣ�����ͼƬ·����");
        return VOS_FAIL;
    }
    srcImg = newImg;
    imgSizeX = 400;//newImg.rows;
    imgSizeY = 400;// newImg.cols;
    return VOS_OK;
}

/*
    ��һ����ʾԭͼ����ȡͼ��ʱ����
*/
void oldPhotoRestoration::showOriginalImg()
{
    Mat imgResize, imgShow;
    Size dsize(400, 400);
    cv::resize(srcImg, imgResize, dsize);
    cvtColor(imgResize, imgShow, COLOR_BGR2RGB); // ͼ���ʽת��

    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,
        imgShow.rows, imgShow.cols * imgShow.channels(), QImage::Format_RGB888);
    imageItem->setPixmap(QPixmap::fromImage(qImg));
    scene->addItem(imageItem);
}
