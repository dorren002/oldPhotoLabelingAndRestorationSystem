#pragma once
#include <qgraphicsscene.h>
class ImageScene :
    public QGraphicsScene
{
public:
    int cursorX;
    int cursorY;

    QString backgroundImgPath = "./image/upload.jpg";

    bool isForegroundHidden();
    
    void hideForeground();
    void showForeground();
    void updateForeground(QImage pixmap);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void drawBackground(QPainter* painter, const QRectF& rect);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
private:
    float opacity = 0.4;

    QPixmap maskPixmap;
};
