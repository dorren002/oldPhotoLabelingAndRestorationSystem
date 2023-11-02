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
	if (!maskPixmap.isNull()) {
		img = maskPixmap;

		if (!img.isNull())
		{
			painter->setOpacity(opacity);//透明度设置
			painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), img);
		}
	}
}

void ImageScene::showForeground() {
	opacity = 0.4;
	update(0, 0, this->width(), this->height());
}

void ImageScene::hideForeground() {
	opacity = 0;
	update(0, 0, this->width(), this->height());
}

void ImageScene::updateForeground(QImage pixmap) {
	maskPixmap = QPixmap::fromImage(pixmap);
	update(0, 0, this->width(), this->height());
}

bool ImageScene::isForegroundHidden() {
	if (opacity == 0) {
		return true;
	}
	else {
		return false;
	}
}
