#include "ImageView.h"
#include <QWheelEvent>
#include <QMouseEvent>

#include "qdebug.h"

ImageView::ImageView(QWidget* parent) :QGraphicsView(parent) {
    isMousePressed = false;
    cursorType = Qt::ArrowCursor;
}

void ImageView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    if (cursorType == Qt::OpenHandCursor) {
        setCursor(QCursor(Qt::ClosedHandCursor));
        // 记录鼠标按下时的中心点坐标
        centerAnchor = mapToScene(event->pos()) - event->pos() + QPointF(width() / 2, height() / 2);
        // 记录当前鼠标在view中的位置，用来在mouseMove事件中计算偏移
        // 此处不将view坐标转换成scene坐标的原因是优化性能，在move的过程中会产生抖动
        posAnchor = event->pos();
        isMousePressed = true;
    }
    // event->ignore();
}

void ImageView::mouseMoveEvent(QMouseEvent* event)
{
    if (isMousePressed && cursorType == Qt::OpenHandCursor) {
        QPointF offsetPos = event->pos() - posAnchor;
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        centerOn(centerAnchor - offsetPos);
        QGraphicsView::repaint();
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(QCursor(cursorType));
    isMousePressed = false;
}

void ImageView::wheelEvent(QWheelEvent* event)
{
    int wheelValue = event->angleDelta().y();
    double ratio = (double)wheelValue / (double)1200 + 1;
    scale(ratio, ratio);
}

void ImageView::setCenter(int width, int height) {
    centerOn(-width/2, -height/2);
    update();
}

void ImageView::setScaleRatio(double ratio) {
    resetTransform();
    scale(ratio, ratio);
}

void ImageView::setCursorType(Qt::CursorShape ctype) {
    cursorType = ctype;
    setCursor(QCursor(cursorType));
}
