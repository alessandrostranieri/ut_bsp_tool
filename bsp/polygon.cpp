#include "polygon.h"

#include <QJsonObject>

Polygon::Polygon()
{
  
}

void Polygon::read(const QJsonArray& json_array) {
  lines_.clear();

  for (auto i = 0; i < json_array.size(); i++) {
    const auto line_json = json_array[i].toObject();
    WorldLine worldLine;
    worldLine.read(line_json);
    lines_.push_back(worldLine);
  }
}

std::vector<WorldLine> Polygon::lines() const {
    return lines_;
}


