#ifndef POLYGON_H
#define POLYGON_H

#include <QJsonArray>
#include <QLine>
#include <QVector>

class Polygon
{
public:
  Polygon();

  void read(const QJsonArray& json_array);

  QVector<QLine> lines() const;

private:
  QVector<QLine> lines_;
};

#endif // POLYGON_H
