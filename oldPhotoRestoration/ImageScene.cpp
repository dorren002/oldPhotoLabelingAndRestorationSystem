#include "ImageScene.h"
#include "qpainter.h"

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event); // 将点击事件向下传递到item中
}

void ImageScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	QPixmap foreImg;
	foreImg.load("F:/input.png");
	if (!foreImg.isNull())
	{
		painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), foreImg);
	}

}
void ImageScene::drawForeground(QPainter* painter, const QRectF& rect)
{
	QPixmap foreImg;
	foreImg.load("F:/mask.png");
	if (!foreImg.isNull())
	{
		painter->setOpacity(0.5);//透明度设置
		painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), foreImg);
	}
}