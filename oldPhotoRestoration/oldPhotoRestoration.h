#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_oldPhotoRestoration.h"

class oldPhotoRestoration : public QMainWindow
{
    Q_OBJECT

public:
    oldPhotoRestoration(QWidget *parent = nullptr);
    ~oldPhotoRestoration();
    void openFile();

private:
    Ui::oldPhotoRestorationClass ui;
};
