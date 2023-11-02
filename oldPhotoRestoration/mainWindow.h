#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2\opencv.hpp>
#include <string>
#include <QVariant>

#include "ImageScene.h"
#include "ImageView.h"
#include "maskUpdater.h"
#include "ui_mainWindow.h"

#include "qnetworkaccessmanager.h"
#include "qhttpmultipart.h"
#include "qnetworkreply.h"

#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qtextcodec.h"
#include "qdebug.h"
#include "util.h"

#include "userConfigDialog.h"
#include "serverConfigDialog.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void sendCfg(QVariant);//向子窗口传结构体
    void sendCfg(QString);

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private:
    Ui::mainWindowClass ui;
    ImageScene* scene;
    QGraphicsPixmapItem* restoredImageItem;
    maskUpdater* muHelper;

    UserCfg pmHelper;
    QString serverPath;

    QNetworkAccessManager* networkManager;
    QNetworkReply* reply;

    int viewSize;
    int serverRequestMode;

    bool useDetectionModel;
    bool selectionCursor;

    string savefname;

    void image2Mat(QImage& qImage, cv::Mat* mat);
    void mat2QImage(cv::Mat& mat, QImage* qImage);
    void updateMaskItem();
    void mousePressEvent(QMouseEvent* event);
    void initSliderValue();
    void setRGBSliderMaxVal(double val);

    bool openImageFile(std::string fname);
    void showRestoredImage();
    void compareRestoredImage();

    void actionFileClicked();
    void openUsrCfgDialog();
    void openServerCfgDialog();

    void loadUsrCfg();
    void saveUsrCfg();

    void getServerProcImage();
    QByteArray cvMatToByteArray(const cv::Mat& image);

private slots:
    void recvUpdatedCfg(QVariant updatedCfg);
    void recvUpdatedCfg(QString updatedCfg);

    void slot_detectionRequestFinished();
    void slot_restoreRequestFinished();
    void upLoadError(QNetworkReply::NetworkError code);
    void uploadProcess(qint64 bytesReceived, qint64 bytesTotal);
};
