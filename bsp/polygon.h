#ifndef POLYGON_H
#define POLYGON_H

#include "worldline.h"

#include <QJsonArray>
#include <QLine>
#include <QVector>

class Polygon
{
  public:
    Polygon();

    void read(const QJsonArray& json_array);

    std::vector<WorldLine> lines() const;

  private:
    std::vector<WorldLine> lines_;
};

#endif // POLYGON_H
