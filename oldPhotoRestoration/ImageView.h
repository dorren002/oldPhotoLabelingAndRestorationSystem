#pragma once
#include <qgraphicsview.h>
class ImageView :
    public QGraphicsView
{
public:
    ImageView(QWidget *parent = 0);
private:
    void ImageView::wheelEvent(QWheelEvent* event);
};

