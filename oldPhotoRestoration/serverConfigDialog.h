#pragma once
#include "util.h"

#include "qdialog.h"
#include "qregexp.h"
#include "qdebug.h"
#include "qmessagebox.h"

#include "ui_serverConfigDialog.h"


class serverConfigDialog :
    public QDialog
{
    Q_OBJECT
signals:
    void sendInitialCfg(QString);//子窗口向主窗口传值
    void sendUpdatedCfg(QString);//子窗口向主窗口传结构体数据

public:
    serverConfigDialog();

public slots:
    void initServerCfg(QString ip);

private:
    Ui::Server_Dialog ui;

private slots:
    void onOKClicked();
};

