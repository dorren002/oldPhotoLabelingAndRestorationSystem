#pragma once

#include <QtWidgets/QWidget>
#include "ui_labelingWidget.h"

class labelingWidget : public QWidget
{
    Q_OBJECT

public:
    labelingWidget(QWidget* parent = nullptr);
    ~labelingWidget();
    void openFile();

private:
    Ui::labelingWidgetClass ui;
};
