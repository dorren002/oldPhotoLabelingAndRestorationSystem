#pragma once
#include <string>

#include "util.h"
#include "qdebug.h"
#include "qdialog.h"
#include "qfiledialog.h"
#include "qsignalmapper.h"
#include "qmessagebox.h"

#include "ui_userConfigDialog.h"

struct UserCfg
{
	int mask_flag;
	int channel_num;
	string root_dir;
	string input_dir;
	string mask_dir;
	string output_dir;
	string img_format;
	// networkCfg serverCfg;
};

using namespace std;
Q_DECLARE_METATYPE(UserCfg);

class userConfigDialog :
    public QDialog
{
	Q_OBJECT
signals:
	void sendInitialCfg(QVariant);//子窗口向主窗口传值
	void sendUpdatedCfg(QVariant);//子窗口向主窗口传结构体数据

public:
	string mWorkDir = "./";
	string mInputDir = "input";
	string mMaskDir = "mask";
	string mSaveDir = "output";

	userConfigDialog();
	~userConfigDialog();
	

public slots:
	void initUsrCfg(QVariant initialCfg);


private:
	Ui::Dialog ui;
	QSignalMapper* editEventMapper;
	int getImgFormatIdx(string formatStr);
	string getImgFormatStr(int idx);

private slots:
	void onWorkDirClicked();
	void onSubDirClicked(int i);

	void onOKClicked();
};

