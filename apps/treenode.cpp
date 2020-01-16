#include "treenode.h"

#include <QPen>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>

constexpr double NODE_RADIUS = 20.0;
constexpr double HIGHLIGHT_RADIOUS = NODE_RADIUS * 1.25;

TreeNode::TreeNode(uint32_t id, QPointF position) :
    id_(id), position_(position)
{
    circle_ = new QGraphicsEllipseItem(position_.rx() - NODE_RADIUS,
                                       position_.ry() - NODE_RADIUS,
                                       NODE_RADIUS * 2,
                                       NODE_RADIUS * 2,
                                       this);
    circle_->setPen(QPen(Qt::yellow));
    idText_ = new QGraphicsSimpleTextItem(QString{"%1"}.arg(id_), circle_);
    idText_->setPen(QPen(Qt::yellow));
    idText_->setPos(position_- QPointF(5, 10));
}

QRectF TreeNode::boundingRect() const
{
    return circle_->boundingRect();
}

void TreeNode::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}
