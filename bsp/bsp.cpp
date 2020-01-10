#include "bsp.h"

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
