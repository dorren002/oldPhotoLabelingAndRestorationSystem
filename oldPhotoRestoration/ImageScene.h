#pragma once
#include <qgraphicsscene.h>
class ImageScene :
    public QGraphicsScene
{
public:
    int cursorX;
    int cursorY;
    QPixmap maskPixmap;
    QString backgroundImgPath = "F:/input.png";
    QString foregroundImgPath = ""; // 非加载本地图像情况需要置为空

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void updateForeImg(QImage pixmap);
    void drawForeground(QPainter* painter, const QRectF& rect);
    void drawBackground(QPainter* painter, const QRectF& rect);
private:
    void setForePathEmpty();
};
