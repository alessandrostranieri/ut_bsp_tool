#include "partitionline.h"

#include <QGraphicsLineItem>

PartitionLine::PartitionLine(QLineF line, PartitionLine *parentLine, QGraphicsItem* parent) :
    QGraphicsLineItem(line, parent), parentLine_(parentLine)
{
}

PartitionLine *PartitionLine::getParentLine() const
{
    return parentLine_;
}
