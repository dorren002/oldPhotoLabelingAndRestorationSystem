#pragma once
#include <qgraphicsscene.h>
class ImageScene :
    public QGraphicsScene
{
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void drawBackground(QPainter* painter, const QRectF& rect);
};

