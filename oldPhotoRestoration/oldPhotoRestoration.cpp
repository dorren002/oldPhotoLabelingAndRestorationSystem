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

    // TODO£º´°¿Ú×ÔÊÊÓ¦
    setWindowTitle("Old");

    // uiÃÀ»¯
    ui.btnBack->setIcon(QIcon(":/oldPhotoRestoration/image/back-fococlipping-standard.png"));
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &oldPhotoRestoration::actionFileClicked);


}

oldPhotoRestoration::~oldPhotoRestoration()
{}

void oldPhotoRestoration::actionFileClicked() 
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("Ñ¡ÔñÍ¼Ïñ"),
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
        QMessageBox::information(this, "¾¯¸æ", "Í¼Æ¬¶ÁÈ¡Ê§°Ü£¬Çë¼ì²éÍ¼Æ¬Â·¾¶£¡");
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

    cvtColor(imgMasked, imgShow, COLOR_BGR2RGB); // Í¼Ïñ¸ñÊ½×ª»»
    QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,
        imgShow.rows, imgShow.cols * imgShow.channels(), QImage::Format_RGB888);

    QGraphicsScene* scene = new QGraphicsScene;//Í¼ÏñÏÔÊ¾
    ui.labelingGraphicView->setScene(scene);
    ui.labelingGraphicView->show();
    scene->addPixmap(QPixmap::fromImage(qImg));
}
