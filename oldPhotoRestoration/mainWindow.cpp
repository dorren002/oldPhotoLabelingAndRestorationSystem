#include "mainWindow.h"

#include <QMouseEvent>
#include <QGraphicsPixmapItem>

using namespace cv;
using namespace std;

mainWindow::mainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // TODO：窗口自适应
    setWindowTitle("老旧图像标注/修复系统");
    workDir = "F:/";
    
    // 掩模操作类
    muHelper = new maskUpdater();

    setRGBSliderMaxVal(1000);
    ui.hsvSlider->setMaxVal(1);
    ui.hsvSlider->setStep(0.01);

    initSliderValue();

    // ui美化
    ui.btnBack->setIcon(QIcon(":/mainWindow/image/back.png"));
    ui.btnRedo->setIcon(QIcon(":/mainWindow/image/redo.png"));
    ui.btnCompare->setIcon(QIcon(":/mainWindow/image/compare.png"));
    ui.btnAddSelection->setIcon(QIcon(":/mainWindow/image/add.png"));
    ui.btnDelSelection->setIcon(QIcon(":/mainWindow/image/del.png"));
    
    // 事件绑定
    connect(ui.actionSelectRootPath, &QAction::triggered, this, &mainWindow::actionFileClicked);
    connect(ui.actionUserConfig, &QAction::triggered, this, &mainWindow::openUsrCfgDialog);

    // 按钮事件
    connect(ui.btnBack, &QToolButton::clicked, this, [=]() {
        muHelper->undo();
        updateMaskItem();
        });
    connect(ui.btnReset, &QToolButton::clicked, this, [=]() {
        muHelper->resetMask();
        updateMaskItem();
        });
    connect(ui.btnSelectRGB, &QToolButton::clicked, this, [=]() {
        muHelper->updateRGBth(ui.rgbSlider->getValue());
        });
    connect(ui.btnCancelRGB, &QToolButton::clicked, this, [=]() {
        ui.rgbSlider->setValue(muHelper->getRGBth());
        });
    connect(ui.btnSelectHSV, &QToolButton::clicked, this, [=]() {
        muHelper->updateHSVth(ui.hsvSlider->getValue());
        });
    connect(ui.btnCancelHSV, &QToolButton::clicked, this, [=]() {
        ui.hsvSlider->setValue(muHelper->getHSVth()); 
        });
    connect(ui.btnErode, &QToolButton::clicked, this, [=]() {
        muHelper->maskErode();
        updateMaskItem();
        });
    connect(ui.btnDilate, &QToolButton::clicked, this, [=]() {
        muHelper->maskDilate(); 
        updateMaskItem();
        });
    connect(ui.btnSave, &QToolButton::clicked, this, [=]() {
        if (muHelper->saveMask(workDir)) {
            QMessageBox::StandardButton result = QMessageBox::information(this, "成功", "保存成功！");
        }
        else {
            QMessageBox::StandardButton result = QMessageBox::critical(this, "失败", "请核对工作目录！");
        }
        });
    
    // 图像显示
    scene = new ImageScene();
    ui.labelingGraphicView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui.labelingGraphicView->setScene(scene);
    ui.labelingGraphicView->setMouseTracking(true);
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

/*
    utils
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
    Mat mask = muHelper->getMask();
    mask *= 255;
    mat2QImage(mask, &qimage);
    scene->updateForeImg(qimage);
}


void mainWindow::setRGBSliderMaxVal(double val) {
    ui.rgbSlider->setMaxVal(val);
}

void mainWindow::initSliderValue() {
    ui.rgbSlider->setValue(muHelper->getRGBth());
    ui.hsvSlider->setValue(muHelper->getHSVth());
}

/*
    SLOTS
*/
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

void mainWindow::openUsrCfgDialog() {
    usrDialog = new userConfigDialog();
    //connect(usrDialog->ui.okButton, SIGNAL(clicked()), this, SLOT(showImage()));//连接ok按钮和槽函数
    usrDialog->show();
}