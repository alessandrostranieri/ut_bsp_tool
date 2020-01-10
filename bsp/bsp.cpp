#include "bsp.h"

PointWrtLine PointOnSide(const WorldLine& word_line, const QPoint point) {
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
