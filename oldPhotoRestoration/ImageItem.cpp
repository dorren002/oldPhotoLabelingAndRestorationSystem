#include "ImageItem.h"
#include <QGraphicsSceneMouseEvent>

#include "qdebug.h"

ImageItem::ImageItem(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
}

ImageItem::ImageItem(const QPixmap& pixmap, QGraphicsItem* parent) : QGraphicsPixmapItem(pixmap, parent)
{
}

void ImageItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    posX = event->scenePos().x();
    posY = event->scenePos().y();
}