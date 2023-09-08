#include "oldPhotoRestoration.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

#include <string>

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


}

oldPhotoRestoration::~oldPhotoRestoration()
{}

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
        if (openFile(fname)) {
            Mat newMask(cv::Size(imgSizeX, imgSizeY), CV_8UC1);
            mask = newMask;
            showCvImg();
        }
    }
}

bool oldPhotoRestoration::openFile(string fname)
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

void oldPhotoRestoration::showCvImg()
{
    Mat imgResize, imgMasked, imgShow;
    Size dsize(400, 400);
    cv::resize(srcImg, imgResize, dsize);

    cvtColor(imgMasked, imgShow, COLOR_BGR2RGB); // ͼ���ʽת��
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,
        imgShow.rows, imgShow.cols * imgShow.channels(), QImage::Format_RGB888);

    QGraphicsScene* scene = new QGraphicsScene;//ͼ����ʾ
    ui.labelingGraphicView->setScene(scene);
    ui.labelingGraphicView->show();
    scene->addPixmap(QPixmap::fromImage(qImg));
}
