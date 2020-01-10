#include "bsp.h"

PointWrtLine PointOnSide(const QPoint& point, const WorldLine& word_line) {
  const QPoint direction = word_line.p1() - point;
  const auto dot_product = QPointF::dotProduct(direction, word_line.normal());
  if (dot_product < -0.001) {
    return PointWrtLine::FRONT;
  } else if (dot_product > 0.001) {
    return PointWrtLine::BACK;
  } else {
    return PointWrtLine::COLINEAR;
  }
}

LineWrtLine LineOnSide(const WorldLine& line, const WorldLine& dividing_line) {
  const auto point_on_side_1 = PointOnSide(line.p0(), dividing_line);
  const auto point_on_side_2 = PointOnSide(line.p1(), dividing_line);

  if (point_on_side_1 == point_on_side_2) {
    if (point_on_side_1 == PointWrtLine::FRONT ||
        point_on_side_1 == PointWrtLine::COLINEAR) {
      return LineWrtLine::FRONT;
    } else {
      return LineWrtLine::BACK;
    }
  } else {
    return LineWrtLine::INTERSECT;
  }
}
