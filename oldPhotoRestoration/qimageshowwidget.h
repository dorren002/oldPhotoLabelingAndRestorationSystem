#pragma once
#ifndef QIMAGESHOWWIDGET_H
#define QIMAGESHOWWIDGET_H

#include <QWidget>

class QImageShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QImageShowWidget(QWidget* parent = nullptr);
    ~QImageShowWidget();

    bool LoadImage(const char* imagePath);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*);     //绘制

    void Release();

private:
    uchar* winBuf;      //窗口填充buf
    int imgWidth;      //窗口像素宽
    int imgHeight;      //窗口像素高
};

#endif // QIMAGESHOWWIDGET_H
