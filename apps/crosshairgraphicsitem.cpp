#include "crosshairgraphicsitem.h"

#include <QGraphicsLineItem>
#include <QPen>

CrosshairGraphicsItem::CrosshairGraphicsItem(QPointF position, qreal sideLength) :
    position_(position),
    sideLength_(sideLength)
{
    const auto halfSideLength = sideLength / 2;

    vertical_ = new QGraphicsLineItem(QLineF{position - QPointF(0, halfSideLength), position + QPointF(0, halfSideLength)});
    vertical_->setPen(QPen(Qt::red));

    horizontal_ = new QGraphicsLineItem(QLineF{position - QPointF{halfSideLength, 0}, QPointF{halfSideLength, 0}});
    horizontal_->setPen(QPen(Qt::red));
}

QRectF CrosshairGraphicsItem::boundingRect() const
{
    return QRectF{
        position_.x() - sideLength_/2,
        position_.y() - sideLength_/2,
        sideLength_,
        sideLength_
    };
}

void CrosshairGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    vertical_->paint(painter, option, widget);
    horizontal_->paint(painter, option, widget);
}
