#include "polygon.h"

#include <QJsonObject>

Polygon::Polygon()
{
  
}

void Polygon::read(const QJsonArray& json_array) {
  lines_.clear();

  for (auto i = 0; i < json_array.size(); i++) {
    QJsonObject line = json_array[i].toObject();

    auto from = line["from"].toArray();
    auto to = line["to"].toArray();

    lines_.push_back(QLine{QPoint{from[0].toInt(), from[1].toInt()},
                           QPoint{to[0].toInt(), to[1].toInt()}});
  }
}

QVector<QLine> Polygon::lines() const { return lines_; }
