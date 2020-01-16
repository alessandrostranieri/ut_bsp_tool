#ifndef TREENODE_H
#define TREENODE_H

// FORWARD DECLARATIONS
class QGraphicsLineItem;
class QGraphicsSimpleTextItem;

#include <QGraphicsItem>

class TreeNode : public QGraphicsItem
{
public:
    TreeNode(std::uint32_t id, QPointF position);
public:
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    std::uint32_t id_;
    QPointF position_;
    QGraphicsEllipseItem* circle_;
    QGraphicsSimpleTextItem* idText_;
};

#endif // TREENODE_H
