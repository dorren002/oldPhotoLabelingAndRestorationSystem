#include "mainWindow.h"

#include <iostream>  
#include <fstream>  
#include <string> 
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
        if (muHelper->saveMask(pmHelper.root_dir + '/' + pmHelper.output_dir, pmHelper.img_format)) {
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

    loadUsrCfg();
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

void mainWindow::actionFileClicked()
{
    QString fileroot = QString::fromStdString(pmHelper.root_dir + '/'+ pmHelper.input_dir);
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select Image"),
        fileroot,
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
    子窗口传值：路径结构体
*/
void mainWindow::openUsrCfgDialog() {
    usrDialog = new userConfigDialog();
   
    //主窗口向子窗口传结构体
    connect(this, SIGNAL(sendCfg(QVariant)), usrDialog, SLOT(initUsrCfg(QVariant)));
    //子窗口向主窗口传结构体
    connect(usrDialog, SIGNAL(sendUpdatedCfg(QVariant)), this, SLOT(recvUpdatedCfg(QVariant)));
   
    QVariant variant = QVariant::fromValue(pmHelper);
    emit sendCfg(variant);

    usrDialog->show();
}

void mainWindow::recvUpdatedCfg(QVariant updatedCfg) {
    pmHelper = updatedCfg.value<UserCfg>();
    saveUsrCfg();
}

/*
    用户配置文件加载和保存
*/
void mainWindow::loadUsrCfg() { 
    ifstream inputFile("./configs/configs.txt"); // 打开名为 example.txt 的文件  

    if (inputFile.is_open()) { // 检查文件是否成功打开  
        string line;
        vector<string> lines;
        while (getline(inputFile, line)) { // 逐行读取文件内容  
            lines.push_back(line);
        }
        inputFile.close(); // 关闭文件  
        pmHelper = { atoi(lines[0].c_str()), atoi(lines[1].c_str()), lines[2], lines[3],
            lines[4], lines[5], lines[6] };
    }
    else {
        pmHelper = { 1, 1, "./", "input","mask","save", "png"};
    }
}

void mainWindow::saveUsrCfg() {
    ofstream outfile("./configs/configs.txt");;
    outfile << pmHelper.mask_flag << endl;
    outfile << pmHelper.channel_num << endl;
    outfile << pmHelper.root_dir << endl;
    outfile << pmHelper.input_dir << endl;
    outfile << pmHelper.mask_dir << endl;
    outfile << pmHelper.output_dir << endl;
    outfile << pmHelper.img_format << endl;
    outfile.flush();
    outfile.close();

    // TODO 新建不存在的路径
}