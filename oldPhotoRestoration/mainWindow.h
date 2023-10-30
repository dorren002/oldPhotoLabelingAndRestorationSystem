#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <string>
#include <QVariant>

#include "ImageItem.h"
#include "ImageScene.h"
#include "ImageView.h"
#include "maskUpdater.h"
#include "ui_mainWindow.h"

#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "util.h"

#include "userConfigDialog.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void sendCfg(QVariant);//向子窗口传结构体

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private:
    Ui::mainWindowClass ui;
    ImageScene* scene;
    maskUpdater* muHelper;
    userConfigDialog* usrDialog;
    UserCfg pmHelper;

    string runPath;
    string workDir;

    void mat2QImage(cv::Mat& img, QImage* qImage);
    void updateMaskItem();
    bool openImageFile(std::string fname);
    void mousePressEvent(QMouseEvent* event);
    void initSliderValue();
    void setRGBSliderMaxVal(double val);

    void actionFileClicked();
    void openUsrCfgDialog();

    void loadUsrCfg();
    void saveUsrCfg();

private slots:
    void recvUpdatedCfg(QVariant updatedCfg);
};
