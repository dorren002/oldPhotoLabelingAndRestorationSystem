#include "userConfigDialog.h"

userConfigDialog::userConfigDialog() : QDialog () {
	ui.setupUi(this);

    editEventMapper = new QSignalMapper();

    connect(ui.selectWorkDir, SIGNAL(clicked()), this, SLOT(onWorkDirClicked()));

    connect(ui.selectInputDir, SIGNAL(clicked()), editEventMapper, SLOT(map()));
    editEventMapper->setMapping(ui.selectInputDir, 0);
    connect(ui.selectMaskDir, SIGNAL(clicked()), editEventMapper, SLOT(map()));
    editEventMapper->setMapping(ui.selectMaskDir, 1);
    connect(ui.selectSaveDir, SIGNAL(clicked()), editEventMapper, SLOT(map()));
    editEventMapper->setMapping(ui.selectSaveDir, 2);

    connect(editEventMapper, SIGNAL(mapped(int)), this, SLOT(onSubDirClicked(int)));

    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(onOKClicked()));
}

userConfigDialog::~userConfigDialog() {};

void userConfigDialog::onWorkDirClicked() {
    QString selectedDir = QFileDialog::getExistingDirectory(this, "选择一个目录", "./", QFileDialog::ShowDirsOnly);
    //若目录路径不为空
    if (!selectedDir.isEmpty())
    {
        //将路径中的斜杠替换为反斜杠
        selectedDir = selectedDir.replace(QRegExp("\\"), "/");
        //显示选择的目录路径
        mWorkDir = selectedDir.toLocal8Bit().constData();
        ui.workDirEdit->clear();
        ui.workDirEdit->setText(selectedDir);
    }
}


void userConfigDialog::onSubDirClicked(int idx)
{
    QString selectedDir;
    selectedDir = QFileDialog::getExistingDirectory(this, "选择一个目录", QString::fromStdString(mWorkDir), QFileDialog::ShowDirsOnly);
    
    //若目录路径不为空
    if (!selectedDir.isEmpty())
    {
        //将路径中的斜杠替换为反斜杠
        selectedDir = selectedDir.replace(QRegExp("\\"), "/");
        
        // 当选择的子目录在工作目录下时只显示子目录
        if (!mWorkDir.empty()) {
            string temp = selectedDir.toLocal8Bit().constData();
            string root, subdir;

            if (strcmp(mWorkDir.c_str(), temp.c_str()) == 0) {
                QMessageBox::information(this, "警告", "工作路径和子路径不能是相同的路径！");
                return;
            }
           
            for (int i = temp.size() - 1; i >= 0; i--) {
                if (temp.at(i) == '/') {
                    root = temp.substr(0, i);
                    subdir = temp.substr(i + 1, temp.size() - i);
                    break;
                }
            }

            if (strcmp(mWorkDir.c_str(), root.c_str()) == 0) {
                selectedDir = QString::fromStdString(subdir);
            }
        }
      
        //显示选择的子目录路径
        if (idx == 0) {
            ui.inputDirEdit->clear();
            ui.inputDirEdit->setText(selectedDir);
        }
        else if (idx == 1) {
            ui.maskDirEdit->clear();
            ui.maskDirEdit->setText(selectedDir);
        }
        else {
            ui.saveDirEdit->clear();
            ui.saveDirEdit->setText(selectedDir);
        }
        
    }
}

void userConfigDialog::initUsrCfg(QVariant initialCfg) {
    UserCfg cfg;
    cfg = initialCfg.value<UserCfg>();
    
    ui.workDirEdit->setText(QString::fromStdString(cfg.root_dir));
    ui.inputDirEdit->setText(QString::fromStdString(cfg.input_dir));
    ui.maskDirEdit->setText(QString::fromStdString(cfg.mask_dir));
    ui.saveDirEdit->setText(QString::fromStdString(cfg.output_dir));

    ui.channelNum->setCurrentIndex(cfg.channel_num == 1 ? 0 : 1);
    ui.imgFormat->setCurrentIndex(getImgFormatIdx(cfg.img_format));
    ui.maskFlag->setCurrentIndex(cfg.mask_flag);

    mWorkDir = cfg.root_dir;
    mInputDir = cfg.input_dir;
    mMaskDir = cfg.mask_dir;
    mSaveDir = cfg.output_dir;
}

void userConfigDialog::onOKClicked() {
    UserCfg cfg;

    cfg.root_dir = mWorkDir;
    cfg.input_dir = mInputDir;
    cfg.mask_dir = mMaskDir;
    cfg.output_dir = mSaveDir;

    cfg.channel_num = ui.channelNum->currentIndex() == 0 ? 1 : 3;
    cfg.mask_flag = ui.channelNum->currentIndex();
    cfg.img_format = getImgFormatStr(ui.imgFormat->currentIndex());

    QVariant variant;
    variant.setValue(cfg);

    emit sendUpdatedCfg(variant);

    this->close();
}

int userConfigDialog::getImgFormatIdx(string formatStr) {
    if (formatStr == "png" || formatStr == "PNG") {
        return 0;
    }
    else if (formatStr == "jpg" || formatStr == "JPG") {
        return 1;
    }
    else {
        return 2;
    }
}

string userConfigDialog::getImgFormatStr(int idx) {
    if (idx == 0) {
        return "png";
    }
    else if (idx == 1) {
        return "jpg";
    }
    else {
        return "gif";
    }
}