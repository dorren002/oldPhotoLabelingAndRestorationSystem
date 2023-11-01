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

    viewSize = ui.labelingGraphicView->height();

    // TODO：窗口自适应
    setWindowTitle("老旧图像标注/修复系统");
    
    // 掩模操作类
    muHelper = new maskUpdater();
    networkManager = new QNetworkAccessManager(this);

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
    connect(ui.actionSelectServer, &QAction::triggered, this, &mainWindow::openServerCfgDialog);

    // 按钮事件
    connect(ui.btnBack, &QToolButton::clicked, this, [=]() {
        if (muHelper->undo()) {
            updateMaskItem();
        }
        else {
            QMessageBox::StandardButton result = QMessageBox::critical(this, "失败", "没有更多步骤可以撤回，请前往用户配置修改最大撤回步数！");
        }
        
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
    connect(ui.btnSaveMask, &QToolButton::clicked, this, [=]() {
        if (muHelper->saveMask(pmHelper.root_dir + '/' + pmHelper.output_dir, pmHelper.img_format)) {
            QMessageBox::StandardButton result = QMessageBox::information(this, "成功", "保存成功！");
        }
        else {
            QMessageBox::StandardButton result = QMessageBox::critical(this, "失败", "请核对工作目录！");
        }
        });
    connect(ui.btnRestore, &QToolButton::clicked, this, [=]() {
        serverRequestMode = 1;
        getServerProcImage();
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
    if (!muHelper->empty() && ui.labelingGraphicView->geometry().contains(QPoint(globalX, globalY))) {
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
    修复图像的显示和比较等
*/
void mainWindow::showRestoredImage() {
    if (muHelper->restoreFlag) {
        QImage qimage;
        mat2QImage(muHelper->getRestoredImage(), &qimage);
        restoredImageItem = new QGraphicsPixmapItem(QPixmap::fromImage(qimage));
        scene->addItem(restoredImageItem);
    }
}

void mainWindow::compareRestoredImage() {
    
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

void mainWindow::image2Mat(QImage& qImage, Mat* mat) {
    if (qImage.isGrayscale()) {
        *mat = cv::Mat(qImage.height(), qImage.width(), CV_8UC1, (void*)qImage.bits(), qImage.bytesPerLine());
    }
    else {
        *mat = cv::Mat(qImage.height(), qImage.width(), CV_8UC3, (void*)qImage.bits(), qImage.bytesPerLine());
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
        tr("选择一张图片"),
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
            serverRequestMode = 0;
            ui.labelingGraphicView->setScaleRatio(muHelper->getRatio(viewSize));
            updateMaskItem();
            getServerProcImage();
        }
    }
}

/*
    网络相关
*/
void mainWindow::getServerProcImage() {
    QUrl url;
    QByteArray imageData;
    if (serverRequestMode==1) {
        // restore
        url = "http://" + serverPath + "/restore";
        Mat postImage;
        muHelper->getMaskedImage(&postImage);
        imageData = cvMatToByteArray(postImage);
    }
    else {
       // detect
        url = "http://" + serverPath + "/detect";
        imageData = cvMatToByteArray(muHelper->getImage());
    }
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpg"); 
    reply = networkManager->post(request, imageData);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));

    if (serverRequestMode == 1) {
        connect(reply, SIGNAL(finished()), this, SLOT(slot_restoreRequestFinished()));
    }
    else {
        connect(reply, SIGNAL(finished()), this, SLOT(slot_detectionRequestFinished()));
    }
}

QByteArray mainWindow::cvMatToByteArray(const cv::Mat& image) {
    // 将cv::Mat转换为QByteArray，这里使用PNG格式作为示例
    Mat postImage;
    cv::cvtColor(image, postImage, COLOR_RGB2BGR);
    cv::imwrite("tmp/userImg.png", postImage);
    QFile file("tmp/userImg.png");
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }
    else {
        qDebug() << "Failed to open image file";
        return QByteArray();
    }
}

void mainWindow::slot_detectionRequestFinished()
{
    int nHttpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if (nHttpCode == 200) {
        QByteArray resultContent = reply->readAll();
        QImage image;
        if (image.loadFromData(resultContent)) {
            cv::Mat temp;
            image2Mat(image, &temp);
            muHelper->updateDetectedMask(&temp);
            updateMaskItem();
        }
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("连接服务器失败"));
        msgBox.setText(tr("请查看服务器配置并确认服务器可用，点击“重试”按钮重试或点击“取消”进入手动标注流程！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(tr("重试"), QMessageBox::AcceptRole);
        msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if (ret == QMessageBox::AcceptRole)
        {
            getServerProcImage();
        }
        else if (ret == QMessageBox::RejectRole)
        {
            updateMaskItem();
        }
    }
}

void mainWindow::slot_restoreRequestFinished()
{
    int nHttpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if (nHttpCode == 200) {
        QByteArray resultContent = reply->readAll();
        QImage image;
        if (image.loadFromData(resultContent)) {
            image.save("tmp/newnewnew.jpg");
            cv::Mat temp;
            image2Mat(image, &temp);
            cv::imwrite("tmp/test.jpg", temp);
            muHelper->updateRestoredImg(&temp);
            showRestoredImage();
        }
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("连接服务器失败"));
        msgBox.setText(tr("请查看服务器配置并确认服务器可用，如确定没有问题请点击“重试”按钮重试！"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.addButton(tr("重试"), QMessageBox::AcceptRole);
        msgBox.addButton(tr("取消"), QMessageBox::RejectRole);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if (ret == QMessageBox::AcceptRole)
        {
            getServerProcImage();
        }
        else if (ret == QMessageBox::RejectRole)
        {
            
        }
    }
}


void mainWindow::upLoadError(QNetworkReply::NetworkError code)
{
    QMessageBox::StandardButton result = QMessageBox::critical(this, "连接服务器失败", "请查看服务器配置并确认服务器可用，进入手动标注流程！");
}

// 批量
void mainWindow::uploadProcess(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << bytesReceived << bytesTotal;
}


/*
    子窗口传值：路径结构体
*/
void mainWindow::openUsrCfgDialog() {
    userConfigDialog* usrDialog = new userConfigDialog();
    usrDialog->setWindowTitle(tr("自定义配置"));
   
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

void mainWindow::openServerCfgDialog() {
    serverConfigDialog* srvrDialog = new serverConfigDialog();
    srvrDialog->setWindowTitle(tr("服务器配置"));

    //主窗口向子窗口传字符串
    connect(this, SIGNAL(sendCfg(QString)), srvrDialog, SLOT(initServerCfg(QString)));
    //子窗口向主窗口传字符串
    connect(srvrDialog, SIGNAL(sendUpdatedCfg(QString)), this, SLOT(recvUpdatedCfg(QString)));
    
    emit sendCfg(serverPath);

    srvrDialog->show();
}

void mainWindow::recvUpdatedCfg(QString updatedCfg) {
    serverPath = updatedCfg;
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
        serverPath = QString::fromStdString(lines[7]);
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
    outfile << serverPath.toLocal8Bit().constData() << endl;
    outfile.flush();
    outfile.close();

    // TODO 新建不存在的路径
}