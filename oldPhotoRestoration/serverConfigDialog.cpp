#include "serverConfigDialog.h"
#include <QRegExpValidator>

serverConfigDialog::serverConfigDialog() : QDialog() {
    ui.setupUi(this);

    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator* pReg = new QRegExpValidator(rx, this);

    ui.ipEdit->setValidator(pReg);
   
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(onOKClicked()));
}

void serverConfigDialog::initServerCfg(QString ip) {
    QStringList list1 = ip.split(':');
    ui.ipEdit->setText(list1[0]);
    ui.portEdit->setText(list1[1]);
}

void serverConfigDialog::onOKClicked() {
    if (ui.ipEdit->text().isEmpty()) {
        QMessageBox::information(this, "警告", "请输入正确的服务器地址！");
    }
    QString totalIP;
    totalIP = ui.ipEdit->text();
    totalIP += ":";
    if (ui.portEdit->text().isEmpty()) {
        totalIP += "80";
    } else {
        totalIP += ui.portEdit->text();
    }
    
    emit sendUpdatedCfg(totalIP);
    this->close();
}