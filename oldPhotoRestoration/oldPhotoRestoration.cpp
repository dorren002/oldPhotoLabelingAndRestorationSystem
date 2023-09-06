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
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &oldPhotoRestoration::openFile);


}

void oldPhotoRestoration::openFile() 
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
        Mat srcImg = cv::imread(fname);
        if (srcImg.empty())
        {
            QMessageBox::information(this, "����", "ͼƬ��ȡʧ�ܣ�����ͼƬ·����");
            return;
        }
        Mat imgShow;
        cvtColor(srcImg, imgShow, COLOR_BGR2RGB); // ͼ���ʽת��
        QImage qImg = QImage((unsigned char*)(imgShow.data), imgShow.cols,
            imgShow.rows, imgShow.cols * imgShow.channels(), QImage::Format_RGB888);

        QGraphicsScene* scene = new QGraphicsScene;//ͼ����ʾ
        ui.labelingGraphicView->setScene(scene);
        ui.labelingGraphicView->show();
        scene->addPixmap(QPixmap::fromImage(qImg));
    }
}

oldPhotoRestoration::~oldPhotoRestoration()
{}
