#pragma once
#include <string>
#include "qdialog.h"
#include "qfiledialog.h"

#include "ui_userConfigDialog.h"

class userConfigDialog :
    public QDialog
{
	Q_OBJECT
public:
	std::string workDir;
	std::string inputDir;
	std::string maskDir;
	std::string saveDir;

	userConfigDialog();
	~userConfigDialog();
	Ui::Dialog ui;

private slots:
	void onFileNameClicked();
	void onOKClicked();
};

