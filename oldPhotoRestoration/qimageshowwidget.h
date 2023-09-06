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
    void paintEvent(QPaintEvent*);     //����

    void Release();

private:
    uchar* winBuf;      //�������buf
    int imgWidth;      //�������ؿ�
    int imgHeight;      //�������ظ�
};

#endif // QIMAGESHOWWIDGET_H
