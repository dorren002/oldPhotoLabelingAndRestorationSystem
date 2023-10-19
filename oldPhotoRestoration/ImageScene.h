#pragma once
#include <qgraphicsscene.h>
class ImageScene :
    public QGraphicsScene
{
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

