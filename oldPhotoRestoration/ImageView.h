#pragma once
#include <qgraphicsview.h>

class ImageView :
    public QGraphicsView
{
public:
    QPointF posAnchor;
    QPointF centerAnchor;

    ImageView(QWidget *parent = 0);
    void setCenter(int widht, int height);
    void setScaleRatio(double ratio);
    void setCursorType(Qt::CursorShape ctype);
private:
    Qt::CursorShape cursorType;
    bool isMousePressed;

    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
};

