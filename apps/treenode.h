#ifndef TREENODE_H
#define TREENODE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

class TreeNode : public QGraphicsItem
{
public:
    static TreeNode* make_node(std::uint32_t id, QPointF position);
public:
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    TreeNode(std::uint32_t id, QPointF position);
    void init();
private:
    std::uint32_t id_;
    QPointF position_;
    QGraphicsEllipseItem* circle_;
};

#endif // TREENODE_H
