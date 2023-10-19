#pragma once
#include <qgraphicsitem.h>
class ImageItem :
    public QGraphicsPixmapItem
{
public:
    float posX;
    float posY;

    ImageItem(QGraphicsItem* parent);
    ImageItem(const QPixmap& pixmap, QGraphicsItem* parent);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

