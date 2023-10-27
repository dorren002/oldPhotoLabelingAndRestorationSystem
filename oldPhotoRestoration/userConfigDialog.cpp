#include "userConfigDialog.h"

userConfigDialog::userConfigDialog() : QDialog () {
	ui.setupUi(this);

    ui.inputDirEdit->setPlaceholderText("input");
    ui.maskDirEdit->setPlaceholderText("mask");
    ui.saveDirEdit->setPlaceholderText("output");

    connect(ui.selectWorkDir, SIGNAL(clicked()), this, SLOT(onFileNameClicked()));
}

userConfigDialog::~userConfigDialog() {};

void userConfigDialog::onFileNameClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this, "选择一个目录", "./", QFileDialog::ShowDirsOnly);
    //若目录路径不为空
    if (!selectedDir.isEmpty())
    {
        //将路径中的斜杠替换为反斜杠
        selectedDir = selectedDir.replace(QRegExp("\\"), "/");
        //显示选择的目录路径
        workDir = selectedDir.toLocal8Bit().constData();
        ui.workDirEdit->clear();
        ui.workDirEdit->setText(selectedDir);
    }
}

void userConfigDialog::onOKClicked() {
    accept();
}