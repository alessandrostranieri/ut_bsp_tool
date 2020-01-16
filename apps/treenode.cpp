#include "treenode.h"

#include <QPen>
#include <QDebug>

constexpr double NODE_RADIUS = 10.0;

TreeNode::TreeNode(uint32_t id, QPointF position) : id_(id), position_(position)
{
}

void TreeNode::init()
{
    circle_ = new QGraphicsEllipseItem(position_.rx() - NODE_RADIUS,
                                       position_.ry() - NODE_RADIUS,
                                       NODE_RADIUS * 2,
                                       NODE_RADIUS * 2,
                                       this);
    qDebug() << circle_->pos();
    circle_->setPen(QPen(Qt::yellow));
}

TreeNode *TreeNode::make_node(uint32_t id, QPointF position)
{
    TreeNode* node = new TreeNode(id, position);
    node->init();
    return node;
}

QRectF TreeNode::boundingRect() const
{
    return circle_->boundingRect();
}

void TreeNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    circle_->paint(painter, option, widget);
}
