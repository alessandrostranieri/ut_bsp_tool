#ifndef BSP_H
#define BSP_H

#include "worldline.h"

#include <optional>

enum class PointWrtLine { FRONT, BACK, COLINEAR };

PointWrtLine PointOnSide(const QPoint& point, const WorldLine& word_line);

enum class LineWrtLine { FRONT, BACK, INTERSECT };

LineWrtLine LineOnSide(const WorldLine& line, const WorldLine& dividing_line);

std::optional<size_t> SelectBestSplitter(const std::vector<WorldLine>& lines);

#endif // BSP_H
