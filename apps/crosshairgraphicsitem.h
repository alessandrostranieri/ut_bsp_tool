#ifndef CROSSHAIRGRAPHICSITEM_H
#define CROSSHAIRGRAPHICSITEM_H

#include <QGraphicsItem>

// FORWARD DECLARATIONS
class QGraphicsLineItem;

class CrosshairGraphicsItem : public QGraphicsItem
{
public:
    CrosshairGraphicsItem(QPointF position, qreal sideLength = 20);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF position_;
    qreal sideLength_;

    QGraphicsLineItem* vertical_;
    QGraphicsLineItem* horizontal_;
};

#endif // CROSSHAIRGRAPHICSITEM_H
