#include "ImageView.h"
#include <QWheelEvent>

ImageView::ImageView(QWidget* parent) :QGraphicsView(parent) {

}

void ImageView::wheelEvent(QWheelEvent* event)
{
    int wheelValue = event->angleDelta().y();
    double ratio = (double)wheelValue / (double)1200 + 1;
    scale(ratio, ratio);
}

void ImageView::setScaleRatio(double ratio) {
    scale(ratio, ratio);
}