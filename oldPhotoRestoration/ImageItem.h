#pragma once
#include <qgraphicsitem.h>
class ImageItem :
    public QGraphicsPixmapItem
{
public:
    int posX;
    int posY;

    ImageItem(QGraphicsItem* parent);
    ImageItem(const QPixmap& pixmap, QGraphicsItem* parent);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

