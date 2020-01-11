#include "bsp.h"

#include <cassert>

constexpr std::int32_t SPLIT_IMPORTANCE = 8;

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

std::optional<size_t> SelectBestSplitter(const std::vector<WorldLine>& lines) {
  std::optional<size_t> best_splitter_index;
  std::int32_t best_score = std::numeric_limits<std::int32_t>::max();
  for (size_t i_splitter = 0; i_splitter < lines.size(); i_splitter++) {
    const auto& splitter = lines[i_splitter];

    std::int32_t front_facing{0};
    std::int32_t back_facing{0};
    std::int32_t splits{0};

    for (size_t i_line = 0; i_line < lines.size(); i_line++) {
      const auto& line = lines[i_line];

      if (line != splitter) {
        const auto side = LineOnSide(line, splitter);
        switch (side) {
        case LineWrtLine::FRONT:
          front_facing += 1;
          break;
        case LineWrtLine::BACK:
          back_facing += 1;
          break;
        case LineWrtLine::INTERSECT:
          splits += 1;
          break;
        default:
          break;
        }
      }
    }
    auto score = abs(front_facing - back_facing) + (splits * SPLIT_IMPORTANCE);
    if (score < best_score) {
      best_score = score;
      best_splitter_index = i_splitter;
    }
  }
  return best_splitter_index;
}

BspNode BuildBspTree(std::vector<WorldLine> lines) {

  BspNode node;
  // SELECT BEST SPLIT
  auto best_split_index = SelectBestSplitter(lines);

  if (!best_split_index.has_value()) {
    // NO SPLIT MEANS NO LINES OR ALL LINES CAN BE PRINTED AT THE SAME TIME
    node.lines = lines;
  } else {
    // APPLY SPLIT
    // LEFT LINES
    // RIGHT LINES

    // RECURSE CONSTRUCTION
  }
  return node;
}

SplitResult SplitLine(const WorldLine& line, const WorldLine& split_line) {
  QPointF intersection;
  QLineF::IntersectType intersect_type =
      QLineF(line.p0(), line.p1())
          .intersect(QLineF(split_line.p0(), split_line.p1()), &intersection);

  assert(intersect_type != QLineF::IntersectType::NoIntersection);
  PointWrtLine p0_side = PointOnSide(line.p0(), split_line);

  if (p0_side == PointWrtLine::FRONT) {
    return SplitResult{
        WorldLine{QLine{line.p0(), intersection.toPoint()}, line.normal()},
        WorldLine{QLine{intersection.toPoint(), line.p1()}, line.normal()}};
  } else { // P1 in front
    return SplitResult{
        WorldLine{QLine{intersection.toPoint(), line.p1()}, line.normal()},
        WorldLine{QLine{line.p0(), intersection.toPoint()}, line.normal()}};
  }
}
