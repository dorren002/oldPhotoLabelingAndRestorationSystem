#pragma once
#include <qgraphicsview.h>
class ImageView :
    public QGraphicsView
{
public:
    ImageView(QWidget *parent = 0);
    void setScaleRatio(double ratio);
private:
    void wheelEvent(QWheelEvent* event);
};

