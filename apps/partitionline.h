#ifndef PARTITIONLINE_H
#define PARTITIONLINE_H

#include <QGraphicsLineItem>

class PartitionLine : public QGraphicsLineItem
{

public:
    PartitionLine(QLineF line, PartitionLine *parentLine_, QGraphicsItem* parentItem = nullptr);

    PartitionLine* getParentLine() const;

private:
    PartitionLine* parentLine_;
};

#endif // PARTITIONLINE_H
