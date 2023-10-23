#include "ImageScene.h"
#include "qpainter.h"
#include "qdebug.h"

#include <QGraphicsSceneMouseEvent>

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	cursorX = event->scenePos().x();
	cursorY = event->scenePos().y();
}

void ImageScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	QPixmap img;
	img.load(backgroundImgPath);
	if (!img.isNull())
	{
		painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), img);
	}

}

void ImageScene::drawForeground(QPainter* painter, const QRectF& rect)
{
	QPixmap img;
	if (!foregroundImgPath.isEmpty()) {
		img.load(foregroundImgPath);
	}
	else if(!maskPixmap.isNull()){
		img = maskPixmap;
	}
	
	if (!img.isNull())
	{
		painter->setOpacity(0.5);//透明度设置
		painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), img);
	}
}

void ImageScene::setForePathEmpty() {
	foregroundImgPath = "";
}

void ImageScene::updateForeImg(QImage pixmap) {
	setForePathEmpty();
	maskPixmap = QPixmap::fromImage(pixmap);
	update(0, 0, this->width(), this->height());
}

